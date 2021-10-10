#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"
#include "directx/endscene.h"

HookManager g_HookManager{ };

#pragma region HookDefs
namespace EndScene {
	using tEndScene = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9 pDevice);
	inline tEndScene oEndScene = nullptr;

	inline BYTE EndSceneBytes[7]; // stolen bytes
	inline void* d3d9Device[119]; // vtable
	inline int iIndex = 42; // index of endscene

	__forceinline HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
}
#pragma endregion

bool HookManager::AddAllHooks() {
	//grab original function address and add hook to the queue

	// hook EndScene via Trampoline not VEH
	if (g_DirectX.GetD3D9Device(EndScene::d3d9Device, sizeof(EndScene::d3d9Device))) {
		memcpy(EndScene::EndSceneBytes, (char*)EndScene::d3d9Device[EndScene::iIndex], 7);
		EndScene::oEndScene = (EndScene::tEndScene)g_HookLib.TrampHook((char*)EndScene::d3d9Device[EndScene::iIndex], (char*)EndScene::hkEndScene, 7);
	}

	g_HookManager.bHooksAdded = true;
	g_HookManager.iCounter = g_HookLib.GetCounter();
	return true;
}

bool HookManager::InitAllHooks() {
	// we have not yet successfully called AddAllHooks
	if (!g_HookManager.bHooksAdded)
		return false;

	return g_HookLib.InitHooks();
}

bool HookManager::ReleaseAll() {
	// release every tramphook manually
	Patch((char*)EndScene::d3d9Device[EndScene::iIndex], (char*)EndScene::EndSceneBytes, 7);

	// check if there are any hooks to release
	if (g_HookManager.iCounter <= 0)
		return false;

	// we have hooks to release, lets call the function
	g_HookLib.ReleaseAll();
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
HRESULT __stdcall EndScene::hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice) {
		pDevice = o_pDevice;
	}

	cheatEndScene(pDevice);
	return oEndScene(pDevice);
}
#pragma endregion These Functions call the real functions in different cpp files withing the hooks dir