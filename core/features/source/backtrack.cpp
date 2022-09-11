#include "../header/backtrack.h"

Backtrack g_Backtrack;

bool Backtrack::ValidTick(LagRecord& pRecord) {
	// Get NetChannelInfo
	INetChannelInfo* pNCI = g_Interface.pEngine->GetNetChannelInfo();
	if (!pNCI || !pMaxUnlag)
		return false;

	float flCorrect = 0.f;
	flCorrect += pNCI->GetLatency(FLOW_OUTGOING);
	flCorrect += pNCI->GetLatency(FLOW_INCOMING);
	flCorrect += GetLerpTime();

	flCorrect = std::clamp(flCorrect, 0.f, pMaxUnlag->GetFloat());

	float flDeltaTime = flCorrect - (g_Interface.pGlobalVars->flCurTime - pRecord.flSimTime);

	return fabsf(flDeltaTime) < 0.2f;
}

float Backtrack::GetLerpTime() {
	if (!pUpdateRate || !pInterpRatio || !pInterp)
		return 0.f;

	int iUpdateRate = pUpdateRate->GetInt();
	float flInterpRatio = pInterpRatio->GetFloat();

	float flLerp = flInterpRatio / iUpdateRate;

	if (flLerp <= pInterp->GetFloat())
		flLerp = pInterp->GetFloat();
	return flLerp;
}

void Backtrack::RecordData() {
	INetChannelInfo* pNCI = g_Interface.pEngine->GetNetChannelInfo();
	if (!pNCI)
		return;

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		// validate player
		if (!pPlayer || !pPlayer->bIsPlayer() || pPlayer->bDormant() || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal))
			continue;

		// player updated
		if (pPlayer->flSimTime() != pPlayer->flOldSimTime()) {
			LagRecord record(pPlayer);
			deqLagRecords[i].push_front(record);
		}

		auto invalid = std::find_if(std::cbegin(deqLagRecords[i]), std::cend(deqLagRecords[i]), [](LagRecord rec) { return !g_Backtrack.ValidTick(rec); });
		if (invalid != std::cend(deqLagRecords[i]))
			deqLagRecords[i].erase(invalid, std::cend(deqLagRecords[i]));
	}
}

void Backtrack::ApplyRecord(CUserCmd* cmd, LagRecord* record) {
	// apply tickount to cmd
	if (!cmd || !record)
		return;

	cmd->tick_count = TIME_TO_TICKS(record->flSimTime + GetLerpTime());
}