#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>

#define NOT_FOUND -1

// init our handler outside of class
LONG WINAPI VEHHandler(EXCEPTION_POINTERS* pExceptionInfo);

struct HookStatus {
public:
    HookStatus() { pBaseFnc = NULL; pHkAddr = nullptr; iIndex = NULL; }
    uintptr_t pBaseFnc;
    PVOID pHkAddr;
    INT   iIndex;
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
    PVOID                    pVEHHandle;
    PVOID                    pVTableAddr;
    INT                      iCounter;
    BOOL                     bVehInit;

    // private functions
private:
    BOOL DestroyPointers(int index = NOT_FOUND);

public:
    HookLib() { iCounter = 0; bVehInit = false; pVEHHandle = NULL; pVTableAddr = NULL; } // constructor

    // if VirtualProtect was hooked via a usermode hook, this will override it
    BOOL OverrideVirtualProtect();

#pragma region VEHHook
    // hooks function and returns a pointer to the original function, only works on virtual function pointers
    LPVOID AddHook(PVOID pHkFunc, PVOID pVTable, INT16 iIndex, const char* sName = "");

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
    uintptr_t GetPointerDestructor(int index) { return pPointerDestructor.at(index); } // get destructed pointer at index i for the handler
#pragma endregion VEHHandler will call these

#pragma region TrampHook
    // hooks function and returns pointer to the original function, works on all functions
    VOID Patch(char* dst, char* src, short len);
    BOOL Hook(char* src, char* dst, short len);
    char* TrampHook(char* src, char* dst, short len);
#pragma endregion Hook using inline patching

    // functions to debug, only works if hook placed via VEH
    HookStatus GetHookInfo(const char* sName, int ind = NOT_FOUND);
};

extern HookLib g_HookLib;