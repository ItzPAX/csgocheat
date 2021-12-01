#pragma once

#include "directx/directx.h"
#include "includes.h"

void cEndScene() {
	// call visuals
	g_Visuals.OnEndScene();
}

void cMenu() {
	if (!g_Menu.bInitialized)
		g_Menu.Init();

	if (!g_Menu.bToggled)
		return;

	g_Menu.Render();
}