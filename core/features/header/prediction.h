#pragma once
#include "pch.h"
#include "includes.h"

// Sponsored by CorvusHook

class Prediction {
private:
	PlayerMoveData data{ };
	void StartCommand(Player* player, CUserCmd* cmd);
	int GetTickBase(Player* player, CUserCmd* cmd);

	float flOldCurtime, flOldFrameTime;
	int iOldTickCount;

	unsigned int* iPredictionRandomSeed = *reinterpret_cast<unsigned int**>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04"), XOR("xx????x????x????xxx")) + 0x2); //
	Player** pPredictionPlayer = *reinterpret_cast<Player***>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x89\x35\x00\x00\x00\x00\xF3\x0F\x10\x48\x20"), XOR("xx????xxxxx")) + 0x2);

public:
	void Start(CUserCmd* ucmd, Player* player);
	void End(CUserCmd* ucmd, Player* player);
};

extern Prediction g_Prediction;