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

	// thirdperson
	g_Visuals.ThirdPerson();

	// lagcomp stuff
	if (!bBacktrackInit) {
		g_Backtrack.Init();
		bBacktrackInit = true;
	}
	{
		{
			{
				{
					{
						{
							{
								{
									{
										g_Backtrack.RecordData();
										LagRecord* pRecord = g_Backtrack.Lagcompensation(cmd);

										// call aimbot with best record
										if (Game::g_pLocal->bIsAlive())
											g_LegitBot.AimAtBestPlayer(pRecord);

										g_Backtrack.ApplyRecord(cmd, pRecord);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	g_Misc.BunnyHop(cmd);

	if (Variables::bStandaloneRCS)
		g_LegitBot.StandaloneRCS(cmd);
}