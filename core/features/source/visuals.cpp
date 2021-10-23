#include "pch.h"
#include "includes.h"

Visuals g_Visuals;

RECT Visuals::GetPlayerRect(Player* pPlayer) {
	// get pos and add a bit of padding
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

	return rPlayerRect;
}

void Visuals::DrawBox(RECT rPlayerRect, Color col) {
	g_Render.DrawOutlinedRect(rPlayerRect.left, rPlayerRect.top, (rPlayerRect.right - rPlayerRect.left), (rPlayerRect.bottom - rPlayerRect.top), col);

	// box outline
	g_Render.DrawOutlinedRect(rPlayerRect.left + 1, rPlayerRect.top + 1, (rPlayerRect.right - rPlayerRect.left) - 2, (rPlayerRect.bottom - rPlayerRect.top) - 2, Color::Black(col.a));
	g_Render.DrawOutlinedRect(rPlayerRect.left - 1, rPlayerRect.top - 1, (rPlayerRect.right - rPlayerRect.left) + 2, (rPlayerRect.bottom - rPlayerRect.top) + 2, Color::Black(col.a));
}

void Visuals::DrawName(RECT rPlayerRect, Player* pPlayer, Color col) {
	PlayerInfo playerInfo;
	g_Interface.pEngine->GetPlayerInfo(pPlayer->iIndex(), &playerInfo);

	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, playerInfo.name);

	g_Render.Text(g_Render.pEspFont, playerInfo.name, rPlayerRect.left + ((rPlayerRect.right - rPlayerRect.left) / 2), rPlayerRect.top - vSize.y, 14, col);
}

void Visuals::DrawHealth(RECT rPlayerRect, Player* pPlayer, Color col) {
	int iScaledNum = g_Math.ScaleNumber(pPlayer->iHealth(), 100, 0, rPlayerRect.top, rPlayerRect.bottom);

	std::string str = std::to_string(pPlayer->iHealth());
	Vec2D vSize = g_Render.TextSize(g_Render.pEspFont, str.c_str());

	int iWidth = rPlayerRect.right - rPlayerRect.left;

	g_Render.DrawLine(rPlayerRect.left - 5.f, rPlayerRect.top, rPlayerRect.left - 5.f, iScaledNum, 2, Color(50,50,50,col.a));
	g_Render.DrawLine(rPlayerRect.left - 5.f, rPlayerRect.bottom, rPlayerRect.left - 5.f, iScaledNum, 2, col);
	g_Render.Text(g_Render.pEspFont, str.c_str(), rPlayerRect.left - 10.f, iScaledNum - 4, 14,Color::White(col.a));
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

	float flpAlpha = 0xFF * flEntOpacity[pPlayer->iIndex()];

	DrawBox(rPlayerRect, Color(120, 120, 120, flpAlpha));
	DrawName(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha));
	DrawHealth(rPlayerRect, pPlayer, Color(120, 120, 120, flpAlpha));
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

	DrawBox(rPlayerRect, cHealthCol);
	DrawName(rPlayerRect, pPlayer, cHealthCol);
	DrawHealth(rPlayerRect, pPlayer, cHealthCol);
}

void Visuals::OnEndScene() {
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get and validate player
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		RECT rPlayerRect = GetPlayerRect(pPlayer);

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