#pragma once

#include "../vec3d/vec3d.h"

class Vec2D {
public:
	float x, y;

	Vec2D() {
		x = 0; y = 0;
	};
	Vec2D(float _x, float _y) {
		x = _x; y = _y;
	};
	Vec2D(Vec3D vec) {
		x = vec.x; y = vec.y;
	}

	inline Vec2D operator*(const float n) const {
		return Vec2D(x * n, y * n);
	}
	inline Vec2D operator+(const Vec2D& v) const {
		return Vec2D(x + v.x, y + v.y);
	}
	inline Vec2D operator-(const Vec2D& v) const {
		return Vec2D(x - v.x, y - v.y);
	}
	inline void operator+=(const Vec2D& v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const Vec2D& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const Vec2D& v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const Vec2D& v) const {
		return (v.x != x || v.y != y);
	}

	inline float Length() {
		return sqrt((x * x) + (y * y));
	}
};