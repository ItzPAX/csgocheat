#pragma once

#define M_PI 3.1415926535897932384f

class Math {
public:
	float Clamp(float tNum, float tNumMax, float tNumMin);
	float ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin);
};

extern Math g_Math;