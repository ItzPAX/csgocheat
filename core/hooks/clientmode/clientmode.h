#pragma once
#include "includes.h"
#include "pch.h"

void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	// set cmd to be globally accessible
	Game::g_pCmd = cmd;

	// do shit here
	Game::g_pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());

	// check if we have a local player
	if (Game::g_pLocal) {
		if (Variables::bStandaloneRCS)
			g_LegitBot.StandaloneRCS(cmd);

		// call aimbot
		g_LegitBot.AimAtBestPlayer();
	}
}