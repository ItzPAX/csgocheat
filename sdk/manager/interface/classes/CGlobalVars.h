#pragma once

class CGlobalVars {
public:
	float flRealtime;
	int iFrameCount;
	float flAbsFrametime;
	float flAbsFrameStartTime;
	float flCurTime; // seconds
	float flFrameTime;
	int iMaxClients;
	int iTickCount;
	float flIntervalPerTick;
	float flInterpAmount;
	int iSimTicksThisFrame;
	int iNetworkProtocole;
	void* pSaveData;
	bool bIsClient;
	bool bIsRemoteClient;
	int iTimestampNetworkBase;
	int iTimestampRandomizeWindow;
};