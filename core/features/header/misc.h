#pragma once
#include "pch.h"
#include "includes.h"

class Misc {
public:
	inline static std::vector<Spectator> pSpectators;
	std::string speclistname;
	inline static int iParanoiaSpecs = 0;

public:
	void UpdateSpectators();
	void BunnyHop(CUserCmd* cmd);
};

extern Misc g_Misc;