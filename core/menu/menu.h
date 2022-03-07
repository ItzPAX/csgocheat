#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

class Menu {
public:
	inline static bool bInitialized = false;
	inline static bool bToggled = false;

	inline static int iCurrentTab = 0;

	inline static ImGuiStyle* style;

	void Render();
	void Draw();
	void Init();
};

extern Menu g_Menu;