#include <Windows.h>
#include <iostream>
#include "Hooks.h"

void main(HMODULE hMod)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
	
    HMODULE hSteamService = GetModuleHandleA("steamservice.dll");

    if (hSteamService) {
        if (!InitVACHooks((void*)hSteamService))
            TerminateProcess(GetCurrentProcess(), 666);
    }
	
    while (true)
        Sleep(1);

    //FreeLibraryAndExitThread(hMod, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

