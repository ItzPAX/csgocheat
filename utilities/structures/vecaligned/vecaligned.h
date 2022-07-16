#include "includes.h"

class __declspec(align(16)) VectorAligned : public Vec3D
{
public:
	VectorAligned() = default;

	explicit VectorAligned(const Vec3D& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.f;
	}

	constexpr VectorAligned& operator=(const Vec3D& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.f;
		return *this;
	}

public:
	float w;
};