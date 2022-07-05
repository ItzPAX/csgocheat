#pragma once
#include "pch.h"
#include "includes.h"

struct RecordHitbox {
	Vec3D vPos;
	bool bVisible;
};

struct LagRecord {
	Player* pTargetPlayer;
	Matrix boneMat[MAXSTUDIOBONES], tempMat[MAXSTUDIOBONES];
	Vec3D vAbsOrigin, vAbsAngles, vOrigin, vAngles;
	RecordHitbox rHitBoxPos[HITBOX_MAX];
	float flSimTime;

	// overload constructor
	LagRecord() = default;

	LagRecord(Player* pPlayer) {
		pTargetPlayer = pPlayer;
		pPlayer->SetupBones(boneMat, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);

		for (int i = 0; i < HITBOX_MAX; i++) {
			Vec3D vHitboxPos = pPlayer->vGetHitboxPos(i);
			rHitBoxPos[i] = { vHitboxPos, pPlayer->bIsPointVisible(Game::g_pLocal->vEyeOrigin(), vHitboxPos) };
		}

		vAbsOrigin = pPlayer->vAbsOrigin();
		vAbsAngles = pPlayer->vAbsAngles();

		vOrigin = pPlayer->vOrigin();
		vAngles = pPlayer->vEyeAngles();

		flSimTime = pPlayer->flSimTime();
	}

	RecordHitbox GetHitboxPos(int idx) {
		return this->rHitBoxPos[idx];
	}
	 
	void ApplyToPlayer(Player* pPlayer) {
		pPlayer->SetAngles(vAbsAngles);
		pPlayer->SetPosition(vAbsOrigin);

		pPlayer->vOrigin() = vOrigin;
		pPlayer->vEyeAngles() = vAngles;
		
		pPlayer->SetupBones(tempMat, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);
		memcpy(tempMat, boneMat, sizeof(Matrix) * MAXSTUDIOBONES);
	}
};

class Backtrack {
private:
	ConVar* pUpdateRate;
	ConVar* pMaxUpdateRate;
	ConVar* pMinUpdateRate;
	ConVar* pInterp;
	ConVar* pInterpRatio;
	ConVar* pMinInterpRatio;
	ConVar* pMaxInterpRatio;
	ConVar* pMaxUnlag;

public:
	std::deque<LagRecord> deqLagRecords[MAX_PLAYERS];

	bool ValidTick(LagRecord& pRecord);
	float GetLerpTime();
	void RecordData();
	void ApplyRecord(CUserCmd* cmd, LagRecord* record);

	__forceinline void Init() {
		deqLagRecords->clear();

		pUpdateRate = g_Interface.pICVar->FindVar(XOR("cl_updaterate"));
		pMaxUpdateRate = g_Interface.pICVar->FindVar(XOR("sv_maxupdaterate"));
		pMinUpdateRate = g_Interface.pICVar->FindVar(XOR("sv_minupdaterate"));
		pInterp = g_Interface.pICVar->FindVar(XOR("cl_interp"));
		pInterpRatio = g_Interface.pICVar->FindVar(XOR("cl_interp_ratio"));
		pMinInterpRatio = g_Interface.pICVar->FindVar(XOR("sv_client_min_interp_ratio"));
		pMaxInterpRatio = g_Interface.pICVar->FindVar(XOR("sv_client_max_interp_ratio"));
		pMaxUnlag = g_Interface.pICVar->FindVar(XOR("sv_maxunlag"));
	}
};

extern Backtrack g_Backtrack;