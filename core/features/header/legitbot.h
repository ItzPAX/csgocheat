#pragma once
#include "pch.h"
#include "includes.h"

class LegitBot {
private:
	LagRecord* pTargetRecord;
	std::vector<int> vAllowedHitboxes;

	float ApplyBezierSmoothingValues(Vec3D& vViewAngles, Vec3D& vAngle, Vec2D* pBezierVals);
	void CompensateRecoil(Vec3D& vAngle, Vec3D vAimPunch, float flRCS, float flSmoothing);
	void UpdateHitboxes();

public:
	Vec2D vAimbotCurve[100];
	bool bGraphExtended = false;
	int iMenuWeapon = 0;
	int iActiveWeapon = 0;

	void GetTargetRecord(CUserCmd* cmd, LagRecord* pBestRecord);
	void RunTriggerbot(CUserCmd* cmd, LagRecord* pBestRecord);
	void RunAimbot(CUserCmd* cmd, LagRecord* pBestRecord);
};

extern LegitBot g_LegitBot;