#pragma once

//hook status interface struct
struct IHookStatus {
	IHookStatus() { pBaseFnc = NULL; pHkAddr = nullptr; iIndex = NULL; name; }
	uintptr_t pBaseFnc;
	PVOID pHkAddr;
	INT   iIndex;
	std::string *name;
};

//this class is the interface for the g_HookLib class
class HookManager {
private:
	bool bHooksAdded;

public:
	HookManager() { bHooksAdded = false; } // constructor
	bool AddAllHooks();
	bool InitAllHooks();
	bool ReleaseAll();
};

extern HookManager g_HookManager;