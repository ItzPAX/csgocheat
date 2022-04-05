#pragma once
#include "pch.h"
#include "includes.h"

struct LagRecord;

struct AimPlayer {
	Player* pPlayer;
	Vec3D vExtendedHitboxPos;
	Vec3D vActualHitboxPos;
	float flFov;
};

class LegitBot {
private:
	float flAdjustedSmoothing = Variables::flSmoothing;
private:
	bool NonSticky(Vec3D vViewAngles, Vec3D vEyeOrigin, Vec3D vAngle, Player* pTarget);
	bool WaitAfterRetargetting(Vec3D vViewAngles, Vec3D vEyeOrigin, Player* pTarget);
	bool GetExtendedHitboxPos(Player* pPlayer, LagRecord* pRecord, int iHitbox, Vec3D& vNewPoint);
	void CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunchAngle, float flCorrection);
	void SmoothAim(Vec3D& vViewAngles, Vec3D& vAngle, float flSmoothingVal);
	float ApplyBezierSmoothingValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals);
	AimPlayer GetClosestEnemyToCrosshair(LagRecord* pRecord, float flFov, Vec3D& vViewAngles, Vec3D vAimPunchAngle, Vec3D vEyeOrigin);
public:
	bool AimAtBestPlayer(LagRecord* pRecord);

};

extern LegitBot g_LegitBot;