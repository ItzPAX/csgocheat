#pragma once
#include "pch.h"
#include "includes.h"

class Misc {
private:
	CreateClientClassFn GetFlexCreateFn();

public:
	inline static std::vector<Spectator> pSpectators;
	std::string speclistname;
	inline static int iParanoiaSpecs = 0;

public:
	Player* CreateFlex(int entry, int serial);
	void SetClantag(const char* tag);
	void UpdateSpectators();
	void BunnyHop(CUserCmd* cmd);
};

extern Misc g_Misc;