#include "pch.h"
#include "includes.h"
#include "convar.h"
#include "core/security/security.h"

Init g_Init{ };

extern "C" void* internal_cleancall_wow64_gate = 0;

ulong __stdcall Init::InitAll(void* p) {
	// injection started, allocate a console
	AllocConsole(); FILE* f; freopen_s(&f, XOR("CONOUT$"), XOR("w"), stdout);

	//PerformSecurity();

	// wait for serverbrowser dll
	while (!GetModuleHandle(XOR("serverbrowser.dll"))) {
		//std::cout << XOR("[ RAYBOT ] Waiting for serverbrowser.dll...\n");
		Sleep(200);
	}

	internal_cleancall_wow64_gate = (void*)__readfsdword(0xC0);

	// find all interfaces
	std::cout << XOR("[ RAYBOT ] Bruteforcing Interfaces...\n");
	if (!g_Interface.Init())
		return 0;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized Interfaces\n");

	// init materials for chams
	if (!g_Chams.InitMaterials())
		return 0;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized Materials\n");

	// then we add all of our hooks
	if (!g_HookManager.AddAllHooks())
		return 0;
	std::cout << XOR("[ RAYBOT ] Successfully added Hooks to queue\n");

	// now we try to init all hooks
	if (!g_HookManager.InitAllHooks())
		return 0;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized Hooks\n");

	if (!InitConvars())
		return 0;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized ConVars\n");

	g_Config.LoadDefault();
	std::cout << XOR("[ RAYBOT ] Loaded default config: ") << g_Config.GetDefault() << std::endl;

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