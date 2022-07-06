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

	char** CNamesArr = new char* [listentries.size()];
	for (size_t i = 0; i < listentries.size(); i++) {
		CNamesArr[i] = new char[listentries[i].displayname.size() + 1];
		strcpy(CNamesArr[i], listentries[i].displayname.c_str());
	}

	ImGui::BeginChild(XOR("Playerlist"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	ImGui::PushItemWidth(-1);
	int height = (vSize.y - style->WindowPadding.y - style->FramePadding.y - 73) / ImGui::GetTextLineHeightWithSpacing();

	ImGui::ListBox(XOR(""), &iselplayer, CNamesArr, listentries.size(), height);

	for (size_t i = 0; i < listentries.size(); i++) {
		delete[] CNamesArr[i];
	}
	delete[] CNamesArr;
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild(XOR("Options"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	// only draw options if sel entity is valid
	if (iselplayer >= 0 && iselplayer < listentries.size()) {
		ImGui::Checkbox(XOR("Priority"), &settings[listentries[iselplayer].pPlayer->iIndex()].bPriority);
		ImGui::PushItemWidth(-1);
		static std::string info = "";
		if (ImGui::Button(XOR("Dump Player Info"))) {
			info.clear();
			info += XOR("Steam64: ") + std::to_string(listentries[iselplayer].playerInfo.steamid64) + '\n';
			info += XOR("xuidLow: ") + std::to_string(listentries[iselplayer].playerInfo.xuidlow) + '\n';
			info += XOR("xuidHigh: ") + std::to_string(listentries[iselplayer].playerInfo.xuidhigh) + '\n';
			info += XOR("name: "); info.append(listentries[iselplayer].playerInfo.name); info += '\n';
			info += XOR("uid: ") + std::to_string(listentries[iselplayer].playerInfo.userid) + '\n';
			info += XOR("Steam: "); info.append(listentries[iselplayer].playerInfo.steamid); info += '\n';
			info += XOR("SteamID: ") + std::to_string(listentries[iselplayer].playerInfo.isteamid) + '\n';
			info += XOR("friendsname: "); info.append(listentries[iselplayer].playerInfo.friendsname); info += '\n';
			info += XOR("fakeplayer: ") + std::to_string(listentries[iselplayer].playerInfo.fakeplayer) + '\n';
			info += XOR("ishltv: ") + std::to_string(listentries[iselplayer].playerInfo.ishltv) + '\n';
			info += XOR("customfiles: ") + std::to_string(listentries[iselplayer].playerInfo.customfiles[0]) + " " + std::to_string(listentries[iselplayer].playerInfo.customfiles[1]) + " " + std::to_string(listentries[iselplayer].playerInfo.customfiles[2]) + " " + std::to_string(listentries[iselplayer].playerInfo.customfiles[3]) + '\n';
			info += XOR("filesdownloaded: ") + std::to_string(listentries[iselplayer].playerInfo.filesdownloaded) + '\n';
		}
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
		plistEntry.bEnemy = pPlayer->bIsEnemy(Game::g_pLocal);
		plistEntry.displayname = plistEntry.bEnemy ? XOR("[ENEMY] ") : XOR("[TEAM] ");
		plistEntry.displayname += plistEntry.playerInfo.name;
		plistEntry.pPlayer = pPlayer;

		listentries.push_back(plistEntry);
	}
}