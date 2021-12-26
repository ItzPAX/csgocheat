#pragma once
#include "utilities/structures/vec3d/vec3d.h"

class ICollideable {
public:
	Vec3D& vMins() {
		using original_fn = Vec3D & (__thiscall*)(void*);
		return (*(original_fn**)this)[1](this);
	}
	Vec3D& vMaxs() {
		using original_fn = Vec3D & (__thiscall*)(void*);
		return (*(original_fn**)this)[2](this);
	}
};