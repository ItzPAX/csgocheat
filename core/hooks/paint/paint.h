#include "includes.h"

void cPaintTraverse() {
	Render::DrawFilledRect(10, 10, 50, 50, Color::White());

	// for now we do this shit here
	Game::pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());
	if (!Game::pLocal)
		return;

	for (int i = 1; i <= 64; i++) {
		Player* curPlayer = (Player*)g_Interface.pClientEntityList->GetClientEntity(i);
		if (!curPlayer || curPlayer->iHealth() < 0 || curPlayer == Game::pLocal || curPlayer->iTeamNum() == Game::pLocal->iTeamNum())
			continue;

		Vec3D entPos2D;
		Vec3D entPos3D = curPlayer->vOrigin();

		if (g_Interface.pDebugOverlay->WorldToScreen(entPos3D, entPos2D))
			Render::DrawLine(entPos2D.x, entPos2D.y, Game::iScreenX / 2, Game::iScreenY, Color::Red());
	}
}