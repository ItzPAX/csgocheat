#include "../../includes.h"
#include "../../pch/pch.h"

Math g_Math;

template<typename T>
inline T Math::Clamp(T tNum, T tNumMax, T tNumMin) {
    if (tNum >= tNumMax)
        return tNumMax;
    else if (tNum <= tNumMin)
        return tNumMin;
    else
        return tNum;
}

template<typename T>
T Math::ScaleNumber(T flVal, T flValMax, T flValMin, T flNewMax, T flNewMin) {
    float flValToScale = g_Math.Clamp(flVal, flValMax, flValMin);
    float flValPrc = (flValToScale - flValMin) / (flValMax - flValMin);
    return flNewMin + flValPrc * (flNewMax - flNewMin);
}
