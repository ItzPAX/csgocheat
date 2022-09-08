#pragma once

#include "directx.h"
#include "includes.h"

void cEndScene() {
	// goofy ahh (by emlin)
	g_Visuals.GoofyAhhCrosshair();

	// call visuals
	if (Game::g_pLocal && g_Interface.pEngine->IsInGame())
		g_Visuals.OnEndScene();
	g_Visuals.DrawWatermark();
}

void cMenu() {
	if (!g_Menu.bInitialized)
		g_Menu.Init();

	g_Menu.Render();
}