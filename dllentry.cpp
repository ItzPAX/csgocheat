#include "pch.h"
#include "includes.h"

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		// dll has been injected
	case DLL_PROCESS_ATTACH: {
		// set instance
		Game::g_hInstance = hinstDLL;

		// Create thread and init our cheat
		HANDLE hThread = CreateThread(nullptr, NULL, Init::InitAll, hinstDLL, NULL, nullptr);
		if (!hThread)
			return 0;
		CloseHandle(hThread);
		break;
	}

		// dll has been detached
	case DLL_PROCESS_DETACH:
		g_Init.Unload(hinstDLL);
		break;
	}

	return 1;
}