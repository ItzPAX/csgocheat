#pragma once

//this class will act as an interface for the g_HookLib class
class HookManager {
private:
	bool bHooksAdded;
	
public:
	HookManager() { bHooksAdded = false; } // Constructor
	bool AddAllHooks(bool bPrint = false);
	bool InitAllHooks(bool bPrint = false);
	bool ReleaseAll();
	HookStatus GetHookInfo(const char* sName);
	void PrintHookStatus(HookStatus hs);
};

extern HookManager g_HookManager;