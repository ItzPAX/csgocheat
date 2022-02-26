#pragma once
#include "pch.h"
#include "includes.h"

struct AimPlayer {
	Player* pPlayer;
	int iHitbox;
	float flFov;
};

class LegitBot {
public:
	AimPlayer GetClosestEnemyToCrosshair(float flFov, Vec3D& vViewAngles, Vec3D vAimPunchAngle, Vec3D vEyeOrigin);
	bool AimAtBestPlayer();

	bool NonSticky(Vec3D vViewAngles, Vec3D vEyeOrigin, Vec3D vAngle, Player* pTarget);
	float DistanceAdjustedFOV(float flBaseFov, Vec3D vEyeOrigin, Vec3D vTargetPos, Player* pTarget);

	/*TODO: Make this more human like (Add some randomness to it etc.)*/
	void SmoothAim(Vec3D& vViewAngles, Vec3D& vAngle, float flSmoothingVal = 10.f, float flFov = Variables::flFov);
	/*TODO: Make this more human like (Add some randomness to it etc.)*/
	void CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunchAngle, float flCorrection = 100.f);
	void StandaloneRCS(CUserCmd* cmd);
};

extern LegitBot g_LegitBot;