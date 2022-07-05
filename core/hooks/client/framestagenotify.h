#pragma once
#include "pch.h"
#include "includes.h"

void SetInterpolation(Player* ent, bool flag) {
	VarMapping_t* map = (VarMapping_t*)((uintptr_t)ent + 36);
	for (int i = 0; i < map->m_nInterpolatedEntries; i++) {
		VarMapEntry_t* e = &map->m_Entries[i];
		if (!e)
			continue;
		e->m_bNeedsToInterpolate = flag;
	}
}

void cFrameStageNotify(IBaseClientDLL::ClientFrameStage_t curStage) {
	g_Visuals.ThirdPerson();

	switch (curStage) {
	case IBaseClientDLL::FRAME_NET_UPDATE_START:
		g_Backtrack.RecordData();
		break;
	case IBaseClientDLL::FRAME_NET_UPDATE_END:
		if (Game::g_pLocal && Game::g_pLocal->bIsAlive()) {
			for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
				Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
				if (!pPlayer || !pPlayer->bIsPlayer() || !pPlayer->bIsAlive() || !pPlayer->bIsEnemy(Game::g_pLocal) || pPlayer == Game::g_pLocal || pPlayer->bDormant())
					continue;
				SetInterpolation(pPlayer, false);
			}
		}
		break;
	}
}