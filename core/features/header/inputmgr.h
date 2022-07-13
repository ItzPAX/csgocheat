#pragma once
#include "pch.h"
#include "includes.h"

struct HotkeylistEntry {
public:
	int key;
	std::string name, keyname, mode;
public:
	bool active;
	std::string displayname;
};

class InputMgr {
private:
	std::unordered_map<std::string, bool> togglestate;

public:
	std::unordered_map<std::string, HotkeylistEntry> hotkeysinlist;

public:
	// NAME NEEDS TO BE UNIQUE FOR EVERY HOTKEY
	bool GetHotkeyState(int* hotkey, const char* name);
};

extern InputMgr g_InputMgr;