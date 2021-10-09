#include "pch.h"
#include "includes.h"

// constructor
Vec3D::Vec3D(const Vec3D& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

//addition
Vec3D Vec3D ::operator+(const Vec3D& vec) {
    return Vec3D(x + vec.x, y + vec.y, z + vec.z);
}

Vec3D& Vec3D ::operator+=(const Vec3D& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

//substraction//
Vec3D Vec3D ::operator-(const Vec3D& vec) {
    return Vec3D(x - vec.x, y - vec.y, z - vec.z);
}

Vec3D& Vec3D::operator-=(const Vec3D& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

//scalar multiplication
Vec3D Vec3D::operator*(float value) {
    return Vec3D(x * value, y * value, z * value);
}

Vec3D& Vec3D::operator*=(float value) {
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

//scalar division
Vec3D Vec3D ::operator/(float value) {
    return Vec3D(x / value, y / value, z / value);
}

Vec3D& Vec3D ::operator/=(float value) {
    x /= value;
    y /= value;
    z /= value;
    return *this;
}

Vec3D& Vec3D::operator=(const Vec3D& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
}

//Dot product
float Vec3D::DotProduct(const Vec3D& vec) {
    return x * vec.x + vec.y * y + vec.z * z;
}

//cross product
Vec3D Vec3D::CrossProduct(const Vec3D& vec)
{
    float ni = y * vec.z - z * vec.y;
    float nj = z * vec.x - x * vec.z;
    float nk = x * vec.y - y * vec.x;
    return Vec3D(ni, nj, nk);
}

float Vec3D::Magnitude() {
    return sqrt(Square());
}

float Vec3D::Square() {
    return x * x + y * y + z * z;
}

Vec3D Vec3D::Normalization() {
    *this /= Magnitude();
    return *this;
}

Vec3D Vec3D::Normalized(const Vec3D& vec) {
    g_Math.NormalizeIntoRange(vec.x, 89.f, -89.f);
    g_Math.NormalizeIntoRange(vec.y, 180.f, -180.f);
    // dont normalize z axis
}

void Vec3D::Normalize() {
    g_Math.NormalizeIntoRange(this->x, 89.f, -89.f);
    g_Math.NormalizeIntoRange(this->y, 180.f, -180.f);
    // dont normalize z axis
}

float Vec3D::Distance(const Vec3D& vec) {
    Vec3D dist = *this - vec;
    return dist.Magnitude();
}