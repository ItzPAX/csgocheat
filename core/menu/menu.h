#pragma once

class Menu {
public:
	inline static bool bInitialized = false;
	inline static bool bToggled = false;

	void Render();
	void Draw();
	void Init();
};

extern Menu g_Menu;