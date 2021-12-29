#include "../header/aimbot.h"

LegitBot g_LegitBot;

AimPlayer LegitBot::GetClosestEnemyToCrosshair(float flFov, Vec3D& vViewAngles, Vec3D vAimPunchAngle, Vec3D vEyeOrigin) {
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
			Vec3D vHitboxPos = pPlayer->vGetHitboxPos(Variables::iAllowedHitboxes[j]);

			trace_t tTraceRes;

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

	// distance based fov shitttttssssssss
	if (!pBestPlayer)
		return AimPlayer{ nullptr, Hitboxes::HitboxNone };

	if (Variables::bDistanceBasedFov) {
		float flDistanceBestDelta = DistanceAdjustedFOV(flFov, vEyeOrigin, pBestPlayer->vEyeOrigin(), pBestPlayer);
		if (flDistanceBestDelta > flBestDelta)
			return AimPlayer{ pBestPlayer, iBestHitbox };
		else
			return AimPlayer{ nullptr, Hitboxes::HitboxNone };
	}

	return AimPlayer{ pBestPlayer, iBestHitbox };
}

void LegitBot::SmoothAim(Vec3D& vViewAngles, Vec3D& vAngle, float flSmoothingVal) {
	// no division by zero
	if (flSmoothingVal == 0.f)
		return;

	float flDist = (vViewAngles - vAngle).Normalized().Length2D() / flSmoothingVal;

	// adjust smoothing
	vAngle = (vViewAngles + (vAngle - vViewAngles).Clamped() / (flSmoothingVal / flDist)).Clamped();
}

void LegitBot::CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunchAngle, float flCorrection) {
	vAngle.x -= vAimPunchAngle.x * (1.f + (flCorrection / 100.f));
	vAngle.y -= vAimPunchAngle.y * (1.f + (flCorrection / 100.f));
}

bool LegitBot::AimAtBestPlayer() {
	if (!Variables::bAimbot)
		return false;

	if (!GetAsyncKeyState(VK_LBUTTON))
		return false;

	// get local viewangles
	Vec3D vViewAngles;
	g_Interface.pEngine->GetViewAngles(vViewAngles);

	// local eye pos
	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();

	// get local aimpunchangle
	Vec3D vAimPunchAngle = Game::g_pLocal->vGetAimPunchAngle();

	AimPlayer pAimPlayer = GetClosestEnemyToCrosshair(Variables::flFov, vViewAngles, vAimPunchAngle, vEyeOrigin);
	Player* pPlayer = pAimPlayer.pPlayer;

	// we have not found a player return
	if (!pPlayer)
		return false;

	// do trace ray from localplayers viewangles to wait after enemy has been targeted
	static float flCurTimeAtTarget = 0.f;

	if (Variables::bWaitAfterTargetting) {
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

		if (tTrace.m_pEnt == pPlayer) {
			flCurTimeAtTarget = g_Interface.pGlobalVars->flCurTime;
		}

		// wait before targetting again at all
		if (g_Interface.pGlobalVars->flCurTime - flCurTimeAtTarget < Variables::flReactionTime) {
			return false;
		}
	}

	// calc viewangles to shoot at enemy
	Vec3D vAngle;
	g_Math.CalcAngle(vEyeOrigin, pPlayer->vGetHitboxPos(pAimPlayer.iHitbox), vAngle);

	CompensateRecoil(vAngle, vAimPunchAngle, Variables::flCorrecting);
	SmoothAim(vViewAngles, vAngle, Variables::flSmoothing);

	// if Non Sticky extra calculation
	if (Variables::bNonSticky)
		return NonSticky(vViewAngles, vEyeOrigin, vAngle, pPlayer);

	// Non Sticky is off just set the angles
	vAngle.Clamp();
	g_Interface.pEngine->SetViewAngles(vAngle);

	return true;
}

bool LegitBot::NonSticky(Vec3D vViewAngles, Vec3D vEyeOrigin, Vec3D vAngle, Player* pTarget) {
	static float flCurTimePlayerReached = 0.f; 
	static int iNonStickPlayerId = INVALID;

	// do trace ray from localplayers viewangles
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

float LegitBot::DistanceAdjustedFOV(float flBaseFov, Vec3D vEyeOrigin, Vec3D vTargetPos, Player* pTarget) {
	trace_t tTrace;
	// skip local player
	CTraceFilter cTraceFilter;
	cTraceFilter.pSkip = Game::g_pLocal;

	Ray_t ray;
	ray.Init(vEyeOrigin, vTargetPos);

	g_Interface.pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &cTraceFilter, &tTrace);

	// get distance
	float flDist = sqrtf(sqrtf((tTrace.endpos - tTrace.startpos).Length())) - 2.f;

	// cant divide by 0
	if (flDist == NULL)
		return NULL;

	float flAdjustedFOV = flBaseFov / flDist;

	return flAdjustedFOV;
}