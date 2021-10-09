#include "pch.h"
#include "includes.h"

// constructor
Vec2D::Vec2D(const Vec2D& vec) {
    x = vec.x;
    y = vec.y;
}

//addition
Vec2D Vec2D ::operator+(const Vec2D& vec) {
    return Vec2D(x + vec.x, y + vec.y);
}

Vec2D& Vec2D ::operator+=(const Vec2D& vec) {
    x += vec.x;
    y += vec.y;
    return *this;
}

//substraction
Vec2D Vec2D ::operator-(const Vec2D& vec) {
    return Vec2D(x - vec.x, y - vec.y);
}

Vec2D& Vec2D::operator-=(const Vec2D& vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
}

//scalar multiplication
Vec2D Vec2D::operator*(float value) {
    return Vec2D(x * value, y * value);
}

Vec2D& Vec2D::operator*=(float value) {
    x *= value;
    y *= value;
    return *this;
}

//scalar division
Vec2D Vec2D ::operator/(float value) {
    return Vec2D(x / value, y / value);
}

Vec2D& Vec2D ::operator/=(float value) {
    x /= value;
    y /= value;
    return *this;
}

Vec2D& Vec2D::operator=(const Vec2D& vec) {
    x = vec.x;
    y = vec.y;
    return *this;
}

//Dot product
float Vec2D::DotProduct(const Vec2D& vec) {
    return x * vec.x + vec.y * y;
}

//cross product
Vec2D Vec2D::CrossProduct(const Vec2D& vec)
{
    float ni = y * vec.y;
    float nj = vec.x - x;
    return Vec2D(ni, nj);
}

float Vec2D::Magnitude() {
    return sqrt(Square());
}

float Vec2D::Square() {
    return x * x + y * y;
}

Vec2D Vec2D::Normalization() {
    *this /= Magnitude();
    return *this;
}

Vec2D Vec2D::Normalized(const Vec2D& vec) {
    g_Math.NormalizeIntoRange(vec.x, 89.f, -89.f);
    g_Math.NormalizeIntoRange(vec.y, 180.f, -180.f);
}

void Vec2D::Normalize() {
    g_Math.NormalizeIntoRange(this->x, 89.f, -89.f);
    g_Math.NormalizeIntoRange(this->y, 180.f, -180.f);
}

float Vec2D::Distance(const Vec2D& vec) {
    Vec2D dist = *this - vec;
    return dist.Magnitude();
}