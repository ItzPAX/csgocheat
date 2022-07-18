#pragma once
#include "utilities/structures/matrix/matrix.h"
#include "utilities/tools/xorstr.h"
#include "utilities/tools/tools.h"
#include "CUtlVector.h"
#include "includes.h"
#include "pch.h"

#define INVALID -1

class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	CUtlVector< VarMapEntry_t >	m_Entries;
	int							m_nInterpolatedEntries;
	float						m_lastInterpolationTime;
};

class Entity {
public:
	enum EntityFlags {
		FL_ONGROUND = (1 << 0),
		FL_DUCKING = (1 << 1),
		FL_WATERJUMP = (1 << 2),
		FL_ONTRAIN = (1 << 3),
		FL_INRAIN = (1 << 4),
		FL_FROZEN = (1 << 5),
		FL_ATCONTROLS = (1 << 6),
		FL_CLIENT = (1 << 7),
		FL_FAKECLIENT = (1 << 8),
		FL_INWATER = (1 << 9),
		FL_FLY = (1 << 10),
		FL_SWIM = (1 << 11),
		FL_CONVEYOR = (1 << 12),
		FL_NPC = (1 << 13),
		FL_GODMODE = (1 << 14),
		FL_NOTARGET = (1 << 15),
		FL_AIMTARGET = (1 << 16),
		FL_PARTIALGROUND = (1 << 17),
		FL_STATICPROP = (1 << 18),
		FL_GRAPHED = (1 << 19),
		FL_GRENADE = (1 << 20),
		FL_STEPMOVEMENT = (1 << 21),
		FL_DONTTOUCH = (1 << 22),
		FL_BASEVELOCITY = (1 << 23),
		FL_WORLDBRUSH = (1 << 24),
		FL_OBJECT = (1 << 25),
		FL_KILLME = (1 << 26),
		FL_ONFIRE = (1 << 27),
		FL_DISSOLVING = (1 << 28),
		FL_TRANSRAGDOLL = (1 << 29),
		FL_UNBLOCKABLEBYPLAYER = (1 << 30)
	};

	enum WeaponType {
		WEAPONTYPE_KNIFE = 0,
		WEAPONTYPE_PISTOL = 1,
		WEAPONTYPE_SUBMACHINEGUN = 2,
		WEAPONTYPE_RIFLE = 3,
		WEAPONTYPE_SHOTGUN = 4,
		WEAPONTYPE_SNIPER = 5,
		WEAPONTYPE_MACHINEGUN = 6,
		WEAPONTYPE_C4 = 7,
		WEAPONTYPE_PLACEHOLDER = 8,
		WEAPONTYPE_GRENADE = 9,
		WEAPONTYPE_HEALTHSHOT = 11,
		WEAPONTYPE_FISTS = 12,
		WEAPONTYPE_BREACHCHARGE = 13,
		WEAPONTYPE_BUMPMINE = 14,
		WEAPONTYPE_TABLET = 15,
		WEAPONTYPE_MELEE = 16
	};

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,	// never moves
		MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
		MOVETYPE_WALK,				// Player only - moving on the ground
		MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
		MOVETYPE_FLY,				// No gravity, but still collides with stuff
		MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
		MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
		MOVETYPE_PUSH,				// no clip to world, push and crush
		MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
		MOVETYPE_LADDER,			// Used by players only when going onto a ladder
		MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
		MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

		// should always be defined as the last item in the list
		MOVETYPE_LAST = MOVETYPE_CUSTOM,

		MOVETYPE_MAX_BITS = 4
	};

public:
	void* pRenderable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x4); }
	void* pNetworkable() { return reinterpret_cast<void*>(uintptr_t(this) + 0x8); }

	ICollideable* GetCollideable() {
		using original_fn = ICollideable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	CCSWeaponData* GetWeaponData() {
		return g_Interface.pWeaponSystem->GetWeaponData(this->iItemDefinitionIndex());
	}

	int iIndex() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)pNetworkable())[10](pNetworkable());
	}

	int IsMaxHealth()
	{
		// @ida: FF 90 ? ? ? ? 85 C0 0F 8F ? ? ? ? 80 + 0x2
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)this)[122](this);
	}

	bool bIsPlayer() {
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[158](this);
	}

	const char* GetClassname()
	{
		// @ida: client.dll @ [8B 01 FF 90 ? ? ? ? 90 + 0x4] / sizeof(std::uintptr_t)
		using original_fn = const char*(__thiscall*)(void*);
		return (*(original_fn**)this)[143](this);
	}

	unsigned int PhysicsSolidMaskForEntity() {
		using original_fn = unsigned int(__thiscall*)(void*);
		return (*(original_fn**)this)[152](this);
	}

	bool bIsBaseCombatWeapon() {
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[165](this);
	}

	ClientClass* cGetClientClass() {
		using original_fn = ClientClass*(__thiscall*)(void*);
		if (this && this->pNetworkable())
			return (*(original_fn**)pNetworkable())[2](pNetworkable());
		return nullptr;
	}

	Model* mGetModel() {
		using original_fn = Model* (__thiscall*)(void*);
		return (*(original_fn**)pRenderable())[8](pRenderable());
	}

	bool bIsPointVisible(Vec3D vFrom, Vec3D vPoint) {
		if (!Game::g_pLocal)
			return false;

		trace_t tTrace;
		// skip local player
		CTraceFilter cTraceFilter;
		cTraceFilter.pSkip = Game::g_pLocal;

		Ray_t ray;
		ray.Init(vFrom, vPoint);

		g_Interface.pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &cTraceFilter, &tTrace);

		return tTrace.pHitEntity == this && tTrace.flFraction >= 0.97;
	}

	float Inaccuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[483](this);
	}

	float GetSpread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[453](this);
	}

	void UpdateAccuracyPenalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[484](this);
	}

	int iGetWeaponType() {
		using original_fn = int (__thiscall*)(void*);
		return (*(original_fn**)this)[455](this);
	}

	int iHealth() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x100);
	}

	bool bIsBroken() { return g_NetVars.Netvar<bool>(XOR("CBreakableSurface"), XOR("m_bIsBroken"), this); }
	bool bDormant() { 
		using tOriginalFn = bool(__thiscall*)(void*); 
		if (pNetworkable())
			return (*reinterpret_cast<tOriginalFn**>(pNetworkable()))[9](pNetworkable()); 
		return true;
	}
	Vec3D vOrigin() { return g_NetVars.Netvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecOrigin"), this); }
	int iTeamNum() { return g_NetVars.Netvar<int>(XOR("DT_CSPlayer"), XOR("m_iTeamNum"), this); }
	int iTickBase() { return g_NetVars.GetNetvar<int>(XOR("DT_CSPlayer"), XOR("m_nTickBase"), this); }
	float flNextPrimary() { return g_NetVars.GetNetvar<float>(XOR("DT_BaseCombatWeapon"), XOR("m_flNextPrimaryAttack"), this); }
	int iClip() { return g_NetVars.GetNetvar<int>(XOR("DT_CBaseCombatWeapon"), XOR("m_iClip1"), this); }
	short iItemDefinitionIndex() { return g_NetVars.Netvar<short>(XOR("DT_BaseAttributableItem"), XOR("m_iItemDefinitionIndex"), this); }
	Vec3D vBombOrigin() { return g_NetVars.Netvar<Vec3D>(XOR("DT_TEPlantBomb"), XOR("m_vecOrigin"), this); }
};

class Player : public Entity {
public:
	bool SetupBones(Matrix* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, Matrix*, int, int, float);
		return (*(original_fn**)pRenderable())[13](pRenderable(), out, max_bones, mask, time);
	}

	void UpdateClientSideAnimations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
	}

	Vec3D vGetBonePosition(int bone) {
		Matrix BoneMatrix[MAXSTUDIOBONES];
		if (SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_Interface.pGlobalVars->flCurTime))
			return Vec3D{ BoneMatrix[bone][0][3], BoneMatrix[bone][1][3], BoneMatrix[bone][2][3] };
		else
			return Vec3D();
	}

	Vec3D vGetHitboxPos(int hitbox) {
		Matrix BoneMatrix[MAXSTUDIOBONES];
		if (!SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g_Interface.pGlobalVars->flCurTime))
			return Vec3D();

		StudioHDR* pStudioModel = g_Interface.pModelInfo->GetStudioModel(mGetModel());
		if (!pStudioModel)
			return Vec3D();

		StudioBox* pHitbox = pStudioModel->Hitbox(hitbox, iGetHitboxSet());
		if (!pHitbox)
			return Vec3D();

		Vec3D vMin{}, vMax{};
		g_Math.TransformVector(pHitbox->vMins, BoneMatrix[pHitbox->iBone], vMin);
		g_Math.TransformVector(pHitbox->vMaxs, BoneMatrix[pHitbox->iBone], vMax);

		return Vec3D((vMin.x + vMax.x) * 0.5f, (vMin.y + vMax.y) * 0.5f, (vMin.z + vMax.z) * 0.5f);
	}

	Vec3D& vAbsOrigin() {
		using original_fn = Vec3D& (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);
	}

	Vec3D& vAbsAngles() {
		using original_fn = Vec3D & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);
	}

	Vec3D vEyeOrigin() {
		Vec3D vEyeOrig;
		using original_fn = void(__thiscall*)(void*, Vec3D&);
		(*(original_fn**)this)[169](this, vEyeOrig);
		return vEyeOrig;
	}

	Entity* pGetActiveWeapon() {
		using original_fn = Entity* (__thiscall*)(void*);
		return (*(original_fn**)this)[268](this);
	}

	void SetAngles(Vec3D vAngles) {
		using original_fn = void(__thiscall*)(void*, const Vec3D&);
		static original_fn set_angles_fn = (original_fn)((DWORD)g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1"), XOR("xxxxxxxxxxxxxx")));
		set_angles_fn(this, vAngles);
	}
	void SetPosition(Vec3D vPosition) {
		using original_fn = void(__thiscall*)(void*, const Vec3D&);
		static original_fn set_position_fn = (original_fn)((DWORD)g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8"),XOR("xxxxxxxxxxxxx")));
		set_position_fn(this, vPosition);
	}

	bool bIsEnemy(Player* to) { return this->iTeamNum() != to->iTeamNum(); }

	bool bIsAlive() { 
		return iHealth() > 0;
	}

	ulong hObserverTarget() { return g_NetVars.Netvar<ulong>(XOR("DT_BasePlayer"), XOR("m_hObserverTarget"), this); }
	int iObserverMode() { return g_NetVars.Netvar<int>(XOR("DT_BasePlayer"), XOR("m_iObserverMode"), this); }
	Vec3D vGetVelocity() { return g_NetVars.Netvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecVelocity[0]"), this); }
	Vec3D vGetViewOffset() { return g_NetVars.Netvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_vecViewOffset[0]"), this); }
	Vec3D vGetAimPunchAngle() { return g_NetVars.Netvar<Vec3D>(XOR("DT_BasePlayer"), XOR("m_aimPunchAngle"), this); }
	float flSimTime() { return g_NetVars.Netvar<float>(XOR("DT_CSPlayer"), XOR("m_flSimulationTime"), this); }
	int iGetHitboxSet() { return g_NetVars.Netvar<int>(XOR("DT_BasePlayer"), XOR("m_nHitboxSet"), this); }
	bool bIsDefusing() { return g_NetVars.Netvar<bool>(XOR("DT_CSPlayer"), XOR("m_bIsDefusing"), this); }
	bool bIsScoped() { return g_NetVars.Netvar<bool>(XOR("DT_CSPlayer"), XOR("m_bIsScoped"), this); }
	bool bHasGunGameImmunity() { return g_NetVars.Netvar<bool>(XOR("DT_CSPlayer"), XOR("m_bGunGameImmunity"), this); }
	bool bHasHelmet() { return g_NetVars.Netvar<bool>(XOR("DT_CSPlayer"), XOR("m_bHasHelmet"), this); }
	bool bHasHeavyArmor() { return g_NetVars.Netvar<bool>(XOR("DT_CSPlayer"), XOR("m_bHasHeavyArmor"), this); }
	int iArmor() { return g_NetVars.Netvar<int>(XOR("DT_CSPlayer"), XOR("m_ArmorValue"), this); }

	Vec3D vEyeAngles() { return g_NetVars.Netvar<Vec3D>(XOR("DT_CSPlayer"), XOR("m_angEyeAngles"), this); }
	int iShotsFired() { return g_NetVars.Netvar<int>(XOR("DT_CSPlayer"), XOR("m_iShotsFired"), this); }
	int iFlags() { return g_NetVars.Netvar<int>(XOR("DT_CSPlayer"), XOR("m_fFlags"), this); }

	float flOldSimTime() {
		return g_NetVars.Netvar<float>(XOR("DT_CSPlayer"), XOR("m_flSimulationTime"), this + 0x4);;
	}
};