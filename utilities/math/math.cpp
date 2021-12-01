#include "pch.h"
#include "includes.h"

Math g_Math{ };

float Math::Clamp(float tNum, float tNumMax, float tNumMin) {
    if (tNum >= tNumMax)
        return tNumMax;
    if (tNum <= tNumMin)
        return tNumMin;
    return tNum;
}

void Math::Clamp(float* tNum, float tNumMax, float tNumMin) {
    if (!tNum)
        return;

    if (*tNum >= tNumMax)
        *tNum = tNumMax;
    if (*tNum <= tNumMin)
        *tNum = tNumMin;
}

float Math::NormalizeIntoRange(float flVal, float flMax, float flMin) {
    return (flVal - flMin) / (flMax - flMin);
}

float Math::ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin) {
    float flValToScale = g_Math.Clamp(flVal, flValMax, flValMin);
    float flValPrc = (flValToScale - flValMin) / (flValMax - flValMin);
    return flNewMin + flValPrc * (flNewMax - flNewMin);
}

void Math::CalcAngle(Vec3D src, Vec3D dst, Vec3D& angles) {
    // get target Vector
    Vec3D vTargetVec = dst - src;
	VectorAngles(vTargetVec, angles);
}

void Math::VectorAngles(Vec3D& forward, Vec3D angles) {
	Vec3D view;
	float flTmp;

	if (forward[1] == 0.f && forward[0] == 0.f) {
		view[0] = 0.f;
		view[1] = 0.f;
	}
	else {
		view[1] = RAD2DEG(atan2(forward[1], forward[0]));

		if (view[1] < 0.f)
			view[1] += 360.f;

		flTmp = forward[0] * forward[0] + forward[1] * forward[1];
		view[2] = sqrt(flTmp);

		//view[2] = 1/fastInverseSquare(forward[0] * forward[0] + forward[1] * forward[1]);

		view[0] = RAD2DEG(atan2(forward[2], view[2]));
	}

	angles[0] = -view[0];
	angles[1] = view[1];
	angles[2] = 0.f;
}