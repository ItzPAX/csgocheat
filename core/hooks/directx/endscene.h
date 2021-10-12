#include "includes.h"

// here we can draw the shit
void cheatEndScene() {
	// for now we do this shit here
	Game::pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());
	for (int i = 1; i < 32; i++) {
		Player* curPlayer = (Player*)g_Interface.pClientEntityList->GetClientEntity(i);
		if (!curPlayer->bIsValid() || i != 2 || !Game::pLocal)
			continue;

		Vec3D entPos2d;
		if (g_Math.WorldToScreen(curPlayer->vecOrigin(), entPos2d)) {
			std::cout << "Player 2 w2s x: " << entPos2d.x << "Player 2 w2s y: " << entPos2d.y << '\n';
		}
			//Render::DrawLine(entPos2d.x, entPos2d.y, g_DirectX.iWindowWidth / 2, g_DirectX.iWindowHeight, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	Render::DrawFilledRect(10, 10, 50, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
}