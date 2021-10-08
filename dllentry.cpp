#include "includes.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		// Create thread and init our cheat
		HANDLE hThread = CreateThread(nullptr, 0, Init::OnInject, nullptr, 0, nullptr);
		if (!hThread)
			return 0;
		return 1;
	}
}
