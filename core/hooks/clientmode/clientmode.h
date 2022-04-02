#pragma once
#include "includes.h"
#include "pch.h"

static bool bBacktrackInit = false;
void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {
	if (!cmd || !cmd->command_number)
		return;

	uintptr_t* pFramePointer;
	__asm mov pFramePointer, ebp;
	bool& bSendPacket = *reinterpret_cast<bool*>(*pFramePointer - 0x1C);

	// set cmd to be globally accessible
	Game::g_pCmd = cmd;

	// do shit here
	Game::g_pLocal = (Player*)g_Interface.pClientEntityList->GetClientEntity(g_Interface.pEngine->GetLocalPlayer());

	if (!Game::g_pLocal)
		return;

	if (NetvarOffsets::iHealth == 0)
		NetvarOffsets::iHealth = g_NetVars.GetOffsetDirect(XOR("DT_BasePlayer"), XOR("m_iHealth"), Game::g_pLocal);

	// lagcomp stuff
	if (!bBacktrackInit) {
		g_Backtrack.Init();
		bBacktrackInit = true;
	}

	g_Backtrack.pBestRecord = g_Backtrack.Lagcompensation();

	// call aimbot with best record
	if (Game::g_pLocal->bIsAlive())
		g_LegitBot.AimAtBestPlayer(g_Backtrack.pBestRecord);

	g_Backtrack.ApplyRecord(Game::g_pCmd, g_Backtrack.pBestRecord);

	g_Misc.BunnyHop(cmd);

	//if (Variables::bStandaloneRCS)
		//g_LegitBot.StandaloneRCS(cmd);
}