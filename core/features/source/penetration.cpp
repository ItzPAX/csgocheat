#include "pch.h"
#include "includes.h"

Penetration g_Penetration;

void Penetration::UTIL_TraceLine(Vec3D vecAbsStart, Vec3D vecAbsEnd, unsigned int iMask, trace_t* trTrace) {
	// create ray and initialize it
	Ray_t rRay;
	rRay.Init(vecAbsStart, vecAbsEnd);

	// dont trace local player!
	CTraceFilter filFilter;
	filFilter.pSkip = Game::g_pLocal;

	// trace from localplayer
	g_Interface.pEngineTrace->TraceRay(rRay, iMask, &filFilter, trTrace);
}

void Penetration::UTIL_ClipTraceToPlayers(const Vec3D& vecAbsStart, const Vec3D& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr, const float flMinRange) {
	if (!filter || !tr)
		return;

	trace_t trace = { };
	float flSmallestFraction = tr->flFraction;

	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);

	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++)
	{
		Player* pEntity = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		if (pEntity == nullptr || !pEntity->bIsAlive() || pEntity->bDormant())
			continue;

		if (filter != nullptr && !filter->ShouldHitEntity(pEntity, mask))
			continue;

		const ICollideable* pCollideable = pEntity->GetCollideable();

		if (pCollideable == nullptr)
			continue;

		// get bounding box
		Vec3D vecMin = pCollideable->OBBMins();
		Vec3D vecMax = pCollideable->OBBMaxs();

		// calculate world space center
		Vec3D vecCenter = (vecMax + vecMin) * 0.5f;
		Vec3D vecPosition = vecCenter + pEntity->vOrigin();

		Vec3D vecTo = vecPosition - vecAbsStart;
		Vec3D vecDirection = vecAbsEnd - vecAbsStart;
		const float flLength = vecDirection.NormalizeInPlace();

		const float flRangeAlong = vecDirection.Dot(vecTo);
		float flRange = 0.0f;

		// calculate distance to ray
		if (flRangeAlong < 0.0f)
			// off start point
			flRange = -vecTo.Length();
		else if (flRangeAlong > flLength)
			// off end point
			flRange = -(vecPosition - vecAbsEnd).Length();
		else
			// within ray bounds
			flRange = (vecPosition - (vecDirection * flRangeAlong + vecAbsStart)).Length();

		constexpr float flMaxRange = 60.f;
		if (flRange < flMinRange || flRange > flMaxRange)
			continue;

		g_Interface.pEngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, (IHandleEntity*)pEntity, &trace);

		if (trace.flFraction < flSmallestFraction)
		{
			// we shortened the ray - save off the trace
			*tr = trace;
			flSmallestFraction = trace.flFraction;
		}
	}
}

__forceinline bool Penetration::TraceToExit(trace_t& enterTrace, trace_t& exitTrace, Vec3D& vecPosition, Vec3D& vecDirection, Player* pClipPlayer) {
	float flMaxDistance = 90.f, flRayExtension = 4.f, flCurrDistance = 0;
	int iStartContents = 0;

	while (flCurrDistance <= flMaxDistance)
	{
		// add extra distance to our ray
		flCurrDistance += flRayExtension;

		// multiply the direction vector to the distance so we go outwards, add our position to it
		Vec3D vecStart = vecPosition + vecDirection * flCurrDistance;

		if (!iStartContents)
			iStartContents = g_Interface.pEngineTrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		int iCurrentContents = g_Interface.pEngineTrace->GetPointContents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		
		if (!(iCurrentContents & MASK_SHOT_HULL) || (iCurrentContents & CONTENTS_HITBOX && iCurrentContents != iStartContents))
		{
			// setup our end position by deducting the direction by the extra added distance
			const Vec3D vecEnd = vecStart - (vecDirection * flRayExtension);

			// trace ray to world
			Ray_t rayWorld;
			rayWorld.Init(vecStart, vecEnd);

			CTraceFilter nofilter;
			g_Interface.pEngineTrace->TraceRay(rayWorld, MASK_SHOT | CONTENTS_HITBOX, nullptr, &exitTrace);
			
			CTraceFilter filter;
			filter.pSkip = pClipPlayer;
			UTIL_ClipTraceToPlayers(vecEnd, vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &exitTrace, -60.f);

			// check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exitTrace.bStartSolid && exitTrace.surface.flags & SURF_HITBOX)
			{
				// trace ray to entity
				Ray_t ray;
				ray.Init(vecStart, vecPosition);
			
				CTraceFilter filter;
				filter.pSkip = exitTrace.pHitEntity;
			
				g_Interface.pEngineTrace->TraceRay(ray, MASK_SHOT_HULL, &filter, &exitTrace);
			
				if (exitTrace.DidHit() && !exitTrace.bStartSolid)
				{
					vecStart = exitTrace.vecEnd;
					return true;
				}
			
				continue;
			}
			
			if (exitTrace.DidHit() && !exitTrace.bStartSolid)
			{
				if (g_Penetration.IsBreakableEnt(enterTrace.pHitEntity) && g_Penetration.IsBreakableEnt(exitTrace.pHitEntity))
					return true;
			
				if (enterTrace.surface.flags & SURF_NODRAW || !(exitTrace.surface.flags & SURF_NODRAW) && (exitTrace.plane.normal.Dot(vecDirection) <= 1.0f))
				{
					const float flMultiplier = exitTrace.flFraction * 4.0f;
					vecStart -= vecDirection * flMultiplier;
					return true;
				}
			
				continue;
			}
			
			if (!exitTrace.DidHit() || exitTrace.bStartSolid)
			{
				if (enterTrace.pHitEntity != nullptr && enterTrace.pHitEntity->iIndex() != 0 && g_Penetration.IsBreakableEnt(enterTrace.pHitEntity))
				{
					// did hit breakable non world entity
					exitTrace = enterTrace;
					exitTrace.vecEnd = vecStart + vecDirection;
					return true;
				}
			
				continue;
			}
		}
	}
	
	return false;
}

bool Penetration::HandleBulletPenetration(Player* pLocal, CCSWeaponData* pWeaponData, surfacedata_t* pEnterSurfaceData, FireBulletData_t& data) {
	static ConVar* ff_damage_reduction_bullets = g_Interface.pICVar->FindVar(XOR("ff_damage_reduction_bullets"));
	static ConVar* ff_damage_bullet_penetration = g_Interface.pICVar->FindVar(XOR("ff_damage_bullet_penetration"));

	if (!ff_damage_reduction_bullets || !ff_damage_bullet_penetration)
		return false;

	const float flReductionDamage = ff_damage_reduction_bullets->GetFloat();
	const float flPenetrateDamage = ff_damage_bullet_penetration->GetFloat();

	const MaterialHandle_t hEnterMaterial = pEnterSurfaceData->game.hMaterial;

	if (data.iPenetrateCount == 0 && hEnterMaterial != CHAR_TEX_GRATE && hEnterMaterial != CHAR_TEX_GLASS && !(data.enterTrace.surface.flags & SURF_NODRAW))
		return false;

	if (pWeaponData->flPenetration <= 0.0f || data.iPenetrateCount <= 0)
		return false;

	trace_t exitTrace;
	if (!TraceToExit(data.enterTrace, exitTrace, data.enterTrace.vecEnd, data.vecDirection, pLocal) && !(g_Interface.pEngineTrace->GetPointContents(data.enterTrace.vecEnd, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL))
		return false;

	const surfacedata_t* pExitSurfaceData = g_Interface.pSurfaceProps->GetSurfaceData(exitTrace.surface.surfaceProps);
	if (!pExitSurfaceData)
		return false;

	const MaterialHandle_t hExitMaterial = pExitSurfaceData->game.hMaterial;

	const float flExitPenetrationModifier = pExitSurfaceData->game.flPenetrationModifier;
	const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

	float flDamageLostModifier = 0.16f;
	float flPenetrationModifier = 0.0f;

	if (hEnterMaterial == CHAR_TEX_GRATE || hEnterMaterial == CHAR_TEX_GLASS)
	{
		flDamageLostModifier = 0.05f;
		flPenetrationModifier = 3.0f;
	}
	else if (((data.enterTrace.iContents >> 3) & CONTENTS_SOLID) || ((data.enterTrace.surface.flags >> 7) & SURF_LIGHT))
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = 1.0f;
	}
	else if (hEnterMaterial == CHAR_TEX_FLESH && data.enterTrace.pHitEntity != nullptr && data.enterTrace.pHitEntity->bIsPlayer() && (pLocal->iTeamNum() == data.enterTrace.pHitEntity->iTeamNum()))
	{
		if (flPenetrateDamage == 0.0f)
			return false;

		if (g_Config.ints[XOR("teamcheck")].val)
			return false;

		// shoot through teammates
		flDamageLostModifier = flPenetrateDamage;
		flPenetrationModifier = flPenetrateDamage;
	}
	else
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = (flEnterPenetrationModifier + flExitPenetrationModifier) * 0.5f;
	}

	if (hEnterMaterial == hExitMaterial)
	{
		if (hExitMaterial == CHAR_TEX_CARDBOARD || hExitMaterial == CHAR_TEX_WOOD)
			flPenetrationModifier = 3.0f;
		else if (hExitMaterial == CHAR_TEX_PLASTIC)
			flPenetrationModifier = 2.0f;
	}

	const float flTraceDistance = (exitTrace.vecEnd - data.enterTrace.vecEnd).LengthSqr();

	// penetration modifier
	const float flModifier = (flPenetrationModifier > 0.0f ? 1.0f / flPenetrationModifier : 0.0f);

	// this calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
	const float flLostDamage = (data.flCurrentDamage * flDamageLostModifier + (pWeaponData->flPenetration > 0.0f ? 3.75f / pWeaponData->flPenetration : 0.0f) * (flModifier * 3.0f)) + ((flModifier * flTraceDistance) / 24.0f);
	
	// did we loose too much damage?
	if (flLostDamage > data.flCurrentDamage)
		return false;

	// we can't use any of the damage that we've lost
	if (flLostDamage > 0.0f)
		data.flCurrentDamage -= flLostDamage;

	// do we still have enough damage to deal?
	if (data.flCurrentDamage < 1.0f)
		return false;

	data.vecPosition = exitTrace.vecEnd;
	--data.iPenetrateCount;
	return true;
}

bool Penetration::SimulateFireBullet(Player* pLocal, Entity* pWeapon, FireBulletData_t& data) {
	CCSWeaponData* pWeaponData = pWeapon->GetWeaponData();

	if (pWeaponData == nullptr)
		return false;

	float flMaxRange = pWeaponData->flRange;

	// the total number of surfaces any bullet can penetrate in a single flight is capped at 4
	data.iPenetrateCount = 4;
	// set our current damage to what our gun's initial damage reports it will do
	data.flCurrentDamage = static_cast<float>(pWeaponData->iDamage);

	float flTraceLenght = 0.0f;
	CTraceFilter filter;
	filter.pSkip = pLocal;

	while (data.iPenetrateCount > 0 && data.flCurrentDamage >= 1.0f)
	{
		// max bullet range
		flMaxRange -= flTraceLenght;

		// end position of bullet
		Vec3D vecEnd = data.vecPosition + data.vecDirection * flMaxRange;

		Ray_t ray;
		ray.Init(data.vecPosition, vecEnd);

		g_Interface.pEngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enterTrace);

		// check for player hitboxes extending outside their collision bounds
		UTIL_ClipTraceToPlayers(data.vecPosition, vecEnd + data.vecDirection * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enterTrace, -60.f);

		surfacedata_t* pEnterSurfaceData = g_Interface.pSurfaceProps->GetSurfaceData(data.enterTrace.surface.surfaceProps);
		if (!pEnterSurfaceData)
			return false;

		const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

		// we didn't hit anything, stop tracing shoot
		if (data.enterTrace.flFraction == 1.0f)
			break;

		// calculate the damage based on the distance the bullet traveled
		flTraceLenght += data.enterTrace.flFraction * flMaxRange;
		data.flCurrentDamage *= std::powf(pWeaponData->flRangeModifier, flTraceLenght / 500.f);

		// check is actually can shoot through
		if (flTraceLenght > 3000.0f || flEnterPenetrationModifier < 0.1f)
			break;

		// check is can do damage
		if (data.enterTrace.iHitGroup != HITGROUP_GENERIC && data.enterTrace.iHitGroup != HITGROUP_GEAR && pLocal->bIsEnemy(reinterpret_cast<Player*>(data.enterTrace.pHitEntity)))
		{
			// we got target - scale damage
			data.flCurrentDamage = ScaleDamage(reinterpret_cast<Player*>(data.enterTrace.pHitEntity), data.flCurrentDamage, pWeaponData->flArmorRatio, data.enterTrace.iHitbox);
			return true;
		}

		// calling handlebulletpenetration here reduces our penetration ñounter, and if it returns true, we can't shoot through it
		if (!HandleBulletPenetration(pLocal, pWeaponData, pEnterSurfaceData, data))
			break;
	}

	return false;
}

float Penetration::ScaleDamage(Player* plPlayer, float flDamage, float flArmorRatio, int iHitbox) {
	bool  bHasHeavyArmor;
	int   iArmor;
	float flHeavyRatio, flBonusRatio, flRatio, flNewDamage;

	static auto bIsArmored = [](Player* plPlayer, int iArmor, int iHitbox) {
		// the player has no armor.
		if (iArmor <= 0)
			return false;

		// if the hitgroup is head and the player has a helment, return true.
		// otherwise only return true if the hitgroup is not generic / legs / gear.
		if (iHitbox == HITBOX_HEAD && plPlayer->bHasHelmet())
			return true;

		else if (iHitbox == HITBOX_MAX)
			return false;

		return true;
	};

	// check if the player has heavy armor, this is only really used in operation stuff.
	bHasHeavyArmor = plPlayer->bHasHeavyArmor();

	// scale damage based on hitgroup.
	switch (iHitbox) {
	case HITBOX_HEAD:
		if (bHasHeavyArmor)
			flDamage = (flDamage * 4.f) / 2;
		else
			flDamage *= 4.f;
		break;

	case HITBOX_STOMACH:
		flDamage *= 1.25f;
		break;

	case HITBOX_LEFT_CALF:
	case HITBOX_LEFT_THIGH:
	case HITBOX_LEFT_FOOT:
	case HITBOX_RIGHT_CALF:
	case HITBOX_RIGHT_THIGH:
	case HITBOX_RIGHT_FOOT:
		flDamage *= 0.75f;
		break;

	default:
		break;
	}

	// grab amount of player armor.
	iArmor = plPlayer->iArmor();

	// check if the ent is armored and scale damage based on armor.
	if (bIsArmored(plPlayer, iArmor, iHitbox)) {
		flHeavyRatio = 1.f;
		flBonusRatio = 0.5f;
		flRatio = flArmorRatio * 0.5f;

		// player has heavy armor.
		if (bHasHeavyArmor) {
			// calculate ratio values.
			flHeavyRatio = 0.33f;
			flBonusRatio = 0.33f;
			flRatio = flArmorRatio / 4;

			// calculate new damage.
			flNewDamage = (flDamage * flRatio) * 0.85f;
		}

		// no heavy armor, do normal damage calculation.
		else
			flNewDamage = flDamage * flRatio;

		if (((flDamage - flNewDamage) * (flHeavyRatio * flBonusRatio)) > iArmor)
			flNewDamage = flDamage - (iArmor / flBonusRatio);

		flDamage = flNewDamage;
	}

	return std::floor(flDamage);
}

bool Penetration::IsBreakableEnt(Entity* pEnt) {
	using Fn = bool(__fastcall*)(Entity*);
	static auto fn = reinterpret_cast<Fn>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68\x83\xBE"), XOR("xxxxxxxxxxxxx")));

	if (!pEnt || !pEnt->iIndex())
		return false;

	auto take_damage{ (char*)((uintptr_t)pEnt + *(size_t*)((uintptr_t)fn + 0x26)) };
	auto take_damage_backup{ *take_damage };

	ClientClass* pClass = pEnt->cGetClientClass();

	if (pClass->m_pNetworkName == XOR("CBreakableSurface"))
		*take_damage = DAMAGE_YES;
	else if (pClass->m_pNetworkName == XOR("CBaseDoor") || pClass->m_pNetworkName == XOR("CDynamicProp"))
		*take_damage = DAMAGE_NO;

	bool breakable = fn(pEnt);
	*take_damage = take_damage_backup;

	return breakable;
}

float Penetration::GetDamage(Player* local, const Vec3D& point, FireBulletData_t& dataOut) {
	Vec3D vecPosition = local->vEyeOrigin();

	// setup data
	FireBulletData_t data = { };
	data.vecPosition = vecPosition;
	data.vecDirection = (point - vecPosition).Normalized();

	Entity* pWeapon = local->pGetActiveWeapon();

	if (pWeapon == nullptr)
		return -1.0f;

	if (!SimulateFireBullet(local, pWeapon, data))
		return -1.0f;

	dataOut = data;
	return data.flCurrentDamage;
}