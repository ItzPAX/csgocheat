#pragma once

struct PlayerListEntry {
	Player* pPlayer;
	PlayerInfo playerInfo;
	std::string displayname;
	bool bEnemy;
};

struct PlayerListSettings {
	bool bPriority = false;
};

class PlayerList {
private:
	ImVec2 WindowSize = { 700,500 };
	ImGuiStyle* style;
	std::vector<PlayerListEntry> listentries;
public:
	PlayerListSettings settings[MAX_PLAYERS];

	bool bListOpened = false;
	int iselplayer = 0;
	void DrawPlayerList();
	void UpdatePlayerList();
};

extern PlayerList g_PlayerList;