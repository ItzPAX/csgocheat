#include "pch.h"
#include "includes.h"

PlayerList g_PlayerList;

void PlayerList::DrawPlayerList() {
	if (!bListOpened)
		return;

	style = &ImGui::GetStyle();
	if (!style)
		return;

	ImGui::SetNextWindowSize(WindowSize, ImGuiCond_Once);
	ImGui::Begin(XOR("Playerlist"), &bListOpened);
	ImVec2 vSize = ImGui::GetWindowSize();

	std::vector<char*> cstring;

	for (size_t i = 0; i < listentries.size(); i++) {
		cstring.push_back(const_cast<char*>(listentries[i].displayname.c_str()));
	}

	ImGui::BeginChild(XOR("Playerlist"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	ImGui::PushItemWidth(-1);
	int height = (vSize.y - style->WindowPadding.y - style->FramePadding.y - 73) / ImGui::GetTextLineHeightWithSpacing();

	if (!cstring.empty()) {
		ImGui::ListBox(XOR(""), &iselplayer, &cstring[0], cstring.size(), height);
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild(XOR("Options"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	// only draw options if sel entity is valid
	if (iselplayer >= 0 && iselplayer < listentries.size()) {
		if (!listentries[iselplayer].pPlayer)
			return;

		int playerind = listentries[iselplayer].pPlayer->iIndex();
		if (playerind < 0 || playerind > MAX_PLAYERS) // invalid player
			return;

		PlayerInfo pinfo;
		g_Interface.pEngine->GetPlayerInfo(playerind, &pinfo);

		ImGui::Checkbox(XOR("Prioritize Player"), &settings[pinfo.isteamid].bPrioritizePlayer);
		ImGui::Checkbox(XOR("Hightlight Player"), &settings[pinfo.isteamid].bHighlightPlayer);
		ImGui::Checkbox(XOR("Exclude from Paranoia Mode"), &settings[pinfo.isteamid].bIgnoreFromParanoia);
		//ImGui::Checkbox(XOR("Set as Clantag Target"), &settings[pinfo.isteamid].bTargetForClantag);

		// get playerinfo struct and dump it to a string
		static std::string info = "";
		info.clear();
		info += XOR("Info:\n");
		info += XOR("Steam64: ") + std::to_string(listentries[iselplayer].playerInfo.steamid64) + '\n';
		info += XOR("Name: "); info.append(listentries[iselplayer].playerInfo.name); info += '\n';
		info += XOR("PlayerID: ") + std::to_string(listentries[iselplayer].playerInfo.userid) + '\n';
		info += XOR("SteamStr: "); info.append(listentries[iselplayer].playerInfo.steamid); info += '\n';
		info += XOR("SteamID: ") + std::to_string(listentries[iselplayer].playerInfo.isteamid) + '\n';
		ImGui::Text(info.c_str(), va_list());
	}
	ImGui::EndChild();

	ImGui::End();
}

void PlayerList::UpdatePlayerList() {
	listentries.clear();
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		if (!pPlayer || !pPlayer->bIsPlayer())
			continue;

		PlayerListEntry plistEntry;
		g_Interface.pEngine->GetPlayerInfo(pPlayer->iIndex(), &plistEntry.playerInfo);
		if (pPlayer == Game::g_pLocal)
			localplayername = plistEntry.playerInfo.name;

		plistEntry.bEnemy = pPlayer->bIsEnemy(Game::g_pLocal);
		plistEntry.displayname = plistEntry.bEnemy ? XOR("[ENEMY] ") : XOR("[TEAM] ");
		plistEntry.displayname += plistEntry.playerInfo.name;
		plistEntry.pPlayer = pPlayer;

		listentries.push_back(plistEntry);
	}
}