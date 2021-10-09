#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"

#pragma region HookDefs

namespace ExampleFunction {
	using ExampleFunctionT = void (__stdcall*)(double); // prototype for the original function
	ExampleFunctionT oFunc = nullptr; // original function, add hook will return a pointer for this

	int iIndex = NULL; // index of function to hook in the vtable
	PVOID pVTable = nullptr; // the vtable itself

	void __stdcall hkExampleFunction(double d) { oFunc(d); }; // the hook function, set accordingly to the prototype
}

#pragma endregion


HookManager g_HookManager{ };

bool HookManager::AddAllHooks() {
	//grab original function address and add hook to the queue
	//ExampleFunction::oFunc = (ExampleFunction::ExampleFunctionT)g_HookLib.AddHook(ExampleFunction::hkExampleFunction, ExampleFunction::pVTable, ExampleFunction::iIndex, "ExampleFunction");


	g_HookManager.bHooksAdded = true;
	g_HookManager.iCounter = g_HookLib.GetCounter();
	return true;
}

bool HookManager::InitAllHooks() {
	// we have not yet successfully called AddAllHooks, or have hooks queued
	if (!g_HookManager.bHooksAdded)
		return false;

	return g_HookLib.InitHooks();
}

bool HookManager::ReleaseAll() {
	// check if there are any hooks to release
	if (g_HookManager.iCounter <= 0)
		return false;

	// we have hooks to release, lets call the function
	g_HookLib.ReleaseAll();
}

IHookStatus HookManager::GetHookInfo(const char* sName) {
	// check if a name has been entered
	if (sName == "")
		return IHookStatus();

	// create both structs
	IHookStatus ihs = IHookStatus();
	HookStatus hs = g_HookLib.GetHookInfo(sName);

	// copy hs struct to ihs struct
	ihs.iIndex = hs.iIndex;
	ihs.pBaseFnc = hs.pBaseFnc;
	ihs.pHkAddr = hs.pHkAddr;
	ihs.name = sName;

	return ihs;
}

void HookManager::LogHookStatus(IHookStatus ihs) {
	if (ihs.name == "")
		return;
	auto outFile = std::ofstream(ihs.name);
	outFile << "[INDEX] " << ihs.iIndex << std::endl;
	outFile << "[HKFNC] " << ihs.pHkAddr << std::endl;
	outFile << "[BASE] " << ihs.pBaseFnc << std::endl;
}