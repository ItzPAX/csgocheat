#pragma once
#include "interface.h"
#include "pch.h"
#include "includes.h"

Interface g_Interface{ };

void* Interface::GetInterface(const char* dllname, const char* interfacename) {
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

	int iReturncode = 0;
	void* pInterface = CreateInterface(interfacename, &iReturncode);

	return pInterface;
}

bool Interface::Init() {
	// grab all interfaces here
	pClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
	pClient = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
	pEngine = (IVEngineClient*)GetInterface("engine.dll", "VEngineClient014");
	pDebugOverlay = (IVDebugOverlay*)GetInterface("engine.dll", "VDebugOverlay004");
	return true;
}