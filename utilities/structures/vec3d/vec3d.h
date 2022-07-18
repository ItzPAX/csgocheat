#pragma once
#pragma warning( disable : 26451 )
#include <limits>
#include <algorithm>

inline float BitsToFloat(std::uint32_t i) {
	union convertor_t {
		float f; unsigned long ul;
	} tmp;

	tmp.ul = i;
	return tmp.f;
}

constexpr double M_PI = 3.14159265358979323846;
constexpr float M_RADPI = 57.295779513082f;
constexpr float M_PI_F = static_cast<float>(M_PI);

constexpr float RAD2DEG(const float x) {
	return (float)(x) * (float)(180.f / M_PI_F);
}
constexpr float DEG2RAD(const float x) {
	return (float)(x) * (float)(M_PI_F / 180.f);
}

constexpr std::uint32_t FLOAT32_NAN_BITS = 0x7FC00000;
const float FLOAT32_NAN = BitsToFloat(FLOAT32_NAN_BITS);
#define VEC_T_NAN FLOAT32_NAN
#define ASSERT( _exp ) ( (void ) 0 )

class Vec3D {
public:
	Vec3D();
	Vec3D(float, float, float);
	~Vec3D();

	float x, y, z;

	bool operator==(const Vec3D& v) {
		return x == v.x && y == v.y && z == v.z;
	}
	bool operator>(const Vec3D& v) {
		return x > v.x && y > v.y && z > v.z;
	}
	bool operator<(const Vec3D& v) {
		return x < v.x && y < v.y && z < v.z;
	}

	Vec3D& operator+=(const Vec3D& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	Vec3D& operator-=(const Vec3D& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	Vec3D& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	Vec3D operator+(const Vec3D& v) {
		return Vec3D{ x + v.x, y + v.y, z + v.z };
	}
	Vec3D operator-(const Vec3D& v) {
		return Vec3D{ x - v.x, y - v.y, z - v.z };
	}
	Vec3D operator*(float fl) const {
		return Vec3D(x * fl, y * fl, z * fl);
	}
	Vec3D operator*(const Vec3D& v) const {
		return Vec3D(x * v.x, y * v.y, z * v.z);
	}
	Vec3D& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	auto operator-(const Vec3D& other) const -> Vec3D {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/(float other) const {
		Vec3D vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length2D() const {
		return sqrt((x * x) + (y * y));
	}
	void Crossproduct(Vec3D v1, Vec3D v2, Vec3D cross_p) const {
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}

	float NormalizeInPlace()
	{
		const float flLength = this->Length();
		const float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

		this->x *= flRadius;
		this->y *= flRadius;
		this->z *= flRadius;

		return flLength;
	}

	bool IsNull() {
		return this->x == 0 && this->y == 0 && this->z == 0;
	}

	Vec3D Cross(const Vec3D& other) const {
		Vec3D res;
		Crossproduct(*this, other, res);
		return res;
	}

	bool Approx(Vec3D& other, float factor) {
		bool xacc = x > other.x - factor && x < other.x + factor;
		bool yacc = y > other.y - factor && y < other.y + factor;
		bool zacc = z > other.z - factor && z < other.z + factor;

		return xacc && yacc && zacc;
	}

	void Init(float ix, float iy, float iz);
	void Clamp();
	Vec3D Clamped();
	Vec3D Normalized();
	float DistanceTo(const Vec3D& other);
	void Normalize();
	float Length();
	float LengthSqr();
	float Length2DSqr(void) const;
	float Dot(const Vec3D other);
	float Dot(const float* other);
};

inline Vec3D operator*(float lhs, const Vec3D& rhs) {
	return Vec3D(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}