#include "vec3d.h"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

Vec3D::Vec3D(void) {
	x = y = z = 0.0f;
}

Vec3D::Vec3D(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vec3D::~Vec3D(void) {};

void Vec3D::Init(float _x, float _y, float _z) {
	x = _x; y = _y; z = _z;
}

void Vec3D::Clamp(void) {
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
}

Vec3D Vec3D::Clamped() {
	Vec3D clamped = *this;
	clamped.Clamp();
	return clamped;
}

float Vec3D::DistanceTo(const Vec3D& other) {
	Vec3D delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.Length();
}

void Vec3D::Normalize() {
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

Vec3D Vec3D::Normalized(void) {
	Vec3D vec(*this);
	vec.Normalize();

	return vec;
}

float Vec3D::Length(void) {
	float root = 0.0f, sqsr = this->LengthSqr();

	__asm        sqrtss xmm0, sqsr
	__asm        movss root, xmm0

	return root;
}

float Vec3D::LengthSqr(void) {
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
	};

	return (sqr(x) + sqr(y) + sqr(z));
}

float Vec3D::Length2DSqr(void) const {
	return (x * x + y * y);
}

float Vec3D::Dot(const Vec3D other) {
	return (x * other.x + y * other.y + z * other.z);
}

float Vec3D::Dot(const float* other) {
	const Vec3D& a = *this;

	return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}