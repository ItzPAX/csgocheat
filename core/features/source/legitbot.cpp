#include "../header/legitbot.h"

LegitBot g_LegitBot;

float LegitBot::ApplyBezierSmoothingValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals) {
	// get the delta angle and scale it into our spline range
	Vec3D vDelta = (vAngle - vViewAngles).Clamped();
	float flDeltaLength = vDelta.Length();
	// max angle distance can be 180 degrees
	int iScaledNum = (int)g_Math.ScaleNumber(flDeltaLength, g_Config.arrfloats[XOR("legitfov")].val[iActiveWeapon], 0.f, 99.f, 0.f);
	int iIndex = 99 - iScaledNum;

	// adjust smoothing value based on spline
	float randfactor = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (g_Config.ints["legitrandomization"].val + 1)));
	randfactor /= 100.f;
	randfactor += 1.f;
	return (g_Config.arrfloats[XOR("legitsmoothing")].val[iActiveWeapon] / (pBezierVals[iIndex].y * randfactor));
}

void LegitBot::GetTargetRecord(CUserCmd* cmd, LagRecord* pBestRecord) {
	if (Game::g_pLocal->bIsDefusing())
		return;

	int iBestPlayerInd = INVALID;
	Player* pBestPlayer = nullptr;
	float flBestDelta = g_Config.arrfloats[XOR("legitfov")].val[iActiveWeapon];

	float flBestDeltaRecord = FLOAT_MAX;
	int iBestRecord = INVALID;

	Vec3D vViewAngles = cmd->viewangles;

	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();

	Entity* pActiveWeapon = Game::g_pLocal->pGetActiveWeapon();
	if (!pActiveWeapon)
		return;

	int iWeaponType = pActiveWeapon->iGetWeaponType();

	Vec3D vAimPunch{};
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SUBMACHINEGUN:
	case Entity::WEAPONTYPE_MACHINEGUN:
		vAimPunch = Game::g_pLocal->vGetAimPunchAngle();
	}

	// get best player
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		if (!pPlayer || pPlayer->bDormant() || !pPlayer->bIsAlive() || !Game::g_pLocal->bIsEnemy(pPlayer) || pPlayer->bHasGunGameImmunity())
			continue;

		Vec3D vAngle;
		Vec3D vHitboxPos = pPlayer->vGetHitboxPos(HITBOX_HEAD);

		g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
		vAngle.Clamp();

		float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
		if (flCurrDelta < flBestDelta) {
			flBestDelta = flCurrDelta;
			pBestPlayer = pPlayer;
			iBestPlayerInd = i;
		}

		PlayerInfo pinfo;
		g_Interface.pEngine->GetPlayerInfo(i, &pinfo);

		// this is a priorityplayer, if he is in fov range focus him
		if (g_PlayerList.settings[pinfo.isteamid].bPrioritizePlayer) {
			if (flCurrDelta < g_Config.arrfloats[XOR("legitfov")].val[iActiveWeapon]) {
				pBestPlayer = pPlayer;
				iBestPlayerInd = i;
				break;
			}
		}
	}

	if (iBestPlayerInd == INVALID || !pBestPlayer || g_Backtrack.deqLagRecords[iBestPlayerInd].size() <= 3)
		return;

	*pBestRecord = g_Backtrack.deqLagRecords[iBestPlayerInd].front();

	if (!g_Config.ints[XOR("lagcomp")].val || g_Config.ints[XOR("trustfactor")].val) {
		pTargetRecord = &g_Backtrack.deqLagRecords[iBestPlayerInd].front();
		return;
	}

	if (g_Config.ints["legitlagcompmode"].val == 1)
		pTargetRecord = &g_Backtrack.deqLagRecords[iBestPlayerInd].front();
	if (g_Config.ints["legitlagcompmode"].val == 2)
		pTargetRecord = &g_Backtrack.deqLagRecords[iBestPlayerInd].back();

	// get best record
	for (int i = 0; i < g_Backtrack.deqLagRecords[iBestPlayerInd].size(); i++) {
		LagRecord record = g_Backtrack.deqLagRecords[iBestPlayerInd][i];
		if (!g_Backtrack.ValidTick(record))
			continue;

		for (int idx = 0; idx < HITBOX_MAX; idx++) {
			Vec3D vAngle;
			Vec3D vHitboxPos = record.GetHitboxPos(idx).vPos;

			g_Math.CalcAngle(vEyeOrigin, vHitboxPos + vAimPunch * 2.f, vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
			if (flCurrDelta < flBestDeltaRecord) {
				flBestDeltaRecord = flCurrDelta;
				iBestRecord = i;
			}
		}
	}

	if (iBestRecord != INVALID) {
		if (g_Config.ints["legitlagcompmode"].val == 0)
			pTargetRecord = &g_Backtrack.deqLagRecords[iBestPlayerInd][iBestRecord];
		*pBestRecord = g_Backtrack.deqLagRecords[iBestPlayerInd][iBestRecord];
	}
	return;
}

void LegitBot::RunTriggerbot(CUserCmd* cmd, LagRecord* pBestRecord) {
	if (!g_Config.ints["triggerbot"].val)
		return;

	if (!g_InputMgr.GetHotkeyState(g_Config.arrints[XOR("triggerbotkey")].val, XOR("Triggerbot")))
		return;

	if (!pBestRecord)
		return;

	// 8192 is the max weaponrange src: http://strike-counter.com/cs-go-stats/weapons-data
	Vec3D vEndPoint;
	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();
	vEndPoint.Init(vEyeOrigin.x + (8192 * cosf(cmd->viewangles.y * (M_PI / 180.0)) * cosf(cmd->viewangles.x * (M_PI / 180.0))),
		vEyeOrigin.y + (8192 * sinf(cmd->viewangles.y * (M_PI / 180.0)) * cosf(cmd->viewangles.x * (M_PI / 180.0))),
		vEyeOrigin.z - (8192 * sinf(cmd->viewangles.x * (M_PI / 180.0))));

	Ray_t triggerbotRay;
	triggerbotRay.Init(vEyeOrigin, vEndPoint);
	CTraceFilter filter;
	filter.pSkip = Game::g_pLocal;

	CGameTrace trace;
	g_Interface.pEngineTrace->TraceRay(triggerbotRay, MASK_SHOT, &filter, &trace);

	// get entity from trace
	if (!trace.m_pEnt || !trace.m_pEnt->bIsPlayer())
		return;

	// check for hitgroup
	if (trace.hitbox < HITBOX_HEAD || trace.hitbox > HITBOX_MAX)
		return;

	Player* pPlayer = reinterpret_cast<Player*>(trace.m_pEnt);
	if (!pPlayer->bIsEnemy(Game::g_pLocal))
		return;

	cmd->buttons |= CUserCmd::IN_ATTACK;
}

void LegitBot::CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunch, float flRCS, float flSmoothing) {
	vAngle.x -= (vAimPunch.x * (1.f + (flRCS / 100.f))) / flSmoothing;
	vAngle.y -= (vAimPunch.y * (1.f + (flRCS / 100.f))) / flSmoothing;
}

void LegitBot::UpdateHitboxes() {
	vAllowedHitboxes.clear();
	for (int i = 0; i < g_Config.arrints[XOR("legithitboxes")].size; i++) {
		if (g_Config.arrints[XOR("legithitboxes")].val[i]) {
			switch (i) {
			case 0:
				vAllowedHitboxes.push_back(HITBOX_HEAD);
				vAllowedHitboxes.push_back(HITBOX_NECK);
				break;
			case 1:
				vAllowedHitboxes.push_back(HITBOX_CHEST);
				vAllowedHitboxes.push_back(HITBOX_LOWER_CHEST);
				vAllowedHitboxes.push_back(HITBOX_UPPER_CHEST);
				break;
			case 2:
				vAllowedHitboxes.push_back(HITBOX_STOMACH);
				vAllowedHitboxes.push_back(HITBOX_PELVIS);
				break;
			case 3:
				vAllowedHitboxes.push_back(HITBOX_LEFT_CALF);
				vAllowedHitboxes.push_back(HITBOX_LEFT_THIGH);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_CALF);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_THIGH);
				break;
			}
		}
	}
}

void LegitBot::AimAtPosition(float x, float y, float smoothing) {
	int width = g_DirectX.iWindowWidth;
	int height = g_DirectX.iWindowHeight;
	int ScreenCenterY = height * 0.5f;
	int ScreenCenterX = width * 0.5f;

	float AimSpeed = smoothing;
	float TargetX = 0;
	float TargetY = 0;

	// only aim if we are focused into csgo and our menu isn't opened
	if (g_Menu.bToggled || GetForegroundWindow() != g_DirectX.window || g_Interface.pEngine->IsPaused())
		return;

	//X Axis
	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	//Y Axis
	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}


	//improve acc and prevent "overshooting" as good as possible
	if (fabs(TargetX) < 1)
	{
		if (TargetX > 0)
		{
			TargetX = 1;
		}
		if (TargetX < 0)
		{
			TargetX = -1;
		}
	}
	if (fabs(TargetY) < 1)
	{
		if (TargetY > 0)
		{
			TargetY = 1;
		}
		if (TargetY < 0)
		{
			TargetY = -1;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
}

void LegitBot::RunAimbot(CUserCmd* cmd, LagRecord* pBestRecord) {
	if (!g_Config.ints[XOR("legitbot")].val)
		return;

	if (!g_InputMgr.GetHotkeyState(g_Config.arrints[XOR("legitbotkey")].val, XOR("Legitbot")))
		return;

	if (!pTargetRecord)
		return;

	// paranoia mode
	if (g_Misc.iParanoiaSpecs > 0 && g_Config.ints[XOR("paranoia")].val)
		return;

	Entity* pActiveWeapon = Game::g_pLocal->pGetActiveWeapon();
	if (!pActiveWeapon)
		return;

	// only run aimbot on actual weapons, not grenades etc.
	int iWeaponType = pActiveWeapon->iGetWeaponType();
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_MACHINEGUN:
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SHOTGUN:
	case Entity::WEAPONTYPE_PISTOL:
	case Entity::WEAPONTYPE_SNIPER:
	{
		if (!pActiveWeapon->iClip())
			return;
		break;
	}
	default:
		return;
	}

	if (iWeaponType == Entity::WEAPONTYPE_SNIPER)
		iActiveWeapon = 0;
	if (iWeaponType == Entity::WEAPONTYPE_MACHINEGUN || iWeaponType == Entity::WEAPONTYPE_RIFLE)
		iActiveWeapon = 1;
	if (iWeaponType == Entity::WEAPONTYPE_SHOTGUN || iWeaponType == Entity::WEAPONTYPE_PISTOL)
		iActiveWeapon = 2;

	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();
	Vec3D vViewAngles; g_Interface.pEngine->GetViewAngles(vViewAngles);

	int iHitbox = HITBOX_NONE;
	float flBestDelta = g_Config.arrfloats[XOR("legitfov")].val[iActiveWeapon];

	UpdateHitboxes();

	for (int i = 0; i < vAllowedHitboxes.size(); i++) {
		if (pTargetRecord->GetHitboxPos(vAllowedHitboxes[i]).bVisible) {
			Vec3D vAngle;
			g_Math.CalcAngle(vEyeOrigin, pTargetRecord->GetHitboxPos(vAllowedHitboxes[i]).vPos, vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
			if (flCurrDelta < flBestDelta) {
				flBestDelta = flCurrDelta;
				iHitbox = vAllowedHitboxes[i];
			}
		}
	}

	if (iHitbox == HITBOX_NONE)
		return;

	Vec3D vAimPunch{};
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SUBMACHINEGUN:
	case Entity::WEAPONTYPE_MACHINEGUN:
	case Entity::WEAPONTYPE_SNIPER:
		vAimPunch = Game::g_pLocal->vGetAimPunchAngle();
	}

	Vec3D vEnemyAngle;
	g_Math.CalcAngle(vEyeOrigin, pTargetRecord->GetHitboxPos(iHitbox).vPos, vEnemyAngle);
	vEnemyAngle.Clamp();

	float flSmoothing = ApplyBezierSmoothingValues(vViewAngles, vEnemyAngle, vAimbotCurve);

	// clamp smoothing
	g_Math.Clamp(&flSmoothing, FLT_MAX, 1.f);
	vEnemyAngle = (vViewAngles + (vEnemyAngle - vViewAngles).Clamped() / flSmoothing).Clamped();

	CompensateRecoil(vEnemyAngle, vAimPunch, g_Config.arrfloats[XOR("legitrcs")].val[iActiveWeapon], flSmoothing);

	if (g_Config.ints[XOR("trustfactor")].val) {
		Vec3D vTarget;
		g_Interface.pDebugOverlay->WorldToScreen(pTargetRecord->GetHitboxPos(iHitbox).vPos, vTarget);
		vTarget += vTarget * ((g_Config.arrfloats[XOR("legitrandom")].val[iActiveWeapon] / 100.f) * (rand() % 2 ? 1 : -1)); // randomization
		AimAtPosition(vTarget.x, vTarget.y, flSmoothing);
	}
	else {
		// set cmd viewangles, cuz its faster then shoot and then set engine viewangles so it all looks legit
		cmd->viewangles = vEnemyAngle;
		g_Interface.pEngine->SetViewAngles(vEnemyAngle);
	}

	// invalidate record after we used it
	pTargetRecord = nullptr;
}