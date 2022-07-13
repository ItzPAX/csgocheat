#include "../header/inputmgr.h"

InputMgr g_InputMgr;

bool InputMgr::GetHotkeyState(int* hotkey, const char* name) {
	switch (hotkey[ImGuiHotkeyInd_MODE]) {
	case ImGuiHotkeyMode_KEYDOWN: {
		HotkeylistEntry entry;
		entry.key = hotkey[ImGuiHotkeyInd_KEY];
		entry.name = name;
		entry.keyname = KeyNames[hotkey[ImGuiHotkeyInd_KEY]];
		entry.mode = XOR("[KEYDOWN]");
		entry.displayname = entry.name + " " + entry.keyname + " " + entry.mode;
		entry.active = false;

		// set state to disabled if no button is set, so its always active but we dont display it as a hotkey
		if (hotkey[ImGuiHotkeyInd_KEY] == 0) {
			hotkeysinlist[name] = entry;
			return true;
		}

		if (GetAsyncKeyState(hotkey[ImGuiHotkeyInd_KEY]) < 0) {
			if (hotkey[ImGuiHotkeyInd_INLIST] && hotkey[ImGuiHotkeyInd_KEY] != 0) {
				entry.active = true;
				hotkeysinlist[name] = entry;
			}
			return true;
		}
		entry.active = false;
		hotkeysinlist[name] = entry;
	}
		break;

	case ImGuiHotkeyMode_KEYUP: {
		HotkeylistEntry entry;
		entry.key = hotkey[ImGuiHotkeyInd_KEY];
		entry.name = name;
		entry.keyname = KeyNames[hotkey[ImGuiHotkeyInd_KEY]];
		entry.mode = XOR("[KEYUP]");
		entry.displayname = entry.name + " " + entry.keyname + " " + entry.mode;
		entry.active = false;

		// set state to disabled if no button is set, so its always active but we dont display it as a hotkey
		if (hotkey[ImGuiHotkeyInd_KEY] == 0) {
			hotkeysinlist[name] = entry;
			return true;
		}

		if (GetAsyncKeyState(hotkey[ImGuiHotkeyInd_KEY]) == 0) {
			if (hotkey[ImGuiHotkeyInd_INLIST] && hotkey[ImGuiHotkeyInd_KEY] != 0) {
				entry.active = true;
				hotkeysinlist[name] = entry;
			}
			return true;
		}
		entry.active = false;
		hotkeysinlist[name] = entry;
	}
		break;

	case ImGuiHotkeyMode_TOGGLE: {
		if ((GetAsyncKeyState(hotkey[ImGuiHotkeyInd_KEY]) & 0x01) == 1)
			togglestate[name] = !togglestate[name];

		HotkeylistEntry entry;
		entry.key = hotkey[ImGuiHotkeyInd_KEY];
		entry.name = name;
		entry.keyname = KeyNames[hotkey[ImGuiHotkeyInd_KEY]];
		entry.mode = XOR("[TOGGLE]");
		entry.displayname = entry.name + " " + entry.keyname + " " + entry.mode;
		entry.active = false;

		// set state to disabled if no button is set, so its always active but we dont display it as a hotkey
		if (hotkey[ImGuiHotkeyInd_KEY] == 0) {
			hotkeysinlist[name] = entry;
			return true;
		}

		if (hotkey[ImGuiHotkeyInd_INLIST]) {
			entry.active = togglestate[name];
			hotkeysinlist[name] = entry;
		}

		return togglestate[name];
	}
		break;
	default:
		return false;
		break;
	}

	return false;
}