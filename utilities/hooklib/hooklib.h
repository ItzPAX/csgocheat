#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>

#define NOT_FOUND -1
#define SIZE 7

enum Mode {
	MODE_NORMAL,
    MODE_VEH,
    MODE_TRUSTEDMODULE
};

// init our handler outside of class
LONG WINAPI VEHHandler(EXCEPTION_POINTERS* pExceptionInfo);

struct HookStatus {
    HookStatus() { pBaseFnc = NULL; pHkAddr = nullptr; iIndex = NULL; }
    uintptr_t pBaseFnc;
    PVOID pHkAddr;
    INT   iIndex;
};

struct CodeCave {
    uintptr_t pAddr;
    size_t iSize;
};

class HookLib {
    // vars
private:
    std::vector<const char*> pName;
    std::vector<PVOID>       pHkFnc;
    std::vector<uintptr_t>   pBaseFnc;
    std::vector<uintptr_t>   pPointerDestructor;
    std::vector<uintptr_t>   pOrigFncAddr;
    std::vector<INT16>       nIndex;
    std::vector<CodeCave>    cCodeCaves;
    PVOID                    pVEHHandle;
    PVOID                    pVTableAddr;
    INT                      iCounter;
    BOOL                     bVehInit;

    // AC 
    BOOL                     bACHooksOverwritten;
    // bytes of hooked virtualprotect, will be filled out
    BYTE                     wHookedBytes[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    // bytes of original virtualprotect function
    BYTE                     wOrigBytes[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

    Mode                    iMode = MODE_TRUSTEDMODULE;

    // prototypes
    using tVirtualQuery = SIZE_T(__stdcall*)(LPCVOID, PMEMORY_BASIC_INFORMATION, SIZE_T);
    using tGetProcAddress = FARPROC(__stdcall*)(HMODULE, LPCSTR);
    using tRtlAddVectoredHandler = PVOID(NTAPI*)(IN ULONG FirstHandler, IN PVECTORED_EXCEPTION_HANDLER VectoredHandler);

    // global vars
public:
    HANDLE hProc;
    tGetProcAddress oGetProcAddress;
    tVirtualQuery oVirtualQuery;
    tRtlAddVectoredHandler RtlAddVectoredHandler;
	
    BOOL                     criticalHook;
    BOOL                     criticalNotSet;

    // private functions
private:
    BOOL DestroyPointers(int index = NOT_FOUND);

public:
    HookLib() { 
        iCounter = 0; 
        bVehInit = false; 
        pVEHHandle = NULL; 
        pVTableAddr = NULL; 
        RtlAddVectoredHandler = reinterpret_cast<tRtlAddVectoredHandler>(GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlAddVectoredExceptionHandler"));
        hProc = GetCurrentProcess();
        criticalHook = true;    //since this is csgo
        criticalNotSet = false;
    } // constructor
	
    void PlaceInspectionHooks();    //experiment with getprocaddr

    // If hooking where an ac is present, call this BEFORE hooking everything
    BOOL OverrideACHooks();
    // If hooking where an ac is present, call this AFTER hooking everything 
    BOOL RestoreACHooks();

    void SetMode(Mode mode) {
        iMode = mode;
    }

#pragma region VEHHook
    /// FOR MODE = VEH
    LPVOID AddHook(PVOID pHkFunc, PVOID pVTable, INT16 iIndex, const char* sName = "");
    /// FOR MODE = TRUSTEDMODULE
    char* AddHook(const char* cModuleName, void* pVTable, void* pTargetFunction, size_t iIndex);

    // enable all added hooks
    BOOL EnableAllHooks();
    // enable hook either by given name or index
    BOOL EnableHook(const char* sName, int ind = NOT_FOUND);

    // disable all added hooks
    VOID DisableAllHooks();
    // disable hook either by given name or index
    VOID DisableHook(const char* sName, int ind = NOT_FOUND);
#pragma endregion Hook using Pointer Destruction
#pragma region HandlerCalls
    INT GetCounter() { return iCounter; }     // get icounter for the handler
    PVOID GetHkFnc(int index) { return pHkFnc.at(index); }    // get hooked function addr at index i for the handler
    PVOID GetPointerDestructor(int index) { return reinterpret_cast<PVOID>( pPointerDestructor.at(index)); } // get destructed pointer at index i for the handler
    PVOID GetBasePointer(int index) { return reinterpret_cast<PVOID>(pOrigFncAddr.at(index)); }// get base fnc pointer at index i for the handler
    BOOL GetACHookStatus() { return bACHooksOverwritten; }
#pragma endregion VEHHandler will call these

#pragma region TrampHook
    // hooks function and returns pointer to the original function, works on all functions
    VOID Patch(char* dst, char* src, SIZE_T len);
    BOOL Hook(char* src, char* dst, SIZE_T len);
    char* TrampHook(char* src, char* dst, short len);
#pragma endregion Hook using inline patching

#pragma region MarlinHook
    uintptr_t MarlinHook(uintptr_t target, uintptr_t trampoline, bool* enabled);
#pragma endregion

#pragma region TRUSTEDMODULE
    int GetCodeCaveSize() {
        return cCodeCaves.size();
    }
    uintptr_t GetCodeCaveAddr(int index) {
        return cCodeCaves.at(index).pAddr;
    }

    MODULEINFO GetModuleInfo(const char* szModule) {
        MODULEINFO modInfo = { 0 };
        HMODULE hModule = GetModuleHandle(szModule);
        if (hModule == 0)
            return modInfo;

        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
        return modInfo;
    }

    uintptr_t FindCodeCave(const char* cModuleName, size_t iSize);
#pragma endregion

    // functions to debug, only works if hook placed via VEH
    HookStatus GetHookInfo(const char* sName, int ind = NOT_FOUND);
};

extern HookLib g_HookLib;