#pragma once
#include "includes.h"
#include "pch.h"

class Entity {
private:
	// netvar shits
	int m_iTeamNum = g_NetVars.GetNetvar<int>("DT_CSPlayer", "m_iTeamNum", this);
	Vec3D m_vecOrigin = g_NetVars.GetNetvar<Vec3D>("DT_BasePlayer", "m_vecOrigin", this);

public:
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }
	bool bDormant() { using tOriginalFn = bool(__thiscall*)(void*); return (*static_cast<tOriginalFn**>(pNetworkable()))[9](pNetworkable()); }

	bool bIsValid() { return this != nullptr && this->pNetworkable(); };

	Vec3D vecOrigin() { return m_vecOrigin; }
	int iTeamNum() { return m_iTeamNum; };
};

class Player : public Entity {
private:
	// netvar shits
	int m_iHealth = g_NetVars.GetNetvar<int>("DT_BasePlayer", "m_iHealth", this);
	Vec3D m_vEyeAngles = g_NetVars.GetNetvar<Vec3D>("DT_CSPlayer", "m_angEyeAngles", this);

public:
	int iHealth() { return m_iHealth; };
	Vec3D vEyeAngles() { return m_vEyeAngles; }
};