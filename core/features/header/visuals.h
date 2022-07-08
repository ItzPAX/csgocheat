#pragma once

struct PlayerDist {
	float flDist;
	Player* pPlayer;
};

class Visuals {
private:
	inline static float flEntOpacity[MAX_PLAYERS] = { 0.f };
	inline static std::vector<RECT> rPlayerRects;
	inline static std::vector<PlayerDist> pSortedPlayers;

public:
	// local stuff
	void ThirdPerson();

	void UpdatePlayerRects();
	void SortPlayers();
	void DrawWatermark();
	void GoofyAhhCrosshair();
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