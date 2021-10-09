#include "pch.h"
#include "hooklib.h"

HookLib g_HookLib{ };

// The VEHHandler, it will change EIP accordingly to the hooked function we added
LONG __stdcall VEHHandler(EXCEPTION_POINTERS* pExceptionInfo) {
    DWORD dwAddr = -1;
    PVOID pExceptAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;

    // Find the address that triggered the error
    for (int i = 0; i < g_HookLib.GetCounter(); i++) {
        if ((PVOID)g_HookLib.GetPointerDestructor(i) == pExceptAddr) {
            dwAddr = (DWORD)g_HookLib.GetHookFnc(i);
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


// This function will break all the pointers to trigger an excpetion and call the VEHHandler
BOOL HookLib::DestroyPointers() {
    for (int i = 0; i < iCounter; i++) {
        DWORD dwOProc;
        VirtualProtect((LPVOID)(pBaseFnc.at(iCounter)), sizeof((pBaseFnc.at(iCounter))), PAGE_EXECUTE_READWRITE, &dwOProc); // override protection
        *((uintptr_t*)pBaseFnc.at(iCounter)) = (uintptr_t)pPointerDestructor.at(iCounter); // break pointer
        VirtualProtect((LPVOID)(pBaseFnc.at(iCounter)), sizeof((pBaseFnc.at(iCounter))), dwOProc, &dwOProc); // override to old protection
    }

    // if we arrive here all hooks have successfully been placed
    return true;
}

LPVOID HookLib::AddHook(PVOID pHkFunc, PVOID pVTable, UINT16 iIndex, const char* sName) {
    // push back new hook values
    pName.push_back(sName);
    pVTableAddr = pVTable;
    nIndex.push_back(iIndex);
    pHkFnc.push_back(pHkFunc);
    pBaseFnc.push_back(*((uintptr_t*)pVTableAddr) + (sizeof(uintptr_t) * nIndex.at(iCounter)));
    pOrigFncAddr.push_back(*((uintptr_t*)(pBaseFnc.at(iCounter))));
    (nIndex.at(iCounter) > 0) ? pPointerDestructor.push_back(*((uintptr_t*)pVTableAddr) - 1) : pPointerDestructor.push_back(*((uintptr_t*)pVTableAddr) + 1);

    // get original function address
    uintptr_t pRetVal = g_HookLib.pOrigFncAddr.at(iCounter);

    // increment hook counter
    g_HookLib.iCounter++;
    return (LPVOID)pRetVal;
}

BOOL HookLib::InitHooks() {
    if (!g_HookLib.pVEHHandle)
        g_HookLib.pVEHHandle = AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)VEHHandler);

    // we didnt manage to register a handler
    if (!g_HookLib.pVEHHandle)
        return false;

    // Something went wrong when trying to trigger the exception
    if (!DestroyPointers())
        return false;

    return true;
}

VOID HookLib::ReleaseAll() {
    return VOID();
}

VOID HookLib::Patch(char* dst, char* src, short len) {
    DWORD oProc;
    VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oProc);
    memcpy(dst, src, len);
    VirtualProtect(dst, len, oProc, &oProc);
}

BOOL HookLib::Hook(char* src, char* dst, short len) {
    if (len < 5) return false;

    DWORD oProc;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
    memset(src, 0x90, len);
    uintptr_t relAddy = (uintptr_t)(dst - src - 5);
    *src = (char)0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
    VirtualProtect(src, len, oProc, &oProc);

    return true;
}

char* HookLib::TrampHook(char* src, char* dst, short len) {
    if (len < 5) return 0;

    char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);
    uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (char)0xE9;
    *(uintptr_t*)(gateway + len + 1) = jumpAddy;
    if (Hook(src, dst, len)) {
        return gateway;
    }
    return nullptr;
}

HookStatus HookLib::GetHookInfo(const char* sName) {
    // loop through hooks and search for name match
    for (int i = 0; i < iCounter; i++) {
        if (pName.at(i) == sName) {
            HookStatus hs;
            hs.iIndex = i;
            hs.pBaseFnc = pBaseFnc.at(i);
            hs.pHkAddr = pHkFnc.at(i);

            return hs;
        }
    }

    return HookStatus();
}