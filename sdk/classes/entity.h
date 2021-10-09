#pragma once
#include "includes.h"
#include "pch.h"

class Entity {
public:
};

class Player : public Entity {
public:
	int iHealth() { return g_NetVars.GetNetvar<int>("DT_BasePlayer", "m_iHealth", this); };
	Vec3D vEyeAngles() { return g_NetVars.GetNetvar<Vec3D>("DT_CSPlayer", "m_angEyeAngles", this); }
};