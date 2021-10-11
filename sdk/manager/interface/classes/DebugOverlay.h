#pragma once
#include "utilities/structures/vec3d/vec3d.h"

class IVDebugOverlay {
public:
	bool WorldToScreen(const Vec3D& in, Vec3D& out) {
		using original_fn = int(__thiscall*)(IVDebugOverlay*, const Vec3D&, Vec3D&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

	bool ScreenPosition(const Vec3D& in, Vec3D& out) {
		using original_fn = bool(__thiscall*)(IVDebugOverlay*, const Vec3D&, Vec3D&);
		return (*(original_fn**)this)[11](this, std::ref(in), std::ref(out));
	}
};