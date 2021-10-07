#pragma once

#define M_PI 3.1415926535897932384f

class Math {
public:
	template <typename T>
	T Clamp(T tNum, T tNumMax, T tNumMin);

	template <typename T>
	T ScaleNumber(T flVal, T flValMax, T flValMin, T flNewMax, T flNewMin);
};

extern Math g_Math;