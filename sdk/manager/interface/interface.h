#pragma once
#include "classes/IClientEntityList.h"
#include "classes/IBaseClientDLL.h"

class Interface {
private:
	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returncode);

public:
	IClientEntityList* pClientEntityList;
	IBaseClientDLL* pBaseClientDll;

public:
	void* GetInterface(const char* dllname, const char* interfacename);
	bool Init(); // init all interfaces
};

extern Interface g_Interface;