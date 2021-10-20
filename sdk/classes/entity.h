#pragma once
#include "utilities/structures/matrix/matrix.h"
#include "includes.h"
#include "pch.h"

#define MAXSTUDIOBONES		128
#define BONE_USED_BY_HITBOX 0x00000100

class Entity {
public:
	void* Animating() { return reinterpret_cast<void*>(uintptr_t(this) + 0x4); }
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }

	bool SetupBones(Matrix* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, Matrix*, int, int, float);
		return (*(original_fn**)Animating())[13](Animating(), out, max_bones, mask, time);
	}

	int iIndex() {
		using original_fn = int(__thiscall*)(void*);

		if (this && this->pNetworkable())
			return (*(original_fn**)pNetworkable())[10](pNetworkable());
	}

	bool bDormant() { using tOriginalFn = bool(__thiscall*)(void*); return (*static_cast<tOriginalFn**>(pNetworkable()))[9](pNetworkable()); }
	Vec3D vOrigin() { return g_NetVars.GetNetvar<Vec3D>("DT_BasePlayer", "m_vecOrigin", this); }
	int iTeamNum() { return g_NetVars.GetNetvar<int>("DT_CSPlayer", "m_iTeamNum", this); };
};

class Player : public Entity {
public:
	Vec3D GetBonePosition(int bone) {
		Matrix BoneMatrix[MAXSTUDIOBONES];
		if (SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
			return Vec3D{ BoneMatrix[bone][0][3], BoneMatrix[bone][1][3], BoneMatrix[bone][2][3] };
		else
			return Vec3D();
	}

	Vec3D& vAbsOrigin() {
		using original_fn = Vec3D& (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);
	}

	bool bIsEnemy(Player* to) { return this->iTeamNum() != to->iTeamNum(); }
	int iHealth() { return g_NetVars.GetNetvar<int>("DT_BasePlayer", "m_iHealth", this); }
	Vec3D vEyeAngles() { return g_NetVars.GetNetvar<Vec3D>("DT_CSPlayer", "m_angEyeAngles", this); }
	bool bIsAlive() { return iHealth() > 0; }
};