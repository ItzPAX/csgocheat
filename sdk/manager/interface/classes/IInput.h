#pragma once
#include "../sdk/classes/cusercmd.h"
#include "../utilities/structures/vec3d/vec3d.h"

class IInput {
private:
	char u0[172];
	bool u1;
public:
	bool bCameraInThirdperson;
private:
	char u2[2];
public:
	Vec3D vCameraOffset;

	CUserCmd* GetUserCmd(int slot, int sequence_num) {
		using fn = CUserCmd * (__thiscall*)(void*, int, int);
		return (*(fn**)this)[8](this, slot, sequence_num);
	}
};