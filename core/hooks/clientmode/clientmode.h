#pragma once
#include "includes.h"
#include "pch.h"

namespace SendNetMsg {
	using tSendNetMsg = bool(__thiscall*)(void*, INetMessage*, bool, bool);
	tSendNetMsg oSendNetMsg = nullptr;

	int iIndex = 40;
	__forceinline bool __fastcall hkSendNetMsg(void* thisptr, void* edx, INetMessage* msg, bool rel, bool audio);
}

static bool bClientModeInit = false;
static int iHooksAdded = 0;
void cCreateMove(float flInputSampleTime, CUserCmd* cmd) {

	uintptr_t* pFramePointer;
	__asm mov pFramePointer, ebp;
	bool& bSendPacket = *reinterpret_cast<bool*>(*pFramePointer - 0x1C);

	bSendPacket = g_AntiAim.FL_ShouldSendPacket(cmd);

	if (bSendPacket)
		g_Misc.SetClantag(XOR("LaTeX-Cheat"));

	// update the playerlist
	g_PlayerList.UpdatePlayerList();
	g_Misc.UpdateSpectators();

	// lagcomp stuff
	if (!bClientModeInit) {
		g_Backtrack.Init();
		bClientModeInit = true;
	}

	cmd->buttons |= CUserCmd::IN_BULLRUSH;
	g_Misc.BunnyHop(cmd);

	// save old move values to be able to normally move when shooting
	Vec3D vOldViewangles = cmd->viewangles;
	float flOldForward = cmd->forwardmove;
	float flOldSide = cmd->sidemove;

	g_Interface.pNetChannel = g_Interface.pClientState->pNetChannel;

	//if (g_Interface.pNetChannel && iHooksAdded == 0)
	//	SendNetMsg::oSendNetMsg = (SendNetMsg::tSendNetMsg)g_HookLib.AddHook("client.dll", g_Interface.pNetChannel, SendNetMsg::hkSendNetMsg, SendNetMsg::iIndex, true, "INetChannel");
	
	//iHooksAdded++;

	if (Game::g_pLocal->bIsAlive()) {
		g_Prediction.Start(cmd, Game::g_pLocal);

		LagRecord pRecord;
		g_LegitBot.GetTargetRecord(cmd, &pRecord);
		g_LegitBot.RunAimbot(cmd, &pRecord);
		g_LegitBot.RunTriggerbot(cmd, &pRecord);
		g_Backtrack.ApplyRecord(cmd, &pRecord);

		g_Ragebot.RunAimbot(cmd);
		g_AntiAim.DoDesync(cmd, bSendPacket);

		g_Prediction.End(cmd, Game::g_pLocal);
	}

	g_Math.CorrectMovement(vOldViewangles, cmd, flOldForward, flOldSide);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.Normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	// set cmd to be globally accessible
	Game::g_pCmd = cmd;

	// choke one tick
	//bSendPacket = !bSendPacket;
}

void cDoPostScreenSpaceEffects() {
	if (g_Config.ints[XOR("trustfactor")].val)
		return;
	
	// loop through all glow objects
	for (int i = 0; i < g_Interface.pGlow->glowObjects.size; i++) {
		// get the glow obj
		CGlowObjectManager::GlowObjectDefinition_t& glowObject = g_Interface.pGlow->glowObjects[i];

		if (glowObject.IsUnused())
			continue;

		// make sure ent is valid
		if (!glowObject.entity)
			continue;

		Entity* pEnt = reinterpret_cast<Entity*>(glowObject.entity);
		
		int iClassID = pEnt->cGetClientClass()->m_ClassID;

		switch (iClassID) {
		// player
		case CCSPlayer: 
		{
			Player* player = reinterpret_cast<Player*>(pEnt);
		
			// enemy
			if (Game::g_pLocal->bIsEnemy(player)) {
				if (g_Config.ints["enemyglow"].val)
					glowObject.SetColor(g_Config.arrfloats["enemyglowcol"].val);
			}
			// team
			else {
				if (g_Config.ints["friendlyglow"].val && player != Game::g_pLocal)
					glowObject.SetColor(g_Config.arrfloats["friendlyglowcol"].val);
			}
		}
			break;
		default:
			break;
		}

		if (iClassID == CAK47 || (iClassID >= CWeaponAug && iClassID <= CWeaponZoneRepulsor)) {
			if (g_Config.ints["weaponglow"].val)
				glowObject.SetColor(g_Config.arrfloats["weaponglowcol"].val);
		}
	}
}