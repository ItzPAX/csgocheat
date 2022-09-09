#include "pch.h"
#include "includes.h"
#include "utilities/hooklib/hooklib.h"
#include "directx/endscene.h"
#include "directx/beginscene.h"
#include "clientmode/clientmode.h"
#include "studiorender/studiorender.h"
#include "sdk/manager/interface/interface.h"
#include "client/hudupdate.h"
#include "client/framestagenotify.h"

HookManager g_HookManager{ };

#pragma region HookDefs
namespace PaintTraverse {
	using tPaintTraverse = void(__thiscall*)(IPanel*, unsigned int, bool, bool);
	tPaintTraverse oPaintTraverse = nullptr;

	int iIndex = 41;
	__forceinline void __stdcall hkPaintTraverse(unsigned int iPanel, bool bForceRepaint, bool bAllowForce);
}

namespace DrawModel {
	using tDrawModel = void(__fastcall*)(void*, void*, DrawModelResults*, const DrawModelInfo&, Matrix*, float*, float*, const Vec3D&, int);
	tDrawModel oDrawModel = nullptr;

	int iIndex = 29;
	__forceinline void __fastcall hkDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags);
}
namespace IsPaused {
	using tIsPaused = bool(__fastcall*)(void*);
	tIsPaused oIsPaused = nullptr;

	int iIndex = 90;
	__forceinline bool __fastcall hkIsPaused(void* thisptr);
}
namespace IsHLTV {
	using tIsHLTV = bool(__fastcall*)(void*, void*);
	tIsHLTV oIsHLTV = nullptr;

	int iIndex = 93;
	__forceinline bool __fastcall hkIsHLTV(void* thisptr, void* edx);
}
namespace SvCheats {
	using tSvCheats = bool(__thiscall*)(void*);
	tSvCheats oSvCheats = nullptr;

	int iIndex = 13;
	__forceinline bool __fastcall hkSvCheats(void* ConVar, int edx);
}
namespace DoPostScreenSpaceEffects {
	using tDoPostScreenSpaceEffects = void(__fastcall*)(IClientMode*, int, const void*);
	tDoPostScreenSpaceEffects oDoPostScreenSpaceEffects = nullptr;

	int iIndex = 44;
	__forceinline void __fastcall hkDoPostScreenSpaceEffects(IClientMode* icmptr, int edx, const void* pViewSetup);
}
namespace FSN {
	using tFrameStageNotify = void(__thiscall*)(void*, IBaseClientDLL::ClientFrameStage_t);
	tFrameStageNotify oFrameStageNotify = nullptr;

	int iIndex = 37;
	__forceinline void __stdcall hkFrameStageNotfy(IBaseClientDLL::ClientFrameStage_t curStage);
}
namespace HudUpdate {
	using tHudUpdate = void(__stdcall*)(bool);
	tHudUpdate oHudUpdate = nullptr;

	int iIndex = 11;
	__forceinline void __stdcall hkHudUpdate(bool bActive);
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

	HWND hCSGOWindow = NULL;
	while (!(hCSGOWindow != FindWindowA(XOR("Valve001"), nullptr))) {
		Sleep(500);
	}

	// hook directx
	if (g_DirectX.GetD3D9Device(HkDirectX::pD3D9Device, sizeof(HkDirectX::pD3D9Device))) {
		memcpy(HkDirectX::pEndSceneBytes, (char*)HkDirectX::pD3D9Device[HkDirectX::iEndScene], 7);
		g_HookLib.AddHook(HookEntry((char*)HkDirectX::pD3D9Device[HkDirectX::iEndScene], (char*)HkDirectX::hkEndScene, 7, (PVOID*) & HkDirectX::oEndScene));
	}

	// hook windows functions
	WndProc::oWndProc = (WndProc::tWndProc)SetWindowLong(g_DirectX.window, GWL_WNDPROC, (LONG)WndProc::hkWndProc);

	// grab original function address and add hook to the queue
	g_HookLib.AddHook(HookEntry("materialsystem.dll", g_Interface.pClientMode, CreateMove::hkCreateMove, CreateMove::iIndex, (PVOID*)&CreateMove::oCreateMove));
	g_HookLib.AddHook(HookEntry("materialsystem.dll", g_Interface.pClientMode, DoPostScreenSpaceEffects::hkDoPostScreenSpaceEffects, DoPostScreenSpaceEffects::iIndex, (PVOID*)&DoPostScreenSpaceEffects::oDoPostScreenSpaceEffects));
	g_HookLib.AddHook(HookEntry("materialsystem.dll", g_Interface.pStudioRender, DrawModel::hkDrawModel, DrawModel::iIndex, (PVOID*)&DrawModel::oDrawModel));
	g_HookLib.AddHook(HookEntry("panorama.dll", g_Interface.pClient, HudUpdate::hkHudUpdate, HudUpdate::iIndex, (PVOID*)&HudUpdate::oHudUpdate));
	g_HookLib.AddHook(HookEntry("engine.dll", g_Interface.pEngine, IsHLTV::hkIsHLTV, IsHLTV::iIndex, (PVOID*)&IsHLTV::oIsHLTV));
	g_HookLib.AddHook(HookEntry("panorama.dll", g_Interface.pSurface, LockCursor::hkLockCursor, LockCursor::iIndex, (PVOID*)&LockCursor::oLockCursor));
	ConVar* pSvCheats = g_Interface.pICVar->FindVar(XOR("sv_cheats"));
	g_HookLib.AddHook(HookEntry("panorama.dll", pSvCheats, SvCheats::hkSvCheats, SvCheats::iIndex, (PVOID*)&SvCheats::oSvCheats));
	g_HookLib.AddHook(HookEntry("panorama.dll", g_Interface.pClient, FSN::hkFrameStageNotfy, FSN::iIndex, (PVOID*)&FSN::oFrameStageNotify));
	g_HookLib.AddHook(HookEntry("engine.dll", g_Interface.pEngine, IsPaused::hkIsPaused, IsPaused::iIndex, (PVOID*)&IsPaused::oIsPaused));
	g_HookLib.AddHook(HookEntry("engine.dll", g_Interface.pPanel, PaintTraverse::hkPaintTraverse, PaintTraverse::iIndex, (PVOID*)&PaintTraverse::oPaintTraverse));

	g_HookManager.bHooksAdded = true;
	return true;
}

bool HookManager::InitAllHooks() {
	// we have not yet successfully added all hooks
	if (!g_HookManager.bHooksAdded)
		return false;

	bool bStatus = g_HookLib.EnableAll();

	// forward original func pointer to different classes after hooks have been enabled
	g_Chams.c_oDrawModel = DrawModel::oDrawModel;

	g_HookLib.RestoreACHooks();

	// restore hooks placed by vac
	return bStatus;
}

bool HookManager::ReleaseAll() {
	// do some checks for tramp hooks, we need to patch original bytes
	
	// disable all hooks
	g_HookLib.DisableAll();

	return true;
}

#pragma region HkFunctions
void __stdcall PaintTraverse::hkPaintTraverse(unsigned int iPanel, bool bForceRepaint, bool bAllowForce) {
	// render prev. model
	const char* szPanelToDraw = g_Interface.pPanel->GetPanelName(iPanel);

	if (strstr(szPanelToDraw, XOR("MatSystemTopPanel"))) {
		g_PrevModel.Instance();
	}

	oPaintTraverse(g_Interface.pPanel, iPanel, bForceRepaint, bAllowForce);
}

bool __fastcall IsPaused::hkIsPaused(void* thisptr) {
	static DWORD* return_to_extrapolation = (DWORD*)(g_Tools.SignatureScan("client.dll",
		XOR("\xFF\xD0\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xD9\x1D\x00\x00\x00\x00\xFF\x50\x34\x85\xC0\x74\x22\x8B\x0D\x00\x00\x00\x00"), XOR("xxx????x????xx????xxxxxxxxx????")) + 0x29);

	if (_ReturnAddress() == (void*)return_to_extrapolation)
		return true;

	return oIsPaused(thisptr);
}

bool __fastcall IsHLTV::hkIsHLTV(void* thisptr, void* edx) {
	if (!Game::g_pLocal)
		return oIsHLTV(thisptr, edx);

	static const auto velocity = g_Tools.SignatureScan(XOR("client.dll"), XOR("\x84\xC0\x75\x38\x8B\x0D\x00\x00\x00\x00\x8B\x01\x8B\x80"), XOR("xxxxxx????xxxx"));
	static const auto layers = g_Tools.SignatureScan(XOR("client.dll"), XOR("\x84\xC0\x75\x0D\xF6\x87"), XOR("xxxxxx"));
	
	if (_ReturnAddress() == (uint32_t*)(layers))
		return true;

	if (_ReturnAddress() == (uint32_t*)(velocity))
		return true;

	return oIsHLTV(thisptr, edx);
}

bool __fastcall SvCheats::hkSvCheats(void* ConVar, int edx) {
	static auto pCamThink = reinterpret_cast<void*>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x85\xC0\x75\x30\x38\x86"), XOR("xxxxxx")));

	if (!ConVar)
		return false;

	if ((_ReturnAddress()) == pCamThink)
		return true;

	return oSvCheats(ConVar);
}

void __fastcall DoPostScreenSpaceEffects::hkDoPostScreenSpaceEffects(IClientMode* icmptr, int edx, const void* pViewSetup) {
	if (!Game::g_pLocal)
		return oDoPostScreenSpaceEffects(icmptr, edx, pViewSetup);

	cDoPostScreenSpaceEffects();
	oDoPostScreenSpaceEffects(icmptr, edx, pViewSetup);
}

void __stdcall FSN::hkFrameStageNotfy(IBaseClientDLL::ClientFrameStage_t curStage) {
	if (!Game::g_pLocal)
		return oFrameStageNotify(g_Interface.pClient, curStage);

	cFrameStageNotify(curStage);
	oFrameStageNotify(g_Interface.pClient, curStage);
}

void __stdcall HudUpdate::hkHudUpdate(bool bActive) {
	if (!Game::g_pLocal)
		return oHudUpdate(bActive);

	// do worldtoscreen here (only once per frame)
	cHudUpdate();
	oHudUpdate(bActive);
}

void __fastcall DrawModel::hkDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) {
	if (!Game::g_pLocal)
		return oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

	// glow model
	if (g_Interface.pStudioRender->IsForcedMaterialOverride())
		return oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

	cDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	g_Interface.pStudioRender->ForcedMaterialOverride(nullptr);
}	
bool __stdcall CreateMove::hkCreateMove(float flInputSampleTime, CUserCmd* cmd) {	
	Game::g_pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());
	if (!g_Interface.pEngine->IsInGame()) {
		g_PlayerList.listentries.clear();
		g_Misc.pSpectators.clear();
	}

	if (!cmd || !cmd->command_number)
		return false;

	if (!Game::g_pLocal)
		return oCreateMove(flInputSampleTime, cmd);

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