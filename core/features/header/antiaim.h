#pragma once
#include "pch.h"
#include "includes.h"

class AntiAim {
public:
	void DoDesync(CUserCmd* cmd, bool bShouldSendPacket);
};