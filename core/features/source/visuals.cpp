#include "pch.h"
#include "includes.h"

Visuals g_Visuals;

void Visuals::ThirdPerson() {
	g_Interface.pICVar->FindVar(XOR("cam_collision"))->SetValue(true);

	if (!g_Config.ints[XOR("thirdperson")].val) {
		g_Interface.pInput->CAM_ToFirstPerson();
		return;
	}

	if (g_InputMgr.GetHotkeyState(g_Config.arrints[XOR("thirdpersonkey")].val, XOR("Thirdperson"))) {
		g_Interface.pInput->CAM_ToThirdPerson();
		bInThirdperson = true;
	}
	else {
		g_Interface.pInput->CAM_ToFirstPerson();
		bInThirdperson = false;
	}

	g_Interface.pInput->m_vCameraOffset.z = 100.f;
}

void Visuals::UpdatePlayerRects() {
	g_Visuals.rPlayerRects.clear();

	for (int i = 0; i < g_Visuals.pSortedPlayers.size(); i++) {
		// get and validate player
		Player* pPlayer = pSortedPlayers[i].pPlayer;

		// get pos
		Vec3D vPlayerPosTop = pPlayer->vGetBonePosition(8); // bone id 8 is the head
		Vec3D vPlayerPosBottom = pPlayer->vOrigin();

		Vec3D vScreenBottom, vScreenTop;

		g_Interface.pDebugOverlay->WorldToScreen(vPlayerPosTop, vScreenTop);
		g_Interface.pDebugOverlay->WorldToScreen(vPlayerPosBottom, vScreenBottom);

		int iMiddle = vScreenBottom.y - vScreenTop.y;
		int iWidth = iMiddle / 4;

		RECT rPlayerRect;
		rPlayerRect.left = vScreenBottom.x - iWidth;
		rPlayerRect.right = vScreenBottom.x + iWidth;
		rPlayerRect.top = vScreenTop.y;
		rPlayerRect.bottom = vScreenTop.y + iMiddle;

		rPlayerRect.top -= iWidth / 2;

		rPlayerRects.push_back(rPlayerRect);
	}
}

void Visuals::SortPlayers() {
	g_Visuals.pSortedPlayers.clear();

	if (!Game::g_pLocal || Game::g_pLocal == (Player*)0xFFFFFFFF)
		return;

	Player* specplayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntityFromHandle(Game::g_pLocal->hObserverTarget()));

	static auto SortDistance = [](PlayerDist a, PlayerDist b) {
		return a.flDist > b.flDist;
	};

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get and validate player
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || pPlayer == (Player*)0xFFFFFFFF || pPlayer == specplayer || pPlayer == Game::g_pLocal || !pPlayer->bIsAlive() || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		float flDist = pPlayer->vAbsOrigin().DistanceTo(Game::g_pLocal->vAbsOrigin());
		
		pSortedPlayers.push_back(PlayerDist(flDist, pPlayer));
	}

	std::sort(pSortedPlayers.begin(), pSortedPlayers.end(), SortDistance);
}

void Visuals::DrawWatermark() {
	if (!g_Config.ints[XOR("watermark")].val)
		return;

	int ping = 0;
	auto nci = g_Interface.pEngine->GetNetChannelInfo();
	if (nci)
		ping = g_Interface.pEngine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING) * 1000;
	int fps = 1 / g_Interface.pGlobalVars->flAbsFrametime;

	std::string watermark = XOR("RayBot by ItzPAX | Ping: ");
	watermark += std::to_string(ping);
	watermark += XOR(" | FPS: ");

	std::string copy(watermark);
	copy += XOR("100");

	watermark += std::to_string(fps);

	Vec2D size = g_Render.TextSize(g_Render.pEspFont, copy.c_str());
	g_Render.DrawFilledRect(g_DirectX.iWindowWidth - size.x - 15, 6, size.x + 10, size.y + 5, Color(50.f, 50.f, 50.f, 200.f));
	g_Render.Text(g_Render.pEspFont, watermark.c_str(), g_DirectX.iWindowWidth - size.x - 10, 8, Color::White(), DT_LEFT);
}

void Visuals::GoofyAhhCrosshair() {
	if (!g_Config.ints[XOR("goofyahhcrosshair")].val)
		return;

	static ConVar* pCrosshairSize = g_Interface.pICVar->FindVar(XOR("cl_crosshairsize"));
	static ConVar* pCrosshairDot = g_Interface.pICVar->FindVar(XOR("cl_crosshairdot"));
	static ConVar* pCrosshairThickness = g_Interface.pICVar->FindVar(XOR("cl_crosshairthickness"));
	static ConVar* pCrosshairColor_r = g_Interface.pICVar->FindVar(XOR("cl_crosshaircolor_r"));
	static ConVar* pCrosshairColor_g = g_Interface.pICVar->FindVar(XOR("cl_crosshaircolor_g"));
	static ConVar* pCrosshairColor_b = g_Interface.pICVar->FindVar(XOR("cl_crosshaircolor_b"));
	static ConVar* pCrosshairAlpha = g_Interface.pICVar->FindVar(XOR("cl_crosshairalpha"));


	if (!g_Interface.pEngine->IsInGame()) {
	// goofy the crosshair
		pCrosshairSize->SetValue(300);
		pCrosshairDot->SetValue(1);
		pCrosshairThickness->SetValue(900);
		pCrosshairColor_r->SetValue(250);
		pCrosshairColor_g->SetValue(250);
		pCrosshairColor_b->SetValue(250);
		pCrosshairAlpha->SetValue(255);
	}
	else {
	// ungoofy the crosshair
		pCrosshairSize->SetValue(1);
		pCrosshairDot->SetValue(0);
		pCrosshairThickness->SetValue(0.7f);
		pCrosshairColor_r->SetValue(50);
		pCrosshairColor_g->SetValue(250);
		pCrosshairColor_b->SetValue(50);
		pCrosshairAlpha->SetValue(200);
	}
}

void Visuals::DrawBox(RECT rPlayerRect, Color col) {
	g_Render.DrawOutlinedRect(rPlayerRect.left, rPlayerRect.top, (rPlayerRect.right - rPlayerRect.left), (rPlayerRect.bottom - rPlayerRect.top), col);

	// box outline
	g_Render.DrawOutlinedRect(rPlayerRect.left + 1, rPlayerRect.top + 1, (rPlayerRect.right - rPlayerRect.left) - 2, (rPlayerRect.bottom - rPlayerRect.top) - 2, Color::Black(col.rgba[3]));
	g_Render.DrawOutlinedRect(rPlayerRect.left - 1, rPlayerRect.top - 1, (rPlayerRect.right - rPlayerRect.left) + 2, (rPlayerRect.bottom - rPlayerRect.top) + 2, Color::Black(col.rgba[3]));
}

void Visuals::DrawName(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info) {
	std::string name{ std::string(info.name).substr(0, 24) };
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, name.c_str());

	// box
	g_Render.DrawFilledRect((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 1.6), (rPlayerRect.top - vSize.y * 1.5) - 1, vSize.x * 1.3f, vSize.y * 1.25f, Color(30, 30, 30, col.rgba[3]));
	g_Render.DrawLine((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 1.6), (rPlayerRect.top - 4), (rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) + (vSize.x / 1.5), (rPlayerRect.top - 4), 1, Color(130,20,60, col.rgba[3]));

	// text
	g_Render.Text(g_Render.pEspFont, name.c_str(), rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2), (rPlayerRect.top - vSize.y * 1.5f) - 1, col);
}

void Visuals::DrawHealth(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info) {
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, info.name);
	float flScaledNum = g_Math.ScaleNumber(pPlayer->iHealth(), 100.f, 0.f, rPlayerRect.top, rPlayerRect.bottom);

	std::string str = std::to_string(pPlayer->iHealth());
	str += XOR("hp");
	Vec2D vSizeHP = g_Render.TextSize(g_Render.pEspFont, str.c_str());

	g_Render.DrawLine(rPlayerRect.left - 4, rPlayerRect.bottom, rPlayerRect.left - 4, rPlayerRect.top, 1, Color(50,50,50,col.rgba[3]));
	g_Render.DrawLine(rPlayerRect.left - 4, rPlayerRect.bottom, rPlayerRect.left - 4, flScaledNum, 1, col);

	g_Render.Text(g_Render.pEspFont, str.c_str(), rPlayerRect.left - (vSizeHP.x / 2.f) - 5, flScaledNum - (vSizeHP.y / 2), Color::White(col.rgba[3]));
}

void Visuals::DrawWeapon(RECT rPlayerRect, Player* pPlayer, Color col) {
	if (!Game::g_pLocal->bIsAlive())
		return;

	Entity* weapon = pPlayer->pGetActiveWeapon();
	if (!weapon)
		return;

	CCSWeaponData* data = weapon->GetWeaponData();
	if (!data)
		return;

	std::stringstream name(data->szWeaponName);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(name, segment, '_'))
	{
		seglist.push_back(segment);
	}
	if (seglist.size() < 2)
		return;

	Vec2D size = g_Render.TextSize(g_Render.pEspFont, seglist[1].c_str());
	g_Render.Text(g_Render.pEspFont, seglist[1].c_str(), rPlayerRect.right + 4.f + size.x / 2, rPlayerRect.top, Color::White(col.rgba[3]));
}

void Visuals::DrawDormant(Player* pPlayer, RECT rPlayerRect) {
	// opacity should reach 1 in 3000 milliseconds.
	constexpr float flFrequency = 1.f / 3.f;
	float flStep = flFrequency * g_Interface.pGlobalVars->flFrameTime;

	// opacity itself
	flEntOpacity[pPlayer->iIndex()] -= flStep;

	g_Math.Clamp(&flEntOpacity[pPlayer->iIndex()], 1.f, 0.f);
	if (flEntOpacity[pPlayer->iIndex()] == 0.f)
		return;

	PlayerInfo playerInfo;
	g_Interface.pEngine->GetPlayerInfo(pPlayer->iIndex(), &playerInfo);

	

	float flpAlpha = 0xFF * flEntOpacity[pPlayer->iIndex()];

	if (g_Config.ints[XOR("boxesp")].val) DrawBox(rPlayerRect, Color(120, 120, 120, flpAlpha));
	if (g_Config.ints[XOR("nameesp")].val) DrawName(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha), playerInfo);
	if (g_Config.ints[XOR("healthesp")].val) DrawHealth(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha), playerInfo);
	if (g_Config.ints[XOR("weaponesp")].val) DrawWeapon(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha));
}

void Visuals::DrawPlayer(Player* pPlayer, RECT rPlayerRect) {
	// opacity should reach 1 in 500 milliseconds.
	constexpr float flFrequency = 1.f / 0.5f;
	float flStep = flFrequency * g_Interface.pGlobalVars->flFrameTime;

	// opacity itself
	flEntOpacity[pPlayer->iIndex()] += flStep;

	g_Math.Clamp(&flEntOpacity[pPlayer->iIndex()], 1.f, 0.f);
	if (flEntOpacity[pPlayer->iIndex()] == 0.f)
		return;

	int flAlpha = 0xFF * flEntOpacity[pPlayer->iIndex()];

	// construct our color
	Color cHealthCol(((100 - pPlayer->iHealth()) * 2.55f), (pPlayer->iHealth() * 2.55f), 0.f, flAlpha);

	PlayerInfo playerInfo;
	g_Interface.pEngine->GetPlayerInfo(pPlayer->iIndex(), &playerInfo);

	if (g_Config.ints[XOR("boxesp")].val) DrawBox(rPlayerRect, g_PlayerList.settings[playerInfo.isteamid].bHighlightPlayer ? Color::Red(flAlpha) : cHealthCol);
	if (g_Config.ints[XOR("nameesp")].val) DrawName(rPlayerRect, pPlayer, g_PlayerList.settings[playerInfo.isteamid].bHighlightPlayer ? Color::Red(flAlpha) : Color::White(flAlpha), playerInfo);
	if (g_Config.ints[XOR("healthesp")].val) DrawHealth(rPlayerRect, pPlayer, cHealthCol, playerInfo);
	if (g_Config.ints[XOR("weaponesp")].val) DrawWeapon(rPlayerRect, pPlayer, g_PlayerList.settings[playerInfo.isteamid].bHighlightPlayer ? Color::Red(flAlpha) : Color::White(flAlpha));
}

void Visuals::DrawSpectatorList() {
	if (!g_Config.ints[XOR("spectatorlist")].val)
		return;

	if (g_Misc.pSpectators.empty() && !g_Menu.bToggled)
		return;

	// find longest name
	int tmp = g_Misc.pSpectators.size();
	std::vector<char*> names;
	std::vector<char*> obsmodes;

	int i = 0;
	for (auto& entry : g_Misc.pSpectators) {
		names.push_back(const_cast<char*>(entry.name.c_str()));
		obsmodes.push_back(const_cast<char*>(entry.obsmode.c_str()));
		i++;
	}

	int size = g_Misc.pSpectators.size();
	size = std::clamp(size, 0, 7);

	std::string menuname = XOR("Spectatorlist [") + g_Misc.speclistname + XOR("][") + std::to_string(g_Misc.pSpectators.size()) + XOR("]###Spectatorlist");

	ImGui::SetNextWindowSize(ImVec2(300, size * ImGui::GetTextLineHeightWithSpacing() + 72));
	if (!g_Menu.bToggled) {
		ImGui::Begin(menuname.c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	}
	else {
		ImGui::Begin(menuname.c_str(), (bool*)&g_Config.ints[XOR("spectatorlist")].val, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	}

	ImVec2 vSize = ImGui::GetWindowSize();

	ImGui::BeginChild(XOR("Spectators"), ImVec2(vSize.x / 2 - 17, 0.f));
	ImGui::PushItemWidth(-1);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::ListBox(XOR("##specs"), &tmp, &names[0], g_Misc.pSpectators.size());
	ImGui::PopStyleColor(4);
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild(XOR("Observermode"), ImVec2(vSize.x / 2 - 17, 0.f));
	ImGui::PushItemWidth(-1);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::ListBox(XOR("##obsmode"), &tmp, &obsmodes[0], g_Misc.pSpectators.size());
	ImGui::PopStyleColor(4);
	ImGui::EndChild();

	ImGui::End();
}

void Visuals::DrawHotkeyList() {
	if (!g_Config.ints[XOR("hotkeylist")].val)
		return;

	// iterate every entity and check if we have an active state
	int iActiveEntries = 0;
	for (auto& entry : g_InputMgr.hotkeysinlist) {
		if (entry.second.active)
			iActiveEntries++;
	}

	if (iActiveEntries == 0 && !g_Menu.bToggled)
		return;

	int tmp = iActiveEntries;
	std::vector<char*> names;

	for (auto& entry : g_InputMgr.hotkeysinlist) {
		if (entry.second.active)
			names.push_back(const_cast<char*>(entry.second.displayname.c_str()));
	}
	int size = iActiveEntries;
	size = std::clamp(size, 0, 7);

	ImGui::SetNextWindowSize(ImVec2(275, size * ImGui::GetTextLineHeightWithSpacing() + 72));
	if (!g_Menu.bToggled) {
		ImGui::Begin(XOR("Hotkeylist"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	}
	else {
		ImGui::Begin(XOR("Hotkeylist"), (bool*)&g_Config.ints[XOR("hotkeylist")].val, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	}

	ImVec2 vSize = ImGui::GetWindowSize();

	ImGui::PushItemWidth(-1);
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::ListBox(XOR("##hotkeys"), &tmp, &names[0], iActiveEntries);
	ImGui::PopStyleColor(4);

	ImGui::End();
}

void Visuals::OnEndScene() {
	g_Interface.pICVar->FindVar(XOR("r_aspectratio"))->SetValue(g_Config.floats[XOR("aspectratio")].val);

	if (pSortedPlayers.empty() || rPlayerRects.empty() || !Game::g_pLocal || Game::g_pLocal == (Player*)0xFFFFFFFF) {
		pSortedPlayers.clear();
		rPlayerRects.clear();
		return;
	}

	for (int i = 0; i < pSortedPlayers.size(); i++) {
		// get and validate player
		Player* pPlayer = pSortedPlayers[i].pPlayer;
		if (!pPlayer || pPlayer == (Player*)0xFFFFFFFF || pPlayer == Game::g_pLocal || !pPlayer->bIsAlive() || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		// get player rect
		RECT rPlayerRect = rPlayerRects[i];

		// paranoia mode
		if (g_Misc.iParanoiaSpecs > 0 && g_Config.ints[XOR("paranoia")].val)
			continue;

		// dormant esp
		if (pPlayer->bDormant()) {
			DrawDormant(pPlayer, rPlayerRect);
		}

		// non dormant esp
		else {
			DrawPlayer(pPlayer, rPlayerRect);
		}
	}
}