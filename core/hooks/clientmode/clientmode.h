#pragma once
#include "includes.h"
#include "pch.h"

static bool bBacktrackInit = false;
void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	// set cmd to be globally accessible
	Game::g_pCmd = cmd;

	// do shit here
	Game::g_pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());

	if (!Game::g_pLocal)
		return;

	if (!bBacktrackInit) {
		g_Backtrack.Init();
		bBacktrackInit = true;
	}

	g_Backtrack.RecordData();
	
	g_Misc.BunnyHop(cmd);

	if (Variables::bStandaloneRCS)
		g_LegitBot.StandaloneRCS(cmd);

	// call aimbot
	g_LegitBot.AimAtBestPlayer();
}