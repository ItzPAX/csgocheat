#pragma once

#include "directx.h"
#include "includes.h"

void cBeginScene(LPDIRECT3DDEVICE9 pDevice) {
	if (!g_Render.bInitialized) {
		// init d3dx line
		if (!g_Render.InitRenderer())
			return;
		std::cout << "[ RAYBOT ] Successfully Initialized Renderer\n";
	}
}