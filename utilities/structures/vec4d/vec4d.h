#include "includes.h"

struct Vec4D {
public:
	float x, y, z, w;

public:
	// constructors, cuz i cant cba to do the rest of the struct
	Vec4D() { x = y = z = w = 0.f; }
	Vec4D(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
	Vec4D(const Vec4D& vec) { x = vec.x; y = vec.y; z = vec.z; w = vec.w; }
};