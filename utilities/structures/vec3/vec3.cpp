#include "../../../includes.h"
#include "../../../pch/pch.h"

// Constructors
vec3::vec3() { x = 0; y = 0; z = 0; }
vec3::vec3(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }
vec3::vec3(const vec3& v) { x = v.x; y = v.y; z = v.z; }

// operators
vec3 vec3::operator+(const vec3& vec) { return vec3(x + vec.x, y + vec.y, z + vec.z); }

vec3& vec3::operator+=(const vec3& vec) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

vec3 vec3::operator-(const vec3& vec) { return vec3(x - vec.x, y - vec.y, z - vec.z); }

vec3& vec3::operator-=(const vec3& vec) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

vec3 vec3::operator*(float val) { return vec3(x * val, y * val, z * val); }

vec3& vec3::operator*=(float val) {
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

vec3 vec3::operator/(float val) { return vec3(x / val, y / val, z / val); }

vec3& vec3::operator/=(float val) {
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

vec3& vec3::operator=(const vec3& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

// functions