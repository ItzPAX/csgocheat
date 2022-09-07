#pragma once

struct PlayerListEntry {
	Player* pPlayer;
	PlayerInfo playerInfo;
	std::string displayname;
	bool bEnemy;
};

struct PlayerListSettings {
	bool bPrioritizePlayer = false;
	bool bHighlightPlayer = false;
	bool bIgnoreFromParanoia = false;
	bool bTargetForClantag = false;
};

class PlayerList {
private:
	ImVec2 WindowSize = { 700,500 };
	ImGuiStyle* style;
	std::string localplayername;
public:
	std::unordered_map<ulong, PlayerListSettings> settings;
	std::vector<PlayerListEntry> listentries;

	bool bListOpened = false;
	int iselplayer = 0;
	void DrawPlayerList();
	void UpdatePlayerList();
};

extern PlayerList g_PlayerList;