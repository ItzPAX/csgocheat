#pragma once

class Visuals {
private:
	inline static float flEntOpacity[MAX_PLAYERS] { 0 };

public:
	RECT GetPlayerRect(Player* pPlayer);
	void DrawBox(RECT rPlayerRect, Color col);
	void DrawName(RECT rPlayerRect, Player* pPlayer, Color col);
	void DrawHealth(RECT rPlayerRect, Player* pPlayer, Color col);

	// Main Funcs
	void DrawDormant(Player* pPlayer, RECT rPlayerRect);
	void DrawPlayer(Player* pPlayer, RECT rPlayerRect);

	// these functions are called in the hooks
public:
	void OnEndScene();
};

extern Visuals g_Visuals;