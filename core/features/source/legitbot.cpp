#include "../header/legitbot.h"

LegitBot g_LegitBot;

//void LegitBot::StandaloneRCS(CUserCmd* cmd) {
//	static Vec3D vOldPunch(0, 0, 0);
//	if (Game::g_pLocal->iShotsFired() > 1) {
//		Vec3D vAimPunch = Game::g_pLocal->vGetAimPunchAngle() * (1 + (Variables::flCorrecting / 100.f));
//		Vec3D vRCS = cmd->viewangles - (vAimPunch - vOldPunch);
//		vRCS.Clamp();
//		vRCS.Normalize();
//		g_Interface.pEngine->SetViewAngles(vRCS);
//		vOldPunch = vAimPunch;
//	}
//	else vOldPunch.x = vOldPunch.y = vOldPunch.z = 0;
//}

void LegitBot::SmoothAim(Vec3D& vViewAngles, Vec3D& vAngle, float flSmoothingVal) {
	// clamp number
	float flSmoothing = g_Math.Clamp(flSmoothingVal, 100.f, 1.f);

	// adjust smoothing
	vAngle = (vViewAngles + (vAngle - vViewAngles).Clamped() / flSmoothing).Clamped();
}

float LegitBot::ApplyBezierValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals) {
	// get the delta angle and scale it into our spline range
	Vec3D vDelta = (vAngle - vViewAngles).Clamped();
	float flDeltaLength = vDelta.Length2D();
	int iScaledNum = (int)g_Math.ScaleNumber(flDeltaLength, Variables::flFov, 0.f, 99.f, 0.f);
	int iIndex = 99 - iScaledNum;

	// adjust smoothing value based on spline
	std::cout << pBezierVals[iIndex].y << std::endl;
	return Variables::flSmoothing * (2 - pBezierVals[iIndex].y);
}

bool LegitBot::NonSticky(Vec3D vViewAngles, Vec3D vEyeOrigin, Vec3D vAngle, Player* pTarget) {
	static float flCurTimePlayerReached = 0.f;
	static int iNonStickPlayerId = INVALID;

	// do trace ray from localplayers viewangles
	float c = std::cos(vViewAngles.x * M_PI / 180.f);
	Vec3D vForward = Vec3D(c * std::cos(vViewAngles.y * M_PI / 180.f), c * std::sin(vViewAngles.y * M_PI / 180.f), -std::sin(vViewAngles.x * M_PI / 180.f));
	vForward *= 8192.f; // max possible range

	Vec3D vEnd;
	vEnd = vEyeOrigin + vForward;

	trace_t tTrace;
	Ray_t rRay;
	CTraceFilter cFilter;
	cFilter.pSkip = Game::g_pLocal;
	rRay.Init(vEyeOrigin, vEnd);

	g_Interface.pEngineTrace->TraceRay(rRay, MASK_SHOT, &cFilter, &tTrace);

	// still looking at same player
	if (iNonStickPlayerId == pTarget->iIndex()) {
		// we have reached the desired player, free mouse, after wait 2.5s before locking again
		if (tTrace.m_pEnt == pTarget) {
			flCurTimePlayerReached = g_Interface.pGlobalVars->flCurTime;
			return true;
		}
		// dont update if we are on cooldown
		if (g_Interface.pGlobalVars->flCurTime - flCurTimePlayerReached <= 2.5f) {
			return true;
		}
	}

	// set new player ind
	else {
		if (tTrace.m_pEnt == pTarget) {
			iNonStickPlayerId = pTarget->iIndex();
		}
	}

	vAngle.Clamp();
	g_Interface.pEngine->SetViewAngles(vAngle);
	return true;
}

bool LegitBot::WaitAfterRetargetting(Vec3D vViewAngles, Vec3D vEyeOrigin, Player* pTarget) {
	if (Variables::bWaitAfterRetargetting) {
		static float flCurTimeAtTarget = 0.f;
		float c = std::cos(vViewAngles.x * M_PI / 180.f);
		Vec3D vForward = Vec3D(c * std::cos(vViewAngles.y * M_PI / 180.f), c * std::sin(vViewAngles.y * M_PI / 180.f), -std::sin(vViewAngles.x * M_PI / 180.f));
		vForward *= 8192.f; // yesyes ik magic number is ghetto ill fix it when i implemented weapons

		Vec3D vEnd;
		vEnd = vEyeOrigin + vForward;

		trace_t tTrace;
		Ray_t rRay;
		CTraceFilter cFilter;
		cFilter.pSkip = Game::g_pLocal;
		rRay.Init(vEyeOrigin, vEnd);

		g_Interface.pEngineTrace->TraceRay(rRay, MASK_SHOT, &cFilter, &tTrace);

		if (tTrace.m_pEnt == pTarget) {
			flCurTimeAtTarget = g_Interface.pGlobalVars->flCurTime;
		}

		// wait before targetting again at all
		if (g_Interface.pGlobalVars->flCurTime - flCurTimeAtTarget < Variables::flReactionTime) {
			return false;
		}
		return true;
	}
	return true;
}

AimPlayer LegitBot::GetClosestEnemyToCrosshair(LagRecord* pRecord, float flFov, Vec3D& vViewAngles, Vec3D vAimPunchAngle, Vec3D vEyeOrigin) {
	Player* pBestPlayer = nullptr;
	int iBestHitbox = Hitboxes::HitboxNone;
	float flBestDelta = flFov;

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		// get entity and validate it
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));
		if (!pPlayer || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal) || pPlayer->bDormant())
			continue;

		// calculate distance to enemy based on all hitboxes
		Vec3D vAngle;

		for (int j = 0; j < sizeof(Variables::iAllowedHitboxes) / sizeof(int); j++) {
			Vec3D vHitboxPos;
			if (pRecord) vHitboxPos = pRecord->GetHitboxPos(Variables::iAllowedHitboxes[j]);
			else vHitboxPos = pPlayer->vGetHitboxPos(Variables::iAllowedHitboxes[j]);

			// hitbox is not visible, try next one
			if (!pPlayer->bIsPointVisible(vEyeOrigin, vHitboxPos))
				continue;

			g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vAimPunchAngle - vViewAngles).Clamped().Length();
			if (flCurrDelta < flBestDelta) {
				flBestDelta = flCurrDelta;
				pBestPlayer = pPlayer;
				iBestHitbox = Variables::iAllowedHitboxes[j];
			}
		}
	}

	if (!pBestPlayer)
		return AimPlayer{ nullptr, Hitboxes::HitboxNone };

	return AimPlayer{ pBestPlayer, iBestHitbox, flBestDelta };
}

void LegitBot::CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunchAngle, float flCorrection) {
	vAngle.x -= vAimPunchAngle.x * (1.f + (flCorrection / 100.f));
	vAngle.y -= vAimPunchAngle.y * (1.f + (flCorrection / 100.f));
}

bool LegitBot::AimAtBestPlayer(LagRecord* pRecord) {
	if (!Variables::bAimbot || Game::g_pLocal->iTeamNum() == 1 || Game::g_pLocal->iTeamNum() == 0)
		return false;

	//if (!GetAsyncKeyState(VK_LBUTTON))
	//	return false;

	// get local viewangles
	Vec3D vViewAngles;
	g_Interface.pEngine->GetViewAngles(vViewAngles);

	// local eye pos
	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();

	// get local aimpunchangle
	Vec3D vAimPunchAngle = Game::g_pLocal->vGetAimPunchAngle();

	AimPlayer pAimPlayer = GetClosestEnemyToCrosshair(pRecord, Variables::flFov, vViewAngles, vAimPunchAngle, vEyeOrigin);
	Player* pPlayer = pAimPlayer.pPlayer;

	// we have not found a player return
	if (!pPlayer)
		return false;

	if (!WaitAfterRetargetting(vViewAngles, vEyeOrigin, pPlayer))
		return false;

	// calc viewangles to shoot at enemy
	Vec3D vAngle;
	Vec3D vHitboxPos = pRecord != nullptr ? pRecord->GetHitboxPos(pAimPlayer.iHitbox) : pPlayer->vGetHitboxPos(pAimPlayer.iHitbox);
	g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);

	if (!Variables::bStandaloneRCS)
		CompensateRecoil(vAngle, vAimPunchAngle, Variables::flCorrecting);

	float flAdjustedSmoothing = ApplyBezierValues(vViewAngles, vAngle, Variables::vAimbotCurve);
	SmoothAim(vViewAngles, vAngle, flAdjustedSmoothing);

	// if Non Sticky extra calculation
	if (Variables::bNonSticky)
		return NonSticky(vViewAngles, vEyeOrigin, vAngle, pPlayer);

	// Non Sticky is off just set the angles
	vAngle.Clamp();
	g_Interface.pEngine->SetViewAngles(vAngle);

	return true;
}