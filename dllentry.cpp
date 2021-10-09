#include "pch.h"
#include "includes.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		// dll has been injected
	case DLL_PROCESS_ATTACH: {
		// Create thread and init our cheat
		HANDLE hThread = CreateThread(nullptr, NULL, Init::OnInject, hinstDLL, NULL, nullptr);
		if (hThread) CloseHandle(hThread);
		break;
	}

		// dll has been detached
	case DLL_PROCESS_DETACH:
		g_Init.OnUnload(hinstDLL);
		break; 	

	}
	return 1;
}