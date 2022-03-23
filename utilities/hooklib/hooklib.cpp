#include "hooklib.h"
#include "csgocheat/Syscalls.h"

namespace hkFunctions {
    SIZE_T __stdcall hkVirtualQuery(LPCVOID lpAddr, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T lpSize) {
        SIZE_T sReturnVal = g_HookLib.oVirtualQuery(lpAddr, lpBuffer, lpSize);
        bool bTamper = false;

        // we finished hooking, or havent started yet
        if (!g_HookLib.GetACHookStatus())
            return sReturnVal;

        // tamper with return vals only if function has been hooked by us
        if (lpAddr == VirtualProtect) bTamper = true;
        for (int i = 0; i < g_HookLib.GetCounter(); i++) {
            if (lpAddr == g_HookLib.GetBasePointer(i))
                bTamper = true;
        }
        for (int i = 0; i < g_HookLib.GetCodeCaveSize(); i++) {
            if (lpAddr == (void*)g_HookLib.GetCodeCaveAddr(i))
                bTamper = true;
        }

        if (lpBuffer && bTamper) {
            lpBuffer->AllocationProtect = PAGE_EXECUTE_READ;
            lpBuffer->Protect = PAGE_EXECUTE_READ;
        }

        return sReturnVal;
    } 
}

HookLib g_HookLib{ };

// The VEHHandler, it will change EIP accordingly to the hooked function we added
#pragma region VEHHandler
LONG __stdcall VEHHandler(EXCEPTION_POINTERS* pExceptionInfo) {
    DWORD dwAddr = NOT_FOUND;
    PVOID pExceptAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;

    // Find the address that triggered the error
    for (int i = 0; i < g_HookLib.GetCounter(); i++) {
        if (g_HookLib.GetPointerDestructor(i) == pExceptAddr) {
            dwAddr = (DWORD)g_HookLib.GetHkFnc(i);
            break;
        }
    }

    // check if we found an exception at a hooked address, if we do change eip and continue execution
    if (dwAddr != -1 && pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        pExceptionInfo->ContextRecord->Eip = dwAddr;
        return EXCEPTION_CONTINUE_EXECUTION;
    }

    // if we arrive here, there was no excpetion at a hook, so we let the normal handler deal with it
    return EXCEPTION_CONTINUE_SEARCH;
}
#pragma endregion

#pragma region VEHHook
BOOL HookLib::OverrideACHooks() {
    DWORD oProc;

    // copy the old bytes to restore the hook later
    memcpy(wHookedBytes, VirtualProtect, 5);

    // override the ac hook with original bytes, since vac is stupid they dont scan for writecopy
    //VirtualProtect(VirtualProtect, 5, PAGE_EXECUTE_WRITECOPY, &oProc);
    //memcpy(VirtualProtect, wOrigBytes, 5);
    //VirtualProtect(VirtualProtect, 5, oProc, &oProc);

    // hook virtualquery func and return false info
    oVirtualQuery = (tVirtualQuery)TrampHook((char*)VirtualQuery, (char*)hkFunctions::hkVirtualQuery, 5);

    bACHooksOverwritten = true;

    return true;
}

BOOL HookLib::RestoreACHooks() {
    // no hooks overwritten, so we can't patch anything
    if (!bACHooksOverwritten)
        return false;

    // dont restore the virtualquery hook, since we just overwrote the hook from vac

    // restore the virtualprotect hook, we aren't hooked rn so this call is fine
    DWORD oProc;

   /* VirtualProtect(VirtualProtect, 5, PAGE_EXECUTE_WRITECOPY, &oProc);
    memcpy(VirtualProtect, wHookedBytes, 5);
    VirtualProtect(VirtualProtect, 5, oProc, &oProc);*/

    bACHooksOverwritten = false;

    return true;
}

// This function will break all the pointers to trigger an excpetion and call the VEHHandler
BOOL HookLib::DestroyPointers(int index) {
    if (index == NOT_FOUND) {
        for (int i = 0; i < iCounter; i++) {
            DWORD dwOProc;
            VirtualProtect((LPVOID)(pBaseFnc.at(i)), sizeof((pBaseFnc.at(i))), PAGE_EXECUTE_READWRITE, &dwOProc); // override protection
            *((uintptr_t*)pBaseFnc.at(i)) = (uintptr_t)pPointerDestructor.at(i); // break pointer
            VirtualProtect((LPVOID)(pBaseFnc.at(i)), sizeof((pBaseFnc.at(i))), dwOProc, &dwOProc); // override to old protection
        }
    }

    else {
        DWORD dwOProc;
        VirtualProtect((LPVOID)(pBaseFnc.at(index)), sizeof((pBaseFnc.at(index))), PAGE_EXECUTE_READWRITE, &dwOProc); // override protection
        *((uintptr_t*)pBaseFnc.at(index)) = (uintptr_t)pPointerDestructor.at(index); // break pointer
        VirtualProtect((LPVOID)(pBaseFnc.at(index)), sizeof((pBaseFnc.at(index))), dwOProc, &dwOProc); // override to old protection
    }

    // if we arrive here all hooks have successfully been placed
    return true;
}

LPVOID HookLib::AddHook(PVOID pHkFunc, PVOID pVTable, INT16 iIndex, const char* sName) {
    if (iMode == MODE_VEH) {
        // push back new hook values
        pName.push_back(sName);
        pVTableAddr = pVTable;
        nIndex.push_back(iIndex);
        pHkFnc.push_back(pHkFunc);
        pBaseFnc.push_back(*((uintptr_t*)pVTableAddr) + (sizeof(uintptr_t) * nIndex.at(iCounter)));
        pOrigFncAddr.push_back(*((uintptr_t*)(pBaseFnc.at(iCounter))));
        (nIndex.at(iCounter) > 0) ? pPointerDestructor.push_back(*((uintptr_t*)pVTableAddr) - 1) : pPointerDestructor.push_back(*((uintptr_t*)pVTableAddr) + 1);

        // get original function address
        uintptr_t pRetVal = pOrigFncAddr.at(iCounter);

        // increment hook counter
        iCounter++;
        return (LPVOID)pRetVal;
    }
    return nullptr;
}

BOOL HookLib::EnableAllHooks() {
    if (iMode == MODE_VEH) {
        if (!bVehInit)
            g_HookLib.pVEHHandle = RtlAddVectoredHandler(true, (PVECTORED_EXCEPTION_HANDLER)VEHHandler);

        // we didnt manage to register a handler
        if (!pVEHHandle)
            return false;

        // Something went wrong when trying to trigger the exception
        if (!DestroyPointers())
            return false;

        return true;
    }
    return false;
}

BOOL HookLib::EnableHook(const char* sName, int ind) {
    if (iMode == MODE_VEH) {
        int index = NOT_FOUND;

        for (int i = 0; i < iCounter; i++) {
            if (pName[i] == sName) {
                index = i;
            }
        }

        // check if given ind is in bounds and only assign if we didnt find a matching name
        bool bIndIsOk = ind < iCounter&& ind != NOT_FOUND;
        if (bIndIsOk && index == NOT_FOUND)
            index = ind;

        // add the handler
        if (!bVehInit)
            g_HookLib.pVEHHandle = RtlAddVectoredHandler(true, (PVECTORED_EXCEPTION_HANDLER)VEHHandler);

        // we didnt manage to register a handler
        if (!pVEHHandle)
            return false;

        // Something went wrong when trying to trigger the exception
        if (index == NOT_FOUND || !DestroyPointers(index))
            return false;

        return true;
    }
    return false;
}

VOID HookLib::DisableHook(const char* sName, int ind) {
    if (iMode == MODE_VEH) {
        int index = NOT_FOUND;

        // find index based on name
        for (int i = 0; i < iCounter; i++) {
            if (pName[i] == sName) {
                index = i;
            }
        }

        // check if given ind is in bounds and only assign if we didnt find a matching name
        bool bIndIsOk = ind < iCounter&& ind != NOT_FOUND;
        if (bIndIsOk && index == NOT_FOUND)
            index = ind;

        // name and/or index not found
        if (index == NOT_FOUND)
            return;

        // unhook
        DWORD dwOProc;
        VirtualProtect((LPVOID)(pBaseFnc.at(index)), sizeof((pBaseFnc.at(index))), PAGE_EXECUTE_READWRITE, &dwOProc); // override protection
        *((uintptr_t*)pBaseFnc.at(index)) = (uintptr_t)pOrigFncAddr.at(index); // restore pointer
        VirtualProtect((LPVOID)(pBaseFnc.at(index)), sizeof((pBaseFnc.at(index))), dwOProc, &dwOProc); // override to old protection
    }
}

VOID HookLib::DisableAllHooks() {
    if (iMode == MODE_VEH) {
        for (int i = 0; i < iCounter; i++) {
            DWORD dwOProc;
            VirtualProtect((LPVOID)(pBaseFnc.at(i)), sizeof((pBaseFnc.at(i))), PAGE_EXECUTE_READWRITE, &dwOProc); // override protection
            *((uintptr_t*)pBaseFnc.at(i)) = (uintptr_t)pOrigFncAddr.at(i); // restore pointer
            VirtualProtect((LPVOID)(pBaseFnc.at(i)), sizeof((pBaseFnc.at(i))), dwOProc, &dwOProc); // override to old protection
        }

        // remove vectored exceptionhandler
        RemoveVectoredExceptionHandler(pVEHHandle);
        pVEHHandle = NULL;
    }
}
#pragma endregion

#pragma region TrampHook
VOID HookLib::Patch(char* dst, char* src, SIZE_T len) {
    //NtProtectVirtualMemory(hProc, &ppCodeCave, (PSIZE_T)&size, PAGE_EXECUTE_READWRITE, &oProc);
    DWORD oProc;
    NtProtectVirtualMemory(hProc, (LPVOID*)&dst, &len, PAGE_EXECUTE_READWRITE, &oProc); //VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oProc);
    memcpy(dst, src, len);
    NtProtectVirtualMemory(hProc, (LPVOID*)&dst, &len, oProc, &oProc);                  //VirtualProtect(dst, len, oProc, &oProc);
}

BOOL HookLib::Hook(char* src, char* dst, SIZE_T len) {
    if (len < 5) return false;

    DWORD oProc;
    NtProtectVirtualMemory(hProc, (LPVOID*)&dst, &len, PAGE_EXECUTE_READWRITE, &oProc); //VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
    memset(src, 0x90, len);
    uintptr_t relAddy = (uintptr_t)(dst - src - 5);
    *src = (char)0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
    NtProtectVirtualMemory(hProc, (LPVOID*)&dst, &len, oProc, &oProc);

    return true;
}

char* HookLib::TrampHook(char* src, char* dst, short len) {
    if (len < 5) return 0;

    SIZE_T allocateLen = len + 5;

    LPVOID x = nullptr;
    char* gateway = (char*)NtAllocateVirtualMemory(hProc, &x, 0, (PULONG)&allocateLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);    //VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!gateway)
        return nullptr;

    memcpy(gateway, src, len);
    uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (char)0xE9;
    *(uintptr_t*)(gateway + len + 1) = jumpAddy;
    if (Hook(src, dst, len)) {
        return gateway;
    }
    return nullptr;
}
#pragma endregion

HookStatus HookLib::GetHookInfo(const char* sName, int ind) {
    // loop through hooks and search for name match
    int index = NOT_FOUND;

    for (int i = 0; i < iCounter; i++) {
        if (pName.at(i) == sName)
            index = i;
    }

    if (index == NOT_FOUND) {
        index = ind;
    }

    if (index != NOT_FOUND) {
        HookStatus hs;
        hs.iIndex = index;
        hs.pBaseFnc = pBaseFnc.at(index);
        hs.pHkAddr = pHkFnc.at(index);

        return hs;
    }

    return HookStatus();
}

uintptr_t HookLib::FindCodeCave(const char* cModuleName, size_t iSize) {
    MODULEINFO moduleInfo = GetModuleInfo(cModuleName);
    uintptr_t pFinalAddr = 0x00;
    BYTE* moduleContent = (BYTE*)malloc(moduleInfo.SizeOfImage);
    if (!moduleContent)
        return 0x0;

    memcpy(moduleContent, moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage - 1);

    for (int i = 1000; i < moduleInfo.SizeOfImage; i++)
    {
        bool found = true;
        for (int j = 0; j < iSize + 1; j++)
        {
            if (moduleContent[i + j] != 0x00)
                found = false;
        }

        uintptr_t pCodeCave = ((uintptr_t)moduleInfo.lpBaseOfDll + i);
        CodeCave cave{ pCodeCave, iSize };
        cCodeCaves.push_back(cave);

        if (found == true)
            return ((uintptr_t)moduleInfo.lpBaseOfDll + i);
    }

    return 0x0;
}

uintptr_t HookLib::AddHook(const char* cModuleName, void* pVirtualTable, void* pTargetFunction, size_t iIndex) {
    if (iMode == MODE_TRUSTEDMODULE) {
        // get addr of the VTableEntry
        uintptr_t pVTable = *((uintptr_t*)pVirtualTable);
        uintptr_t pEntry = pVTable + (sizeof(uintptr_t) * iIndex);
        uintptr_t pOrig = *((uintptr_t*)pEntry);

        uintptr_t pCodeCave = FindCodeCave(cModuleName, SIZE);
        if (!pCodeCave)
            return NULL;

        std::cout << pCodeCave << std::endl;

        uintptr_t pRelAddr = (uintptr_t)pTargetFunction - pCodeCave - SIZE;

        // place our shellcode in the code cave
        DWORD oProc;

        int sizeOfEntry = sizeof(pEntry);
        int size = SIZE;

        LPVOID ppCodeCave = (LPVOID)pCodeCave;
        LPVOID ppEntry = (LPVOID)pEntry;
        
        NtProtectVirtualMemory(hProc, &ppCodeCave, (PSIZE_T)&size, PAGE_EXECUTE_READWRITE, &oProc);       //VirtualProtect((void*)pCodeCave, SIZE, PAGE_EXECUTE_WRITECOPY, &oProc);
        *(uintptr_t*)(pCodeCave) = (char)0x8B;
        *(uintptr_t*)(pCodeCave + 0x01) = (char)0xED;
        *(uintptr_t*)(pCodeCave + 0x02) = (char)0xE9;
        *(uintptr_t*)(pCodeCave + 0x03) = (uintptr_t)pRelAddr;
        NtProtectVirtualMemory(hProc, &ppCodeCave, (PSIZE_T)&size, oProc, &oProc);                        //VirtualProtect((void*)pCodeCave, SIZE, oProc, NULL);

        // swap pointer to our code cave
        NtProtectVirtualMemory(hProc, &ppEntry, (PSIZE_T)&sizeOfEntry, PAGE_EXECUTE_WRITECOPY, &oProc);//VirtualProtect((LPVOID)pEntry, sizeof(pEntry), PAGE_EXECUTE_WRITECOPY, &oProc);
        *(uintptr_t*)pEntry = (uintptr_t)pCodeCave;
        NtProtectVirtualMemory(hProc, &ppEntry, (PSIZE_T)&sizeOfEntry, oProc, &oProc);                   //VirtualProtect((LPVOID)pEntry, sizeof(pEntry), oProc, NULL);

        return pOrig;
    }
    return NULL;
}