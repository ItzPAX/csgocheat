#pragma once
#include "includes.h"
#include "pch.h"

void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	// set cmd to be globally accessible
	Game::pCmd = cmd;

	// do shit here
	Game::pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());
}