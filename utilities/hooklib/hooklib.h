#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>

#define NOT_FOUND -1

// init our handler outside of class
LONG WINAPI VEHHandler(EXCEPTION_POINTERS* pExceptionInfo);

struct HookStatus {
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

    // AC 
    BOOL                     bACHooksOverwritten;
    // bytes of hooked virtualprotect, will be filled out
    BYTE                     wHookedBytes[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
    // bytes of original virtualprotect function
    BYTE                    wOrigBytes[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

    // prototypes
    using tVirtualQuery = SIZE_T(__stdcall*)(LPCVOID, PMEMORY_BASIC_INFORMATION, SIZE_T);

    // global vars
public:
    tVirtualQuery oVirtualQuery;

    // private functions
private:
    BOOL DestroyPointers(int index = NOT_FOUND);

public:
    HookLib() { iCounter = 0; bVehInit = false; pVEHHandle = NULL; pVTableAddr = NULL; } // constructor

    // If hooking where an ac is present, call this BEFORE hooking everything
    BOOL OverrideACHooks();

    // If hooking where an ac is present, call this AFTER hooking everything 
    BOOL RestoreACHooks();

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
    PVOID GetPointerDestructor(int index) { return reinterpret_cast<PVOID>( pPointerDestructor.at(index)); } // get destructed pointer at index i for the handler
    PVOID GetBasePointer(int index) { return reinterpret_cast<PVOID>(pOrigFncAddr.at(index)); }// get base fnc pointer at index i for the handler
    BOOL GetACHookStatus() { return bACHooksOverwritten; }
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