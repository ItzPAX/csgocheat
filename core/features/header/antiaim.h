#pragma once
#include "pch.h"
#include "includes.h"

class AntiAim {
private:
	enum EAAModes : int {
		MODE_STANDING = 0,
		MODE_WALKING,
		MODE_INAIR,
		MODE_MAX
	};

	int iMode = 0;
	void SetMode();

public:
	int iMenuMode = 0;
	void DoDesync(CUserCmd* cmd, bool bShouldSendPacket);
};

extern AntiAim g_AntiAim;