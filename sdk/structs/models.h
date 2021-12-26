#pragma once
#include "utilities/structures/vec3d/vec3d.h"

struct Model {
	void* pHandle;
	char cName[260];
	int	iLoadFlags;
	int	iServerCount;
	int	iType;
	int	iFlags;
	Vec3D vMins;
	Vec3D vMaxs;
	float flRadius;
};