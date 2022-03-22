#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"
#include "directx/endscene.h"
#include "directx/beginscene.h"
#include "clientmode/clientmode.h"
#include "studiorender/studiorender.h"
#include "sdk/manager/interface/interface.h"
#include "client/hudupdate.h"

HookManager g_HookManager{ };

#pragma region HookDefs
namespace SvCheats {
	using tSvCheats = bool(__thiscall*)(void*);
	tSvCheats oSvCheats = nullptr;

	BYTE pOrigBytes[7] = {0};

	int iIndex = 13;
	__forceinline bool __fastcall hkSvCheats(void* ConVar, int edx);
}

namespace HudUpdate {
	using tHudUpdate = void(__stdcall*)(bool);
	tHudUpdate oHudUpdate = nullptr;

	int iIndex = 11;
	__forceinline void __stdcall hkHudUpdate(bool bActive);
}
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
namespace HkDirectX {
	void* pD3D9Device[119];
	static DWORD dwAllowedReturn = 0;

	using tEndScene = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);
	tEndScene oEndScene = nullptr;

	int iEndScene = 42;

	BYTE pEndSceneBytes[7]{ 0 };

	__forceinline HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 o_pDevice);
}
namespace LockCursor {
	using tLockCursor = void(__stdcall*)();
	tLockCursor oLockCursor = nullptr;

	int iIndex = 67;
	__forceinline void __stdcall hkLockCursor();
}
#pragma endregion

bool HookManager::AddAllHooks() {
	// override hooks placed by vac
	if (!g_HookLib.OverrideACHooks())
		return false;

	// hook directx
	if (g_DirectX.GetD3D9Device(HkDirectX::pD3D9Device, sizeof(HkDirectX::pD3D9Device))) {
		memcpy(HkDirectX::pEndSceneBytes, (char*)HkDirectX::pD3D9Device[HkDirectX::iEndScene], 7);
		HkDirectX::oEndScene = (HkDirectX::tEndScene)g_HookLib.TrampHook((char*)HkDirectX::pD3D9Device[HkDirectX::iEndScene], (char*)HkDirectX::hkEndScene, 7);
	}

	// hook svcheats
	ConVar* pSvCheats = g_Interface.pICVar->FindVar(XOR("sv_cheats"));
	SvCheats::oSvCheats = (SvCheats::tSvCheats)g_HookLib.AddHook(XOR("client.dll"), pSvCheats, SvCheats::hkSvCheats, SvCheats::iIndex);

	// hook windows functions
	WndProc::oWndProc = (WndProc::tWndProc)SetWindowLong(g_DirectX.window, GWL_WNDPROC, (LONG)WndProc::hkWndProc);

	// grab original function address and add hook to the queue
	CreateMove::oCreateMove = (CreateMove::tCreateMove)g_HookLib.AddHook(XOR("engine.dll"), g_Interface.pClientMode, CreateMove::hkCreateMove, CreateMove::iIndex);
	DrawModel::oDrawModel = (DrawModel::tDrawModel)g_HookLib.AddHook(XOR("client.dll"), g_Interface.pStudioRender, DrawModel::hkDrawModel, DrawModel::iIndex);
	HudUpdate::oHudUpdate = (HudUpdate::tHudUpdate)g_HookLib.AddHook(XOR("engine.dll"), g_Interface.pClient, HudUpdate::hkHudUpdate, HudUpdate::iIndex);
	LockCursor::oLockCursor = (LockCursor::tLockCursor)g_HookLib.AddHook(XOR("client.dll"), g_Interface.pSurface, LockCursor::hkLockCursor, LockCursor::iIndex);

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

	bool bStatus = g_HookLib.EnableAllHooks();

	// restore hooks placed by vac
	g_HookLib.RestoreACHooks();
	return bStatus;
}

bool HookManager::ReleaseAll() {
	// check if there are any hooks to release
	if (g_HookManager.iCounter <= 0)
		return false;

	// manually remove the trampoline hook
	g_HookLib.Patch((char*)HkDirectX::pD3D9Device[42], (char*)HkDirectX::pEndSceneBytes, 7);

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
	for (int i = 0; i < sName[i] != 0; i++)
		ihs.name += sName[i];

	return ihs;
}

void HookManager::LogHookStatus(IHookStatus ihs) {
	if (ihs.name->empty())
		return;

	ihs.name->append(XOR(".txt"));
	auto outFile = std::ofstream(ihs.name->c_str());
	outFile << XOR("[INDEX] ") << ihs.iIndex << std::endl;
	outFile << XOR("[HKFNC] ") << ihs.pHkAddr << std::endl;
	outFile << XOR("[BASE] ") << ihs.pBaseFnc << std::endl;

	std::cout << XOR("[INDEX] ") << ihs.iIndex << std::endl;
	std::cout << XOR("[HKFNC] ") << ihs.pHkAddr << std::endl;
	std::cout << XOR("[BASE] ") << ihs.pBaseFnc << std::endl;
}

#pragma region HkFunctions
bool __fastcall SvCheats::hkSvCheats(void* ConVar, int edx) {
	static auto pCamThink = reinterpret_cast<void*>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x85\xC0\x75\x30\x38\x86"), XOR("xxxxxx")));
	
	if (!ConVar)
		return false;
	
	if ((_ReturnAddress()) == pCamThink)
		return true;
	return oSvCheats(ConVar);
}

void __stdcall HudUpdate::hkHudUpdate(bool bActive) {
	// do worldtoscreen here (only once per frame)
	if (Game::g_pLocal)
		cHudUpdate();
	oHudUpdate(bActive);
}
void __fastcall DrawModel::hkDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) {
	if (Game::g_pLocal)
		cDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	DrawModel::oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
}	
bool __stdcall CreateMove::hkCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	// relay function
	cCreateMove(flInputSampleTime, cmd);
	return false;
}
HRESULT __stdcall HkDirectX::hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!g_DirectX.pDevice)
		g_DirectX.pDevice = o_pDevice;

	if (!g_Render.bInitialized) {
		// init d3d9 renderer
		if (!g_Render.InitRenderer())
			return;
		std::cout << XOR("[ RAYBOT ] Successfully Initialized Renderer\n");
	}

	// get return address
	DWORD dwReturnAddress;
	__asm
	{
		push eax
		mov eax, [ebp + 4]
		mov dwReturnAddress, eax
		pop eax
	}

	// only draw if return address is our own device
	if (dwAllowedReturn == 0 || dwAllowedReturn == dwReturnAddress) {
		if (Game::g_pLocal)
			cEndScene(); // relay function
		cMenu(); // menu shit

		dwAllowedReturn = dwReturnAddress;
	}

	return oEndScene(g_DirectX.pDevice);
}
void __stdcall LockCursor::hkLockCursor() {
	if (g_Menu.bToggled) {
		g_Interface.pSurface->UnlockCursor();
		return;
	}
	LockCursor::oLockCursor();
}
LRESULT __stdcall WndProc::hkWndProc(const HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (!WndProc::bInputReceived && GetAsyncKeyState(VK_INSERT) & 0x01) {
		bInputReceived = true;
		g_Menu.bToggled = !g_Menu.bToggled;

		ImGui::GetIO().MouseDrawCursor = !g_Menu.bToggled;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	if (WndProc::bInputReceived && GetAsyncKeyState(VK_INSERT) & 0x8000)
		bInputReceived = false;

	if (g_Menu.bToggled) {// menu opened give input to imgui
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}

	// menu closed restore normal input
	CallWindowProc(WndProc::oWndProc, hwnd, uMsg, wParam, lParam);
	return true;
}
#pragma endregion These Functions call the real functions in different cpp files withing the hooks dir