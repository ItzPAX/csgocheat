#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"
#include "directx/endscene.h"
#include "clientmode/clientmode.h"
#include "studiorender/studiorender.h"
#include "sdk/manager/interface/interface.h"

HookManager g_HookManager{ };

#pragma region HookDefs
namespace DrawModel {
	using tDrawModel = void(__fastcall*)(void*, void*, DrawModelResults*, const DrawModelInfo&, Matrix*, float*, float*, const Vec3D&, int);
	tDrawModel oDrawModel = nullptr;

	int iIndex = 29;
	__forceinline void __fastcall hkDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags);
}

namespace WndProc {
	using tWndProc = LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);
	tWndProc oWndProc = nullptr;

	bool bInputReceived = false;

	__forceinline LRESULT __stdcall hkWndProc(const HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

namespace CreateMove {
	using tCreateMove = bool(__stdcall*)(float, CUserCmd*);
	tCreateMove oCreateMove = nullptr;
	inline int iIndex = 24;
	__forceinline bool __stdcall hkCreateMove(float flInputSampleTime, CUserCmd* cmd);
}

namespace EndScene {
	using tEndScene = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);
	tEndScene oEndScene = nullptr;

	inline static bool bDraw = true;
	inline int iIndex = 42;
	void* pD3D9Device[119];
	BYTE pEndSceneBytes[7]{ 0 };
	
	__forceinline void __stdcall hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
}
#pragma endregion

bool HookManager::AddAllHooks() {
	// override hook placed by vac in VirtualProtect
	if (!g_HookLib.OverrideVirtualProtect())
		return false;

	 // hook directx, maybe also hook reset in the future
	if (g_DirectX.GetD3D9Device(EndScene::pD3D9Device, sizeof(EndScene::pD3D9Device))) {
		memcpy(EndScene::pEndSceneBytes, (char*)EndScene::pD3D9Device[EndScene::iIndex], 7);
		EndScene::oEndScene = (EndScene::tEndScene)g_HookLib.TrampHook((char*)EndScene::pD3D9Device[EndScene::iIndex], (char*)EndScene::hkEndScene, 7);
	}

	// hook windows functions
	WndProc::oWndProc = (WndProc::tWndProc)SetWindowLong(g_DirectX.window, GWL_WNDPROC, (LONG)WndProc::hkWndProc);

	// grab original function address and add hook to the queue
	CreateMove::oCreateMove = (CreateMove::tCreateMove)g_HookLib.AddHook(CreateMove::hkCreateMove, g_Interface.pClientMode, CreateMove::iIndex, "CreateMove");
	DrawModel::oDrawModel = (DrawModel::tDrawModel)g_HookLib.AddHook(DrawModel::hkDrawModel, g_Interface.pStudioRender, DrawModel::iIndex, "DrawModel");

	// forward original func pointer to different classes
	g_Chams.c_oDrawModel = DrawModel::oDrawModel;

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

	// manually remove the trampoline hook
	g_HookLib.Patch((char*)EndScene::pD3D9Device[42], (char*)EndScene::pEndSceneBytes, 7);

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
void __fastcall DrawModel::hkDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) {
	if (Game::g_pLocal && !g_DirectX.bDrawing) {
		cDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}

	DrawModel::oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
}	

LRESULT __stdcall WndProc::hkWndProc(const HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (!WndProc::bInputReceived && GetAsyncKeyState(VK_INSERT) & 0x01) {
		bInputReceived = true;
		g_Menu.bToggled = !g_Menu.bToggled;
		// ghetto fix but idc :)
		g_Menu.bToggled ? g_Interface.pConsole->Activate() : g_Interface.pConsole->Hide();
	}

	if (WndProc::bInputReceived && GetAsyncKeyState(VK_INSERT) & 0x8000)
		bInputReceived = false;

	if (g_Menu.bToggled) { // menu opened give input to imgui
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}

	// menu closed restore normal input
	CallWindowProc(WndProc::oWndProc, hwnd, uMsg, wParam, lParam);
	return true;
}

bool __stdcall CreateMove::hkCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	cCreateMove(flInputSampleTime, cmd); // relay function

	return CreateMove::oCreateMove(flInputSampleTime, cmd);
}

void __stdcall EndScene::hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!g_DirectX.pDevice)
		g_DirectX.pDevice = o_pDevice;

	// we are drawing in endscene rn
	g_DirectX.bDrawing = true;

	if (Game::g_pLocal && EndScene::bDraw)
		cEndScene(); // relay function

	cMenu(); // menu shit

	// we have stopped drawing
	g_DirectX.bDrawing = false;

	EndScene::bDraw = !EndScene::bDraw;
	EndScene::oEndScene(g_DirectX.pDevice);
}
#pragma endregion These Functions call the real functions in different cpp files withing the hooks dir