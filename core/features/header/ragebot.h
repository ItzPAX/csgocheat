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
	bool GunReady();
	bool HitChance(Player* player, CUserCmd* cmd, Vec3D vTargetPoint);
	std::vector<int> vAllowedHitboxes;

public:
	int iMenuWeapon = 0;
	void RunAimbot(CUserCmd* cmd);
};

extern RageBot g_Ragebot;