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