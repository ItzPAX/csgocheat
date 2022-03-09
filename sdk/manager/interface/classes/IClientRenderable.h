#pragma once
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/matrix/matrix.h"

class IClientRenderable {
public:
	// Gets at the containing class...
	virtual IClientUnknown* GetIClientUnknown() = 0;

	bool SetupBones(Matrix pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) {
		using original_fn = bool(__thiscall*)(IClientRenderable*, Matrix, int, int, float);
		return (*(original_fn**)this)[13](this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	void* GetModel() {
		using original_fn = void*(__thiscall*)(IClientRenderable*);
		return (*(original_fn**)this)[13](this);
	}
};