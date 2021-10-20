#pragma once
#include "sdk/classes/cusercmd.h"

class IClientMode {
	bool CreateMove(float flInputSampleTime, CUserCmd* cmd) {
		using original_fn = bool(__thiscall*)(IClientMode*, float, CUserCmd*);
		return (*(original_fn**)this)[24](this, flInputSampleTime, cmd);
	}

	void LevelInit(const char* newmap) {
		using original_fn = void(__thiscall*)(IClientMode*, const char*);
		return (*(original_fn**)this)[25](this, newmap);
	}
};