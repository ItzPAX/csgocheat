#pragma once
#include "interface.h"
#include "pch.h"
#include "includes.h"

Interface g_Interface{ };

void* Interface::GetInterface(const char* dllname, const char* interfacename, bool bruteforce) {
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), XOR("CreateInterface"));

	int iReturnCode;
	void* pInterface = nullptr;

	// Bruteforce that mf
	if (bruteforce) {
		int iTries = 0;
		while (iTries <= 999) {
			std::string temp = interfacename;
			std::string toadd = "";

			if (iTries < 10) {
				toadd = XOR("00");
				toadd += std::to_string(iTries);
			}
			else if (iTries < 100) {
				toadd = XOR("0");
				toadd += std::to_string(iTries);
			}
			else {
				toadd = XOR("");
				toadd += std::to_string(iTries);
			}

			temp += toadd;
			pInterface = CreateInterface(temp.c_str(), &iReturnCode);
			iTries++;

			if (pInterface && iReturnCode == 0)
				break;
		}
	}
	else 
		pInterface = CreateInterface(interfacename, &iReturnCode);
	return pInterface;
}

bool Interface::Init() {
	// grab all interfaces here
	pClientEntityList = (IClientEntityList*)GetInterface(XOR("client.dll"), XOR("VClientEntityList"));
	pClient = (IBaseClientDLL*)GetInterface(XOR("client.dll"), XOR("VClient"));
	pEngine = (IVEngineClient*)GetInterface(XOR("engine.dll"), XOR("VEngineClient"));
	pDebugOverlay = (IVDebugOverlay*)GetInterface(XOR("engine.dll"), XOR("VDebugOverlay"));
	pInputSystem = (IInputSystem*)GetInterface(XOR("inputsystem.dll"), XOR("InputSystemVersion"));
	pConsole = (IGameConsole*)GetInterface(XOR("client.dll"), XOR("GameConsole"));
	pModelRender = (IVModelRender*)GetInterface(XOR("engine.dll"), XOR("VEngineModel"));
	pMaterialSystem = (IMaterialSystem*)GetInterface(XOR("materialsystem.dll"), XOR("VMaterialSystem"));
	pStudioRender = (IStudioRender*)GetInterface(XOR("studiorender.dll"), XOR("VStudioRender"));
	pRenderView = (IRenderView*)GetInterface(XOR("engine.dll"), XOR("VEngineRenderView"));
	pSurface = (ISurface*)GetInterface(XOR("vguimatsurface.dll"), XOR("VGUI_Surface"));
	pPanel = (IPanel*)GetInterface(XOR("vgui2.dll"), XOR("VGUI_Panel"));
	pModelInfo = (IVModelInfo*)GetInterface(XOR("engine.dll"), XOR("VModelInfoClient"));
	pEngineTrace = (IEngineTrace*)GetInterface(XOR("engine.dll"), XOR("EngineTraceClient"));
	pICVar = (ICvar*)GetInterface(XOR("vstdlib.dll"), XOR("VEngineCvar"));
	pSurfaceProps = (IPhysicsSurfaceProps*)GetInterface(XOR("Vphysics.dll"), XOR("VPhysicsSurfaceProps"));
	pICVarQuery = (ICvarQuery*)GetInterface(XOR("vstdlib.dll"), XOR("VCvarQuery"));
	pClientLeafSystem = (IClientLeafSystem*)GetInterface(XOR("client.dll"), XOR("ClientLeafSystem"));
	pPrediction = (PlayerPrediction*)GetInterface(XOR("client.dll"), XOR("VClientPrediction"));
	pGameMovement = (PlayerGameMovement*)GetInterface(XOR("client.dll"), XOR("GameMovement"));
	pMDLCache = (IMDLCache*)GetInterface(XOR("datacache.dll"), XOR("MDLCache"));

	// custom interfaces
	pGlobalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(pClient))[11] + 10);

	// sig interfaces 
	pGlow = *reinterpret_cast<CGlowObjectManager**>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x0F\x11\x05\x00\x00\x00\x00\x83\xC8\x01"), XOR("xxx????xxx")) + 0x03);
	pClientMode = **reinterpret_cast<IClientMode***>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x8B\x0D\x00\x00\x00\x00\x8B\x01\x5D\xFF\x60\x30"), XOR("xx????xxxxxx")) + 0x02);
	pInput = *reinterpret_cast<IInput**>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\xB9\x00\x00\x00\x00\xF3\x0F\x11\x04\x24\xFF\x50\x10"), XOR("x????xxxxxxxx")) + 0x01);
	pWeaponSystem = *reinterpret_cast<IWeaponSystem**>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x8B\x35\x00\x00\x00\x00\xFF\x10\x0F\xB7\xC0"), XOR("xx????xxxxx")) + 0x02);
	pMoveHelper = **reinterpret_cast<PlayerMoveHelper***>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x68"), XOR("xx????xxxx")) + 0x02);
	pClientState = **reinterpret_cast<IClientState***>(g_Tools.SignatureScan(XOR("engine.dll"), XOR("\xA1\x00\x00\x00\x00\x8B\x88\x00\x00\x00\x00\x85\xC9\x75\x07"), XOR("x????xx????xxxx")) + 0x01);

	return true;
}