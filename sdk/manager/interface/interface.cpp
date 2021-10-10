#pragma once
#include "interface.h"
#include "pch.h"
#include "includes.h"

Interface g_Interface{ };

void* Interface::GetInterface(const char* dllname, const char* interfacename) {
	if (!GetModuleHandle(dllname))
		return NULL;

	FARPROC pProcAddr = GetProcAddress(GetModuleHandle(dllname), "CreateInterface");
	if (!pProcAddr)
		return NULL;


	tCreateInterface CreateInterface = (tCreateInterface)pProcAddr;

	int iReturncode = 0;
	void* pInterface = CreateInterface(interfacename, &iReturncode);

	if (!pInterface)
		return NULL;

	return pInterface;
}

bool Interface::Init() {
	// grab all interfaces here
	pClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
	pClient = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
	pEngine = (IVEngineClient013*)GetInterface("engine.dll", "VEngineClient014");
	return true;
}