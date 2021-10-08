#include "pch.h"
#include "includes.h"

#pragma region HookDefs
namespace ExampleFunction {
	using ExampleFunctionT = void (__stdcall*)(double); // prototype for the original function
	ExampleFunctionT oFunc = nullptr; // original function, add hook will return a pointer for this

	int iIndex = NULL; // index of function to hook in the vtable
	PVOID pVTable = nullptr; // the vtable itself

	void __stdcall hkExampleFunction(double d) {}; // the hook function, set accordingly to the prototype
}

#pragma endregion


HookManager g_HookManager;

bool HookManager::AddAllHooks(bool bPrint) {
	// grab original function address and add hook to the queue
	ExampleFunction::oFunc = (ExampleFunction::ExampleFunctionT)g_HookLib.AddHook(ExampleFunction::hkExampleFunction, ExampleFunction::pVTable, ExampleFunction::iIndex, "ExampleFunction");


	if (bPrint) std::cout << "[HOOKS] Successfully added all hooks to queue" << std::endl;
	g_HookManager.bHooksAdded = true;
	return true;
}

bool HookManager::InitAllHooks(bool bPrint) {
	// we have not yet successfully called AddAllHooks, or have hooks queued
	if (!g_HookManager.bHooksAdded || g_HookLib.GetCounter() <= 0)
		return false;

	if (bPrint) std::cout << "[HOOKS] Started initializing all queued hooks" << std::endl;
	bool bRet = g_HookLib.InitHooks();
	if (bPrint) std::cout << "[HOOKS] Finished initializing all queued hooks" << std::endl;
	return bRet;
}

bool HookManager::ReleaseAll() {
	// check if there are any hooks to release
	if (g_HookLib.GetCounter() <= 0)
		return false;

	// we have hooks to release, lets call the function
	g_HookLib.ReleaseAll();
}

HookStatus HookManager::GetHookInfo(const char* sName) {
	// check if a name has been entered
	if (sName == "")
		return HookStatus();

	return g_HookLib.GetHookInfo(sName);
}

void HookManager::PrintHookStatus(HookStatus hs) {
	std::cout << "[INDEX] " << hs.iIndex << std::endl;
	std::cout << "[HKFNC] " << hs.pHkAddr << std::endl;
	std::cout << "[BASE] " << hs.pBaseFnc << std::endl;
}