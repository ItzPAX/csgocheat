#pragma once

/* autowall objects structure */
struct FireBulletData_t
{
	Vec3D			vecPosition = { };
	Vec3D			vecDirection = { };
	trace_t			enterTrace = { };
	float			flCurrentDamage = 0.0f;
	int				iPenetrateCount = 0;
};

class Penetration {
private:
	void UTIL_TraceLine(Vec3D vecAbsStart, Vec3D vecAbsEnd, unsigned int iMask, trace_t* trTrace);
	void UTIL_ClipTraceToPlayers(const Vec3D& vecAbsStart, const Vec3D& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr, const float flMinRange);

	bool TraceToExit(trace_t& enterTrace, trace_t& exitTrace, Vec3D& vecPosition, Vec3D& vecDirection, Player* pClipPlayer);
	bool HandleBulletPenetration(Player* pLocal, CCSWeaponData* pWeaponData, surfacedata_t* pEnterSurfaceData, FireBulletData_t& data);
	bool SimulateFireBullet(Player* pLocal, Entity* pWeapon, FireBulletData_t& data);

	bool bDidPenetrateBreakable = false;
	bool bBreakableHidden = false;

public:
	float ScaleDamage(Player* plPlayer, float flDamage, float flArmorRatio, int iHitbox);
	bool IsBreakableEnt(Entity* pEnt);
	float GetDamage(Player* local, const Vec3D& point, FireBulletData_t& dataOut);
};

extern Penetration g_Penetration;