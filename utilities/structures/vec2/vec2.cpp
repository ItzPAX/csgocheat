#include "../../../includes.h"
#include "../../../pch/pch.h"

vec2::vec2() { x = 0; y = 0; }

vec2::vec2(float vx, float vy) { x = vx; y = vy; }

vec2::vec2(const vec2& v) { x = v.x; y = v.y; }

vec2 vec2::operator+(const vec2& vec) { return vec2(x + vec.x, y + vec.y); }

vec2& vec2::operator+=(const vec2& vec) {
	x += vec.x;
	y += vec.y;
	return *this;
}

vec2 vec2::operator-(const vec2& vec) { return vec2(x - vec.x, y - vec.y); }

vec2& vec2::operator-=(const vec2& vec) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

vec2 vec2::operator*(float val) { return vec2(x * val, y * val); }

vec2& vec2::operator*=(float val) {
	x *= val;
	y *= val;
	return *this;
}

vec2 vec2::operator/(float val) { return vec2(x / val, y / val); }

vec2& vec2::operator/=(float val) {
	x /= val;
	y /= val;
	return *this;
}

vec2& vec2::operator=(const vec2& vec) {
	x = vec.x;
	y = vec.y;
	return *this;
}
