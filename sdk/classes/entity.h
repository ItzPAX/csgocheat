#pragma once
#include "utilities/structures/matrix/matrix.h"
#include "utilities/tools/xorstr.h"
#include "includes.h"
#include "pch.h"

#define INVALID -1;

class Entity {
public:
	void* pRenderable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x4); }
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }

	int iIndex() {
		using original_fn = int(__thiscall*)(void*);

		if (this && this->pNetworkable())
			return (*(original_fn**)pNetworkable())[10](pNetworkable());
		return INVALID;
	}

	ClientClass* cGetClientClass() {
		using original_fn = ClientClass*(__thiscall*)(void*);

		if (this && this->pNetworkable())
			return (*(original_fn**)pNetworkable())[2](pNetworkable());
		return nullptr;
	}

	Model* mGetModel() {
		using original_fn = Model* (__thiscall*)(void*);
		return (*(original_fn**)pRenderable())[8](pRenderable());
	}

	bool bIsPointVisible(Vec3D vFrom, Vec3D vPoint) {
		if (!Game::g_pLocal)
			return false;

		trace_t tTrace;
		// skip local player
		CTraceFilter cTraceFilter;
		cTraceFilter.pSkip = Game::g_pLocal;

		Ray_t ray;
		ray.Init(vFrom, vPoint);

		g_Interface.pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &cTraceFilter, &tTrace);

		return tTrace.m_pEnt == this;
	}

	bool bDormant() { using tOriginalFn = bool(__thiscall*)(void*); return (*static_cast<tOriginalFn**>(pNetworkable()))[9](pNetworkable()); }
	Vec3D vOrigin() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecOrigin"), this); }
	int iTeamNum() { return g_NetVars.GetNetvar<int>(XOR("DT_CSPlayer"), XOR("m_iTeamNum"), this); };
};

class Player : public Entity {
public:
	bool SetupBones(Matrix* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, Matrix*, int, int, float);
		return (*(original_fn**)pRenderable())[13](pRenderable(), out, max_bones, mask, time);
	}

	Vec3D vGetBonePosition(int bone) {
		Matrix BoneMatrix[MAXSTUDIOBONES];
		if (SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_Interface.pGlobalVars->flCurTime))
			return Vec3D{ BoneMatrix[bone][0][3], BoneMatrix[bone][1][3], BoneMatrix[bone][2][3] };
		else
			return Vec3D();
	}

	Vec3D vGetHitboxPos(int hitbox) {
		Matrix BoneMatrix[MAXSTUDIOBONES];
		if (!SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_Interface.pGlobalVars->flCurTime))
			return Vec3D();

		StudioHDR* pStudioModel = g_Interface.pModelInfo->GetStudioModel(mGetModel());
		if (!pStudioModel)
			return Vec3D();

		StudioBox* pHitbox = pStudioModel->Hitbox(hitbox, iGetHitboxSet());
		if (!pHitbox)
			return Vec3D();

		Vec3D vMin{}, vMax{};
		g_Math.TransformVector(pHitbox->vMins, BoneMatrix[pHitbox->iBone], vMin);
		g_Math.TransformVector(pHitbox->vMaxs, BoneMatrix[pHitbox->iBone], vMax);

		return Vec3D((vMin.x + vMax.x) * 0.5f, (vMin.y + vMax.y) * 0.5f, (vMin.z + vMax.z) * 0.5f);
	}

	Vec3D& vAbsOrigin() {
		using original_fn = Vec3D& (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);
	}

	Vec3D vEyeOrigin() {
		Vec3D vEyeOrig;
		using original_fn = void(__thiscall*)(void*, Vec3D&);
		(*(original_fn**)this)[169](this, vEyeOrig);
		return vEyeOrig;
	}

	bool bIsEnemy(Player* to) { return this->iTeamNum() != to->iTeamNum(); }
	bool bIsAlive() { return iHealth() > 0; }

	Vec3D vGetVelocity() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecVelocity[0]"), this); }
	Vec3D vGetViewOffset() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecViewOffset[0]"), this); }
	Vec3D vGetAimPunchAngle() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_aimPunchAngle"), this); }
	int iGetHitboxSet() { return g_NetVars.GetNetvar<int>(XOR("DT_BasePlayer"), XOR("m_nHitboxSet"), this); }
	int iHealth() { return g_NetVars.GetNetvar<int>(XOR("DT_BasePlayer"), XOR("m_iHealth"), this); }
	Vec3D vEyeAngles() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_CSPlayer"), XOR("m_angEyeAngles"), this); }
};