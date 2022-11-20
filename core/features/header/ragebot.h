#pragma once

class RageBot {
private:
	enum ETargetMode : int {
		MODE_DMG = 0,
		MODE_CROSSHAIR,
		MODE_DISTANCE,
		MODE_MAX
	};
	int iActiveWeapon = 0;

	Player* GetTargetPlayer();
	void UpdateHitboxes();
	void GetMultipoints(std::vector<int> hitboxes, Player* pPlayer, float flScale, std::vector<Vec3D>& multipoints);
	Vec3D ExtrapolatePlayer(Player* pTarget, Vec3D vAimPos);
	LagRecord* GetBestRecord(Player* pTarget);
	bool HitChance();
	std::vector<int> vAllowedHitboxes;

public:
	int iMenuWeapon = 0;
	bool GunReady();
	void RunAimbot(CUserCmd* cmd, bool* bSendPacket);
};

extern RageBot g_Ragebot;