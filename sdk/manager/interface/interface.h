#pragma once
#include "classes/IClientEntityList.h"
#include "classes/IBaseClientDLL.h"
#include "classes/IVEngineClient.h"
#include "classes/DebugOverlay.h"
#include "classes/IPanel.h"
#include "classes/ISurface.h"

class Interface {
private:
	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returncode);

public:
	IClientEntityList* pClientEntityList;
	IBaseClientDLL* pClient;
	IVEngineClient* pEngine;
	IVDebugOverlay* pDebugOverlay;
	IPanel* pPanel;
	ISurface* pSurface;

public:
	void* GetInterface(const char* dllname, const char* interfacename);
	bool Init(); // init all interfaces
};

extern Interface g_Interface;