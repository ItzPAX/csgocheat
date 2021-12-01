#pragma once

class Visuals {
private:
	inline static float flEntOpacity[MAX_PLAYERS] { 0 };
	inline static RECT rPlayerRects[MAX_PLAYERS] { 0 };

public:
	void UpdatePlayerRects();
	void DrawBox(RECT rPlayerRect, Color col);
	void DrawName(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info);
	void DrawHealth(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info);

	// Main Funcs
	void DrawDormant(Player* pPlayer, RECT rPlayerRect);
	void DrawPlayer(Player* pPlayer, RECT rPlayerRect);

	// these functions are called in the hooks
public:
	void OnEndScene();
};

extern Visuals g_Visuals;