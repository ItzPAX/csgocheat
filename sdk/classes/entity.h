#pragma once
#include "includes.h"
#include "pch.h"

class Entity {
public:
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