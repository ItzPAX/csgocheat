#pragma once
#include "interface.h"
#include "pch.h"
#include "includes.h"

Interface g_Interface{ };

void* Interface::GetInterface(const char* dllname, const char* interfacename) {
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

	int iReturnCode;
	void* pInterface = nullptr;

	// Bruteforce that mf
	int iTries = 0;
	while (iTries <= 999) {
		std::string temp = interfacename;
		std::string toadd = "";
	
		if (iTries < 10) {
			toadd = "00";
			toadd += std::to_string(iTries);
		}
		else if (iTries < 100) {
			toadd = "0";
			toadd += std::to_string(iTries);
		}
		else {
			toadd = "";
			toadd += std::to_string(iTries);
		}
	
		temp += toadd;
		pInterface = CreateInterface(temp.c_str(), &iReturnCode);
		iTries++;

		if (pInterface && iReturnCode == 0)
			break;
	}
	return pInterface;
}

bool Interface::Init() {
	// grab all interfaces here
	pClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList");
	pClient = (IBaseClientDLL*)GetInterface("client.dll", "VClient");
	pEngine = (IVEngineClient*)GetInterface("engine.dll", "VEngineClient");
	pDebugOverlay = (IVDebugOverlay*)GetInterface("engine.dll", "VDebugOverlay");
	pInputSystem = (IInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion");
	pConsole = (IGameConsole*)GetInterface("client.dll", "GameConsole");
	pModelRender = (IVModelRender*)GetInterface("engine.dll", "VEngineModel");
	pMaterialSystem = (IMaterialSystem*)GetInterface("materialsystem.dll", "VMaterialSystem");
	pStudioRender = (IStudioRender*)GetInterface("studiorender.dll", "VStudioRender");
	pRenderView = (IRenderView*)GetInterface("engine.dll", "VEngineRenderView");
	pSurface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface");
	

	// custom interfaces
	pGlobalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(pClient))[11] + 10);

	// sig interfaces 
	pClientMode = **reinterpret_cast<IClientMode***>(g_Tools.SignatureScan("client.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\x01\x5D\xFF\x60\x30", "xx????xxxxxx") + 0x02);

	return true;
}