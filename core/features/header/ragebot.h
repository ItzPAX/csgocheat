#pragma once

struct MultiPoints {
	std::vector<Vec3D> cSafePoints;
	std::vector<Vec3D> cUnsafePoints;
};

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
	bool HitChance();
	std::vector<int> vAllowedHitboxes;

public:
	int iMenuWeapon = 0;
	bool GunReady();
	void RunAimbot(CUserCmd* cmd);
};

extern RageBot g_Ragebot;