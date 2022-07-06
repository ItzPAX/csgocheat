#pragma once

#include "directx.h"
#include "includes.h"

void cEndScene() {
	// call visuals
	if (Game::g_pLocal)
		g_Visuals.OnEndScene();
	g_Visuals.DrawWatermark();
}

void cMenu() {
	if (!g_Menu.bInitialized)
		g_Menu.Init();

	if (!g_Menu.bToggled)
		return;

	g_Menu.Render();
}