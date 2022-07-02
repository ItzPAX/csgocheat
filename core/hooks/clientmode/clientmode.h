#pragma once
#include "includes.h"
#include "pch.h"

static bool bClientModeInit = false;
void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	uintptr_t* pFramePointer;
	__asm mov pFramePointer, ebp;
	bool& bSendPacket = *reinterpret_cast<bool*>(*pFramePointer - 0x1C);

	// set cmd to be globally accessible
	Game::g_pCmd = cmd;

	// lagcomp stuff
	if (!bClientModeInit) {
		g_Backtrack.Init();
		bClientModeInit = true;
	}

	LagRecord* pRecord = g_LegitBot.GetTargetRecord(cmd);

	if (Game::g_pLocal->bIsAlive()) {
		g_LegitBot.RunAimbot(cmd);
	}

	g_Backtrack.ApplyRecord(cmd, pRecord);

	g_Misc.BunnyHop(cmd);
}