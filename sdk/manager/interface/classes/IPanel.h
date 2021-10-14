#pragma once

class IPanel {
public:
	void SetKeyboardInputEnabled(unsigned int panel_id, bool state) {
		using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
		return (*(original_fn**)this)[31](this, panel_id, state);
	}

	void SetMouseInputEnabled(unsigned int panel_id, bool state) {
		using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
		return (*(original_fn**)this)[32](this, panel_id, state);
	}

	const char* GetPanelName(unsigned int panel_id) {
		using original_fn = const char* (__thiscall*)(IPanel*, unsigned int);
		return (*(original_fn**)this)[36](this, panel_id);
	}
};