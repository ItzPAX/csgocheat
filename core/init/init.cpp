#include "../../includes.h"

Init g_Init{ };

ulong __stdcall Init::OnInject(void* p) {
	// injection started, allocate a console to log
	if (g_Init.bAllocConsole)
		AllocConsole();

	// first we add all of our hooks
	if (!g_HookManager.AddAllHooks(true))
		return 0;

	// next we try to init all hooks
	if (!g_HookManager.InitAllHooks(true))
		return 0;

	return 1;
}