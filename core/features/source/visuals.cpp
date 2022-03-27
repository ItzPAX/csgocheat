#include "pch.h"
#include "includes.h"

Visuals g_Visuals;

void Visuals::ThirdPerson() {
	static bool bInThirdPerson = false;

	if (GetAsyncKeyState(VK_MBUTTON) & 1)
		bInThirdPerson = !bInThirdPerson;

	g_Interface.pInput->bCameraInThirdperson = bInThirdPerson;
	g_Interface.pInput->vCameraOffset.z = 100.f;
}

void Visuals::UpdatePlayerRects() {
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
	g_Visuals.rPlayerRects.clear();

	static auto SortDistance = [](PlayerDist a, PlayerDist b) {
		return a.flDist > b.flDist;
	};

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get and validate player
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		float flDist = pPlayer->vAbsOrigin().DistanceTo(Game::g_pLocal->vAbsOrigin());
		
		pSortedPlayers.push_back(PlayerDist(flDist, pPlayer));
	}

	std::sort(pSortedPlayers.begin(), pSortedPlayers.end(), SortDistance);
}

void Visuals::DrawBox(RECT rPlayerRect, Color col) {
	g_Render.DrawOutlinedRect(rPlayerRect.left, rPlayerRect.top, (rPlayerRect.right - rPlayerRect.left), (rPlayerRect.bottom - rPlayerRect.top), Color(20,100,130,col.rgba[3]));

	// box outline
	g_Render.DrawOutlinedRect(rPlayerRect.left + 1, rPlayerRect.top + 1, (rPlayerRect.right - rPlayerRect.left) - 2, (rPlayerRect.bottom - rPlayerRect.top) - 2, Color::Black(col.rgba[3]));
	g_Render.DrawOutlinedRect(rPlayerRect.left - 1, rPlayerRect.top - 1, (rPlayerRect.right - rPlayerRect.left) + 2, (rPlayerRect.bottom - rPlayerRect.top) + 2, Color::Black(col.rgba[3]));
}

void Visuals::DrawName(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info) {
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, info.name);

	// box
	g_Render.DrawFilledRect((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 1.6), (rPlayerRect.top - vSize.y * 1.5) - 1, vSize.x * 1.3f, vSize.y * 1.25f, Color(30, 30, 30, col.rgba[3]));
	g_Render.DrawLine((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 1.6), (rPlayerRect.top - 4), (rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) + (vSize.x / 1.5), (rPlayerRect.top - 4), 1, Color(130,20,60, col.rgba[3]));

	// text
	g_Render.Text(g_Render.pEspFont, info.name, rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2), (rPlayerRect.top - vSize.y * 1.5f) - 1, col);
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

	if (Variables::bBoxEsp) DrawBox(rPlayerRect, Color(120, 120, 120, flpAlpha));
	if (Variables::bNameEsp) DrawName(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha), playerInfo);
	if (Variables::bHealthEsp) DrawHealth(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha), playerInfo);
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

	int flpAlpha = 0xFF * flEntOpacity[pPlayer->iIndex()];

	// construct our color
	Color cHealthCol(((100 - pPlayer->iHealth()) * 2.55f), (pPlayer->iHealth() * 2.55f), 0.f, flpAlpha);

	PlayerInfo playerInfo;
	g_Interface.pEngine->GetPlayerInfo(pPlayer->iIndex(), &playerInfo);

	if (Variables::bBoxEsp) DrawBox(rPlayerRect, cHealthCol);
	if (Variables::bNameEsp) DrawName(rPlayerRect, pPlayer, Color::White(), playerInfo);
	if (Variables::bHealthEsp) DrawHealth(rPlayerRect, pPlayer, cHealthCol, playerInfo);
}

void Visuals::OnEndScene() {
	for (int i = 0; i < pSortedPlayers.size(); i++) {
		// get and validate player
		Player* pPlayer = pSortedPlayers[i].pPlayer;
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		// get player rect
		RECT rPlayerRect = rPlayerRects[i];

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