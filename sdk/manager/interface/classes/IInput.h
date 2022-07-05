#pragma once
#include "../sdk/classes/cusercmd.h"
#include "../utilities/structures/vec3d/vec3d.h"

class IInput {
public:
	char _pad0[0xad];
	bool m_fCameraInThirdPerson;
	char _pad1[0x2];
	Vec3D m_vCameraOffset;
	char _pad2[0x44];
	int m_iCameraThirdData;
	int m_iCamCommand;
public:
	CUserCmd* GetUserCmd(int slot, int sequence_num) {
		using fn = CUserCmd * (__thiscall*)(void*, int, int);
		return (*(fn**)this)[8](this, slot, sequence_num);
	}

	void CAM_Think() {
		using fn = void* (__thiscall*)(void*);
		(*(fn**)this)[31](this);
	}

	void CAM_ToThirdPerson() {
		using fn = void* (__thiscall*)(void*);
		(*(fn**)this)[35](this);
	}

	void CAM_ToFirstPerson() {
		using fn = void* (__thiscall*)(void*);
		(*(fn**)this)[36](this);
	}
};