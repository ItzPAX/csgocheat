#pragma once
#include "utilities/structures/matrix/matrix.h"
#include "utilities/tools/xorstr.h"
#include "utilities/tools/tools.h"
#include "includes.h"
#include "pch.h"

#define INVALID -1

enum entity_flags {
	FL_ONGROUND= (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8),
	FL_INWATER = (1 << 9),
	FL_FLY = (1 << 10),
	FL_SWIM = (1 << 11),
	FL_CONVEYOR = (1 << 12),
	FL_NPC = (1 << 13),
	FL_GODMODE = (1 << 14),
	FL_NOTARGET = (1 << 15),
	FL_AIMTARGET = (1 << 16),
	FL_PARTIALGROUND = (1 << 17),
	FL_STATICPROP = (1 << 18),
	FL_GRAPHED = (1 << 19),
	FL_GRENADE = (1 << 20),
	FL_STEPMOVEMENT = (1 << 21),
	FL_DONTTOUCH = (1 << 22),
	FL_BASEVELOCITY = (1 << 23),
	FL_WORLDBRUSH = (1 << 24),
	FL_OBJECT = (1 << 25),
	FL_KILLME = (1 << 26),
	FL_ONFIRE = (1 << 27),
	FL_DISSOLVING = (1 << 28),
	FL_TRANSRAGDOLL = (1 << 29),
	FL_UNBLOCKABLEBYPLAYER = (1 << 30)
};

class Entity {
public:
	void* pRenderable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x4); }
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }

	int iIndex() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)pNetworkable())[10](pNetworkable());
	}

	bool bIsPlayer() {
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[158](this);
	}

	bool bIsBaseCombatWeapon() {
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[165](this);
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

	void UpdateClientSideAnimations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
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

	Vec3D& vAbsAngles() {
		using original_fn = Vec3D & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);
	}

	Vec3D vEyeOrigin() {
		Vec3D vEyeOrig;
		using original_fn = void(__thiscall*)(void*, Vec3D&);
		(*(original_fn**)this)[169](this, vEyeOrig);
		return vEyeOrig;
	}

	void SetAngles(Vec3D vAngles) {
		using original_fn = void(__thiscall*)(void*, const Vec3D&);
		static original_fn set_angles_fn = (original_fn)((DWORD)g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1"), XOR("xxxxxxxxxxxxxx")));
		set_angles_fn(this, vAngles);
	}
	void SetPosition(Vec3D vPosition) {
		using original_fn = void(__thiscall*)(void*, const Vec3D&);
		static original_fn set_position_fn = (original_fn)((DWORD)g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8"),XOR("xxxxxxxxxxxxx")));
		set_position_fn(this, vPosition);
	}

	bool bIsEnemy(Player* to) { return this->iTeamNum() != to->iTeamNum(); }

	bool bIsAlive() { 
		if (NetvarOffsets::iHealth == 0)
			return iHealth() > 0;

		return *reinterpret_cast<int*>(uintptr_t(this) + NetvarOffsets::iHealth) > 0;
		//return iHealth() > 0; 
	}

	Vec3D vGetVelocity() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecVelocity[0]"), this); }
	Vec3D vGetViewOffset() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecViewOffset[0]"), this); }
	Vec3D vGetAimPunchAngle() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_aimPunchAngle"), this); }
	float flSimTime() { return g_NetVars.GetNetvar<float>(XOR("DT_CSPlayer"), XOR("m_flSimulationTime"), this); }
	int iGetHitboxSet() { return g_NetVars.GetNetvar<int>(XOR("DT_BasePlayer"), XOR("m_nHitboxSet"), this); }
	int iHealth() { return g_NetVars.GetNetvar<int>(XOR("DT_BasePlayer"), XOR("m_iHealth"), this); }
	Vec3D vEyeAngles() { return g_NetVars.GetNetvar<Vec3D>(XOR("DT_CSPlayer"), XOR("m_angEyeAngles"), this); }
	int iShotsFired() { return g_NetVars.GetNetvar<int>(XOR("DT_CSPlayer"), XOR("m_iShotsFired"), this); }
	int iFlags() { return g_NetVars.GetNetvar<int>(XOR("DT_CSPlayer"), XOR("m_fFlags"), this); }

	float flOldSimTime() {
		return this->flSimTime() + 0x4;
	}
};