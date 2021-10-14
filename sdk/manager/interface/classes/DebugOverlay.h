#pragma once
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/color/color.h"

class IVDebugOverlay {
public:
	// updated function, class is outdated
	bool WorldToScreen(const Vec3D& in, Vec3D& out) {
		using original_fn = int(__thiscall*)(IVDebugOverlay*, const Vec3D&, Vec3D&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}
};