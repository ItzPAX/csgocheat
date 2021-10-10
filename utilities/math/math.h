#pragma once

// Math defines
#define M_PI 3.1415926535897932384
#define M_PI2 M_PI * 2
#define DEG2RAD(x) x * M_PI/180.f
#define RAD2DEG(x) x * 180.f/M_PI

class Math {
public:
	float Clamp(float tNum, float tNumMax, float tNumMin);
	void Clamp(float* tNum, float tNumMax, float tNumMin);

	float NormalizeIntoRange(float flVal, float flMax, float flMin);
	float ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin);

	void WorldToScreen(Vec3D world, Vec2D& screen);
};

extern Math g_Math;