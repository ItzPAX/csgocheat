#pragma once

#include "../vec3d/vec3d.h"

class Vec4D {
public:
	float x, y, z, w;

	Vec4D() {
		x = 0; y = 0; z = 0; w = 0;
	};
	Vec4D(float _x, float _y, float _z, float _w) {
		x = _x; y = _y; z = _z; w = _w;
	};
	Vec4D(Vec3D vec) {
		x = vec.x; y = vec.y; z = vec.z;
	}

	inline Vec4D operator*(const float n) const {
		return Vec4D(x * n, y * n, z * n, w * n);
	}
	inline Vec4D operator+(const Vec4D& v) const {
		return Vec4D(x + v.x, y + v.y, z + v.z, w + v.w );
	}
	inline Vec4D operator-(const Vec4D& v) const {
		return Vec4D(x - v.x, y - v.y, z + v.z, w + v.w);
	}
	inline void operator+=(const Vec4D& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	inline void operator-=(const Vec4D& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}

	bool operator==(const Vec4D& v) const {
		return (v.x == x && v.y == y && v.z == z && v.w == w);
	}
	bool operator!=(const Vec4D& v) const {
		return (v.x != x || v.y != y || v.z != z || v.w != w);
	}

	inline float Length() {
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}
};