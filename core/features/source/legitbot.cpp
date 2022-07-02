#include "../header/legitbot.h"

LegitBot g_LegitBot;

float LegitBot::ApplyBezierSmoothingValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals) {
	// get the delta angle and scale it into our spline range
	Vec3D vDelta = (vAngle - vViewAngles).Clamped();
	float flDeltaLength = vDelta.Length2D();
	// max angle distance can be 180 degrees
	int iScaledNum = (int)g_Math.ScaleNumber(flDeltaLength, g_Config.arrfloats["legitfov"].val[iSelWeapon], 0.f, 99.f, 0.f);
	int iIndex = 99 - iScaledNum;

	// adjust smoothing value based on spline
	return g_Config.arrfloats["legitsmoothing"].val[iSelWeapon] * (2 - pBezierVals[iIndex].y);
}

LagRecord* LegitBot::GetTargetRecord(CUserCmd* cmd) {
	if (Game::g_pLocal->bIsDefusing())
		return nullptr;

	Entity* pActiveWeapon = Game::g_pLocal->pGetActiveWeapon();
	if (!pActiveWeapon)
		return nullptr;

	// only run aimbot on actual weapons, not grenades etc.
	int iWeaponType = pActiveWeapon->iGetWeaponType();

	Vec3D vAimPunch{};
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SUBMACHINEGUN:
	case Entity::WEAPONTYPE_MACHINEGUN:
		vAimPunch = Game::g_pLocal->vGetAimPunchAngle();
	}

	int iBestPlayerInd = INVALID;
	Player* pBestPlayer = nullptr;
	float flBestDelta = g_Config.arrfloats["legitfov"].val[iSelWeapon];

	float flBestDeltaRecord = FLOAT_MAX;
	int iBestRecord = INVALID;

	Vec3D vViewAngles;
	g_Interface.pEngine->GetViewAngles(vViewAngles);

	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();

	// get best player
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		if (!pPlayer || pPlayer->bDormant() || !pPlayer->bIsAlive() || !Game::g_pLocal->bIsEnemy(pPlayer) || pPlayer->bHasGunGameImmunity())
			continue;

		Vec3D vAngle;
		Vec3D vHitboxPos = pPlayer->vGetHitboxPos(HITBOX_HEAD);

		g_Math.CalcAngle(vEyeOrigin, vHitboxPos + vAimPunch * 2, vAngle);
		vAngle.Clamp();

		float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
		if (flCurrDelta < flBestDelta) {
			flBestDelta = flCurrDelta;
			pBestPlayer = pPlayer;
			iBestPlayerInd = i;
		}
	}

	if (iBestPlayerInd == INVALID || !pBestPlayer || g_Backtrack.deqLagRecords[iBestPlayerInd].size() <= 3)
		return nullptr;

	// aimbot will always aim at most recent record
	pTargetRecord = &g_Backtrack.deqLagRecords[iBestPlayerInd].front();

	if (!g_Config.ints["lagcomp"].val)
		return &g_Backtrack.deqLagRecords[iBestPlayerInd].front();

	// get best record
	for (int i = 0; i < g_Backtrack.deqLagRecords[iBestPlayerInd].size(); i++) {
		LagRecord record = g_Backtrack.deqLagRecords[iBestPlayerInd][i];
		if (!g_Backtrack.ValidTick(record))
			continue;

		Vec3D vAngle;

		Vec3D vHitboxPos = record.GetHitboxPos(HITBOX_HEAD);

		g_Math.CalcAngle(vEyeOrigin, vHitboxPos + vAimPunch * 2, vAngle);
		vAngle.Clamp();

		float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
		if (flCurrDelta < flBestDeltaRecord) {
			flBestDeltaRecord = flCurrDelta;
			iBestRecord = i;
		}
	}

	if (iBestRecord != INVALID) {
		return &g_Backtrack.deqLagRecords[iBestPlayerInd][iBestRecord];
	}
	return nullptr;
}

void LegitBot::CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunch, float flRCS, float flSmoothing) {
	vAngle.x -= (vAimPunch.x * (1.f + (flRCS / 100.f))) / flSmoothing;
	vAngle.y -= (vAimPunch.y * (1.f + (flRCS / 100.f))) / flSmoothing;
}

void LegitBot::RunAimbot(CUserCmd* cmd) {
	if (!g_Config.ints["legitbot"].val)
		return;

	// only run if we are trying to shoot
	if (!(cmd->buttons & CUserCmd::IN_ATTACK))
		return;

	if (!pTargetRecord)
		return;

	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();
	Vec3D vViewAngles; g_Interface.pEngine->GetViewAngles(vViewAngles);

	int iHitbox = HITBOX_NONE;

	for (int i = 0; i < HITBOX_MAX; i++) {
		if (pTargetRecord->pTargetPlayer->bIsPointVisible(vEyeOrigin, pTargetRecord->GetHitboxPos(i))) {
			iHitbox = i;
			break;
		}
	}

	if (iHitbox == HITBOX_NONE)
		return;

	Entity* pActiveWeapon = Game::g_pLocal->pGetActiveWeapon();
	if (!pActiveWeapon)
		return;

	// only run aimbot on actual weapons, not grenades etc.
	int iWeaponType = pActiveWeapon->iGetWeaponType();
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_MACHINEGUN:
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SNIPER:
	case Entity::WEAPONTYPE_SHOTGUN:
	case Entity::WEAPONTYPE_PISTOL:
	{
		if (!pActiveWeapon->iClip())
			return;
		break;
	}
	default:
		return;
	}

	Vec3D vAimPunch{};
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SUBMACHINEGUN:
	case Entity::WEAPONTYPE_MACHINEGUN:
		vAimPunch = Game::g_pLocal->vGetAimPunchAngle();
	}

	cmd->buttons &= ~CUserCmd::IN_ATTACK;

	Vec3D vEnemyAngle;
	g_Math.CalcAngle(vEyeOrigin, pTargetRecord->GetHitboxPos(iHitbox), vEnemyAngle);
	vEnemyAngle.Clamp();

	float flSmoothing = ApplyBezierSmoothingValues(vViewAngles, vEnemyAngle, vAimbotCurve);

	// clamp smoothing
	g_Math.Clamp(&flSmoothing, FLT_MAX, 1.f);
	vEnemyAngle = (vViewAngles + (vEnemyAngle - vViewAngles).Clamped() / flSmoothing).Clamped();

	CompensateRecoil(vEnemyAngle, vAimPunch, g_Config.arrfloats["legitrcs"].val[iSelWeapon], flSmoothing);

	// set cmd viewangles, cuz its faster then shoot and then set engine viewangles so it all looks legit
	cmd->viewangles = vEnemyAngle;
	cmd->buttons |= CUserCmd::IN_ATTACK;
	g_Interface.pEngine->SetViewAngles(vEnemyAngle);

	// invalidate record after we used it
	pTargetRecord = nullptr;
}