#pragma once
#include "includes.h"
#include "pch.h"

class Entity {
private:

public:
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }
	bool bDormant() { using tOriginalFn = bool(__thiscall*)(void*); return (*static_cast<tOriginalFn**>(pNetworkable()))[9](pNetworkable()); }

	bool bIsValid() { return this != nullptr && this->pNetworkable(); };

	Vec3D vecOrigin() { return g_NetVars.GetNetvar<Vec3D>("DT_BasePlayer", "m_vecOrigin", this); }
	int iTeamNum() { return g_NetVars.GetNetvar<int>("DT_CSPlayer", "m_iTeamNum", this); };
};

class Player : public Entity {
private:

public:
	int iHealth() { return g_NetVars.GetNetvar<int>("DT_BasePlayer", "m_iHealth", this); }
	Vec3D vEyeAngles() { return g_NetVars.GetNetvar<Vec3D>("DT_CSPlayer", "m_angEyeAngles", this); }
};