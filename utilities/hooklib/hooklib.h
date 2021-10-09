#pragma once

#include <Windows.h>
#include <vector>

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
    std::vector<const char*>  pName;
    std::vector<PVOID>        pHkFnc;
    std::vector<uintptr_t>    pBaseFnc;
    std::vector<uintptr_t>    pPointerDestructor;
    std::vector<uintptr_t>    pOrigFncAddr;
    std::vector<UINT16>        nIndex;
    PVOID                    pVEHHandle;
    PVOID                    pVTableAddr;
    INT                      iCounter;

    // private functions
private:
    BOOL DestroyPointers();

public:
    HookLib() { iCounter = 0; pVEHHandle = nullptr; pVTableAddr = nullptr; } // constructor

    // hooks function and returns a pointer to the original function, only works on virtual function pointers
#pragma region VEHHook
    LPVOID AddHook(PVOID pHkFunc, PVOID pVTable, UINT16 iIndex, const char* sName = "");
    BOOL InitHooks();
    VOID ReleaseAll();
#pragma endregion Hook using Pointer Destruction

    // hooks function and returns pointer to the original function, works on all functions
#pragma region TrampHook
    VOID Patch(char* dst, char* src, short len);
    BOOL Hook(char* src, char* dst, short len);
    char* TrampHook(char* src, char* dst, short len);
#pragma endregion Hook using inline patching

#pragma region HandlerCalls
    INT GetCounter() { return iCounter; }     // get icounter for the handler
    PVOID GetHookFnc(int index) { return pHkFnc.at(index); }    // get hooked function addr at index i for the handler
    uintptr_t GetPointerDestructor(int index) { return pPointerDestructor.at(index); } // get destructed pointer at index i for the handler
#pragma endregion VEHHandler will call these

    // functions to debug, only works if hook placed via VEH
    HookStatus GetHookInfo(const char* sName);
};

extern HookLib g_HookLib;