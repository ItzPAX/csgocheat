#pragma once
#include "classes/IClientEntityList.h"
#include "classes/IBaseClientDLL.h"
#include "classes/IVEngineClient.h"
#include "classes/DebugOverlay.h"
#include "classes/IClientRenderable.h"
#include "classes/CGlobalVars.h"
#include "classes/IClientMode.h"
#include "classes/IInputSystem.h"
#include "classes/IPanel.h"
#include "classes/IGameConsole.h"
#include "classes/IVModelRender.h"
#include "classes/IMaterialSystem.h"
#include "classes/IStudioRender.h"
#include "classes/IRenderView.h"
#include "classes/ISurface.h"

class Interface {
private:
	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returncode);

public:
	IClientEntityList* pClientEntityList;
	IBaseClientDLL* pClient;
	IVEngineClient* pEngine;
	IVDebugOverlay* pDebugOverlay;
	CGlobalVars* pGlobalVars;
	IClientMode* pClientMode;
	IInputSystem* pInputSystem;
	IPanel* pPanel;
	IGameConsole* pConsole;
	IVModelRender* pModelRender;
	IMaterialSystem* pMaterialSystem;
	IStudioRender* pStudioRender;
	IRenderView* pRenderView;
	ISurface* pSurface;
	

	// TODO: Find Interface entry for this
	IClientRenderable* pRenderable;

public:
	void* GetInterface(const char* dllname, const char* interfacename);
	bool Init(); // init all interfaces
};

extern Interface g_Interface;