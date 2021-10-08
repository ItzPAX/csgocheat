#include "../../includes.h"

Math g_Math;

float Math::Clamp(float tNum, float tNumMax, float tNumMin) {
    if (tNum >= tNumMax)
        return tNumMax;
    else if (tNum <= tNumMin)
        return tNumMin;
    else
        return tNum;
}

float Math::ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin) {
    float flValToScale = g_Math.Clamp(flVal, flValMax, flValMin);
    float flValPrc = (flValToScale - flValMin) / (flValMax - flValMin);
    return flNewMin + flValPrc * (flNewMax - flNewMin);
}
