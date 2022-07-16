#include "../header/misc.h"

Misc g_Misc;


void Misc::UpdateSpectators() {
	pSpectators.clear();

	Player* pSpecPlayer = Game::g_pLocal->bIsAlive() ? Game::g_pLocal : reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntityFromHandle(Game::g_pLocal->hObserverTarget()));
	if (!pSpecPlayer)
		return;

	PlayerInfo specinfo;
	g_Interface.pEngine->GetPlayerInfo(pSpecPlayer->iIndex(), &specinfo);

	speclistname = std::string(specinfo.name);
	iParanoiaSpecs = 0;

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsPlayer() || pPlayer->bDormant() || pPlayer->bIsAlive() || pPlayer == Game::g_pLocal)
			continue;

		if (reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntityFromHandle(pPlayer->hObserverTarget())) != pSpecPlayer)
			continue;

		Spectator spec;

		PlayerInfo pinfo;
		g_Interface.pEngine->GetPlayerInfo(i, &pinfo);

		if (!g_PlayerList.settings[pinfo.isteamid].bIgnoreFromParanoia && Game::g_pLocal->bIsAlive())
			iParanoiaSpecs++;

		spec.name = std::string(pinfo.name);

		switch (pPlayer->iObserverMode()) {
		case OBS_MODE_NONE:
			spec.obsmode = XOR("None");
			break;
		case OBS_MODE_DEATHCAM:
			spec.obsmode = XOR("Deathcam");
			break;
		case OBS_MODE_FREEZECAM:
			spec.obsmode = XOR("Freezecam");
			break;
		case OBS_MODE_FIXED:
			spec.obsmode = XOR("Fixed");
			break;
		case OBS_MODE_IN_EYE:
			spec.obsmode = XOR("Firstperson");
			break;
		case OBS_MODE_CHASE:
			spec.obsmode = XOR("Thirdperson");
			break;
		case OBS_MODE_ROAMING:
			spec.obsmode = XOR("Roaming");
			break;
		}

		pSpectators.push_back(spec);
	}
}

void Misc::BunnyHop(CUserCmd* cmd) {
	if (!g_Config.ints[XOR("bunnyhop")].val || g_Config.ints[XOR("trustfactor")].val)
		return;

	if (!(Game::g_pLocal->iFlags() & Entity::FL_ONGROUND))
		cmd->buttons &= ~CUserCmd::IN_JUMP;
}