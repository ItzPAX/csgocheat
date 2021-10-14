#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"
#include "paint/paint.h"

HookManager g_HookManager{ };

#pragma region HookDefs
namespace PaintTraverse {
	using tPaintTraverse = void(__thiscall*)(IPanel*, unsigned int, bool, bool);
	tPaintTraverse oPaintTraverse = nullptr;

	inline int iIndex = 41;
	__forceinline void __stdcall hkPaintTraverse(unsigned int panel, bool forcerepaint, bool allowforce);
}
#pragma endregion

bool HookManager::AddAllHooks() {
	//grab original function address and add hook to the queue
	PaintTraverse::oPaintTraverse = (PaintTraverse::tPaintTraverse)g_HookLib.AddHook(PaintTraverse::hkPaintTraverse, g_Interface.pPanel, PaintTraverse::iIndex, "PaintTraverse");


	g_HookManager.bHooksAdded = true;
	g_HookManager.iCounter = g_HookLib.GetCounter();
	return true;
}

bool HookManager::InitAllHooks() {
	// we have not yet successfully called AddAllHooks
	if (!g_HookManager.bHooksAdded)
		return false;

	return g_HookLib.EnableAllHooks();
}

bool HookManager::ReleaseAll() {
	// check if there are any hooks to release
	if (g_HookManager.iCounter <= 0)
		return false;

	// we have hooks to release, lets call the function
	g_HookLib.DisableAllHooks();
	return true;
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

VOID HookManager::Patch(char* dst, char* src, short len) { g_HookLib.Patch(dst, src, len); }

void HookManager::LogHookStatus(IHookStatus ihs) {
	if (ihs.name == "")
		return;
	auto outFile = std::ofstream(ihs.name);
	outFile << "[INDEX] " << ihs.iIndex << std::endl;
	outFile << "[HKFNC] " << ihs.pHkAddr << std::endl;
	outFile << "[BASE] " << ihs.pBaseFnc << std::endl;

	std::cout << "[INDEX] " << ihs.iIndex << std::endl;
	std::cout << "[HKFNC] " << ihs.pHkAddr << std::endl;
	std::cout << "[BASE] " << ihs.pBaseFnc << std::endl;
}

#pragma region HkFunctions
void __stdcall PaintTraverse::hkPaintTraverse(unsigned int panel, bool forcerepaint, bool allowforce) {
	g_Interface.pEngine->GetScreenSize(Game::iScreenX, Game::iScreenY);
	cPaintTraverse(); // real function, this is where the magic happens
	oPaintTraverse(g_Interface.pPanel, panel, forcerepaint, allowforce);
}
#pragma endregion These Functions call the real functions in different cpp files withing the hooks dir