#pragma once
#include "pch.h"
#include "includes.h"

struct LagRecord;

struct AimPlayer {
	Player* pPlayer;
	int iHitbox;
	float flFov;
};

class LegitBot {
private:
	bool NonSticky(Vec3D vViewAngles, Vec3D vEyeOrigin, Vec3D vAngle, Player* pTarget);
	bool WaitAfterRetargetting(Vec3D vViewAngles, Vec3D vEyeOrigin, Player* pTarget);
	void CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunchAngle, float flCorrection);
	void SmoothAim(Vec3D& vViewAngles, Vec3D& vAngle, float flSmoothingVal);
	float ApplyBezierValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals);
	AimPlayer GetClosestEnemyToCrosshair(LagRecord* pRecord, float flFov, Vec3D& vViewAngles, Vec3D vAimPunchAngle, Vec3D vEyeOrigin);
public:
	bool AimAtBestPlayer(LagRecord* pRecord);

};

extern LegitBot g_LegitBot;