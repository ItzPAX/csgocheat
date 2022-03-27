#include "../header/backtrack.h"

Backtrack g_Backtrack;

bool Backtrack::ValidTick(LagRecord& pRecord) {
	// Get NetChannelInfo
	INetChannelInfo* pNCI = g_Interface.pEngine->GetNetChannelInfo();
	if (!pNCI || !pUpdateRate || !pMaxUnlag || !pInterpRatio || !pInterp)
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
	// Get LerpTime
	int iUpdateRate = pUpdateRate->GetInt();
	float flInterpRatio = pInterpRatio->GetFloat();

	float flLerp = flInterpRatio / iUpdateRate;

	if (flLerp <= pInterp->GetFloat())
		flLerp = pInterp->GetFloat();
	return flLerp;
}

void Backtrack::RecordData() {

	if (!Variables::bLagcomp)
		return;

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

LagRecord* Backtrack::Lagcompensation() {
	if (!Variables::bLagcomp)
		return nullptr;

	int iBestPlayerInd = INVALID;
	Player* pBestPlayer = nullptr;
	float flBestDelta = FLOAT_MAX;
	float flBestDeltaRecord = FLOAT_MAX;

	int iBestRecord = INVALID;

	Vec3D vViewAngles;
	g_Interface.pEngine->GetViewAngles(vViewAngles);

	// local eye pos
	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();

	// get local aimpunchangle
	Vec3D vAimPunchAngle = Game::g_pLocal->vGetAimPunchAngle();

	// get best player
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get entity and validate it
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal) || pPlayer->bDormant())
			continue;

		Vec3D vAngle;

		Vec3D vHitboxPos = pPlayer->vGetHitboxPos(HitboxChest);

		g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
		vAngle.Clamp();

		float flCurrDelta = (vAngle - vAimPunchAngle - vViewAngles).Clamped().Length();
		if (flCurrDelta < flBestDelta) {
			flBestDelta = flCurrDelta;
			pBestPlayer = pPlayer;
			iBestPlayerInd = i;
		}
	}

	if (iBestPlayerInd == INVALID || !pBestPlayer || deqLagRecords[iBestPlayerInd].size() <= 3)
		return nullptr;

	// get best record
	for (int i = 0; i < deqLagRecords[iBestPlayerInd].size(); i++) {
		LagRecord record = deqLagRecords[iBestPlayerInd][i];
		if (!ValidTick(record))
			continue;

		if (Variables::bAccurateLagcomp) {
			for (int j = 0; j < sizeof(Variables::iAllowedHitboxes) / sizeof(int); j++) {
				Vec3D vAngle;

				Vec3D vHitboxPos = record.GetHitboxPos(Variables::iAllowedHitboxes[j]);

				g_Math.CalcAngle(vEyeOrigin, vHitboxPos + (Game::g_pLocal->vGetAimPunchAngle() * 2.f), vAngle);
				vAngle.Clamp();

				float flCurrDelta = (vAngle - vAimPunchAngle - vViewAngles).Clamped().Length();
				if (flCurrDelta < flBestDeltaRecord) {
					flBestDeltaRecord = flCurrDelta;
					iBestRecord = i;
				}
			}
		}
		else {
			Vec3D vAngle;

			Vec3D vHitboxPos = record.GetHitboxPos(HitboxHead);

			g_Math.CalcAngle(vEyeOrigin, vHitboxPos + (Game::g_pLocal->vGetAimPunchAngle() * 2.f), vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vAimPunchAngle - vViewAngles).Clamped().Length();
			if (flCurrDelta < flBestDeltaRecord) {
				flBestDeltaRecord = flCurrDelta;
				iBestRecord = i;
			}
		}
	}

	if (iBestRecord != INVALID)
		return &deqLagRecords[iBestPlayerInd][iBestRecord];
	return nullptr;
}

void Backtrack::ApplyRecord(CUserCmd* cmd, LagRecord* record) {
	// apply tickount to cmd
	if (!cmd || !record)
		return;

	if (cmd->buttons & InAttack) {
		cmd->tick_count = TIME_TO_TICKS(record->flSimTime);
	}
}