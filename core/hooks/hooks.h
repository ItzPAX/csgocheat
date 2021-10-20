#pragma once

//hook status interface struct
struct IHookStatus {
	IHookStatus() { pBaseFnc = NULL; pHkAddr = nullptr; iIndex = NULL; name = ""; }
	uintptr_t pBaseFnc;
	PVOID pHkAddr;
	INT   iIndex;
	const char* name;
};

//this class is the interface for the g_HookLib class
class HookManager {
private:
	bool bHooksAdded;
	int  iCounter;

public:
	HookManager() { bHooksAdded = false; iCounter = 0; } // constructor
	bool AddAllHooks();
	bool InitAllHooks();
	bool ReleaseAll();

	// debug functions, only works if hook has been placed via VEH
	IHookStatus GetHookInfo(const char* sName);
	void LogHookStatus(IHookStatus ihs);
};

extern HookManager g_HookManager;