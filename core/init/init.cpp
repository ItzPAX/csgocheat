#include "pch.h"
#include "includes.h"

Init g_Init{ };

ulong __stdcall Init::InitAll(void* p) {
	// wait for serverbrowser dll
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

	// injection started, allocate a console
	AllocConsole(); FILE* f; freopen_s(&f, "CONOUT$", "w", stdout);

	// find all interfaces
	std::cout << "[ RAYBOT ] Bruteforcing Interfaces...\n";
	if (!g_Interface.Init())
		return 0;
	std::cout << "[ RAYBOT ] Successfully Initialized Interfaces\n";

	// then we add all of our hooks
	if (!g_HookManager.AddAllHooks())
		return 0;
	std::cout << "[ RAYBOT ] Successfully added Hooks to queue\n";

	// now we try to init all hooks
	if (!g_HookManager.InitAllHooks())
		return 0;
	std::cout << "[ RAYBOT ] Successfully Initialized Hooks\n";

	if (!g_Render.InitRenderer())
		return 0;
	std::cout << "[ RAYBOT ] Successfully Initialized Renderer\n";

	return 1;
}

void Init::Unload(HINSTANCE hInstance) {
	// release all hooks
	if (!g_HookManager.ReleaseAll())
		return;

	// free console
	FreeConsole();

	// exit thread
	FreeLibraryAndExitThread(hInstance, 0);
}