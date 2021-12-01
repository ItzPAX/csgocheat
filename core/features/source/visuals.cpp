#include "pch.h"
#include "includes.h"

Visuals g_Visuals;

void Visuals::UpdatePlayerRects() {
	for (int i = 0; i < g_Visuals.pSortedPlayers.size(); i++) {
		// get and validate player
		Player* pPlayer = pSortedPlayers[i].pPlayer;

		// get pos
		Vec3D vPlayerPosTop = pPlayer->GetBonePosition(8); // bone id 8 is the head
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

	for (int i = 0; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get and validate player
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		float flDist = pPlayer->vAbsOrigin().Distance(Game::g_pLocal->vAbsOrigin());
		
		pSortedPlayers.push_back(PlayerDist(flDist, pPlayer));
	}

	std::sort(pSortedPlayers.begin(), pSortedPlayers.end(), SortDistance);
}

void Visuals::DrawBox(RECT rPlayerRect, Color col) {
	g_Render.DrawOutlinedRect(rPlayerRect.left, rPlayerRect.top, (rPlayerRect.right - rPlayerRect.left), (rPlayerRect.bottom - rPlayerRect.top), col);

	// box outline
	g_Render.DrawOutlinedRect(rPlayerRect.left + 1, rPlayerRect.top + 1, (rPlayerRect.right - rPlayerRect.left) - 2, (rPlayerRect.bottom - rPlayerRect.top) - 2, Color::Black(col.a));
	g_Render.DrawOutlinedRect(rPlayerRect.left - 1, rPlayerRect.top - 1, (rPlayerRect.right - rPlayerRect.left) + 2, (rPlayerRect.bottom - rPlayerRect.top) + 2, Color::Black(col.a));
}

void Visuals::DrawName(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info) {
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, info.name);

	// box
	g_Render.DrawFilledRect((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 2), (rPlayerRect.top - vSize.y * 1.5) - 6, vSize.x, vSize.y * 1.5f, Color(50, 50, 50, 200));

	// text
	g_Render.Text(g_Render.pEspFont, info.name, rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2), (rPlayerRect.top - vSize.y * 1.5f) - 4, 14, col);
}

void Visuals::DrawHealth(RECT rPlayerRect, Player* pPlayer, Color col, PlayerInfo& info) {
	float flScaledNum = g_Math.ScaleNumber(pPlayer->iHealth(), 100.f, 0.f, 1.f, -1.f);

	std::string str = std::to_string(pPlayer->iHealth());
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, info.name);

	int iWidth = rPlayerRect.right - rPlayerRect.left;

	g_Render.DrawLine((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 2), (rPlayerRect.top - 6), (rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) + (vSize.x / 2), (rPlayerRect.top - 6), 1, Color(130, 130, 130, 150));
	g_Render.DrawLine((rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) - (vSize.x / 2), (rPlayerRect.top - 6), (rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2)) + ((vSize.x / 2) * flScaledNum), (rPlayerRect.top - 6), 1, col);

	//g_Render.DrawLine(rPlayerRect.left - 5.f, rPlayerRect.bottom - 1.f, rPlayerRect.left - 5.f, iScaledNum + 1.f, 2, col);
	//g_Render.Text(g_Render.pEspFont, str.c_str(), rPlayerRect.left - 10.f, iScaledNum - 4, 14, Color::White(col.a));
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