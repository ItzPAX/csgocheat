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

	if (Game::g_pLocal->bIsAlive()) {
		LagRecord pRecord;
		g_LegitBot.GetTargetRecord(cmd, &pRecord);
		g_LegitBot.RunAimbot(cmd, &pRecord);
		g_Backtrack.ApplyRecord(cmd, &pRecord);
	}

	g_Misc.BunnyHop(cmd);
}

void cDoPostScreenSpaceEffects() {
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
		case CClientClass::CCSPlayer: 
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

		if (iClassID == CClientClass::CAK47 || (iClassID >= CClientClass::CWeaponAug && iClassID <= CClientClass::CWeaponZoneRepulsor)) {
			if (g_Config.ints["weaponglow"].val)
				glowObject.SetColor(g_Config.arrfloats["weaponglowcol"].val);
		}
	}
}