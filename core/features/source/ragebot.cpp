#include "pch.h"
#include "includes.h"

RageBot g_Ragebot;

Player* RageBot::GetTargetPlayer() {
	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();
	Vec3D vViewAngles; g_Interface.pEngine->GetViewAngles(vViewAngles);

	Player* pTarget = nullptr;

	float flBest[MODE_MAX];
	std::fill_n(flBest, MODE_MAX, FLT_MAX);
	flBest[MODE_DMG] = -1.f;

	// get best player
	for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
		Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

		if (!pPlayer || !pPlayer->bIsPlayer() || pPlayer->bDormant() || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal) || pPlayer->bHasGunGameImmunity())
			continue;

		Vec3D vAngle;
		Vec3D vHitboxPos = pPlayer->vGetHitboxPos(HITBOX_HEAD);

		switch (g_Config.ints["targetmode"].val) {
		case MODE_DMG: {
			FireBulletData_t dataout;
			float flDamage = g_Penetration.GetDamage(Game::g_pLocal, vHitboxPos, dataout);
			if (flDamage > flBest[MODE_DMG]) {
				flBest[MODE_DMG] = flDamage;
				pTarget = pPlayer;
			}
		}
			break;

		case MODE_CROSSHAIR: {
			g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vViewAngles).Clamped().Length();
			if (flCurrDelta < flBest[MODE_CROSSHAIR]) {
				flBest[MODE_CROSSHAIR] = flCurrDelta;
				pTarget = pPlayer;
			}
		}
			break;

		case MODE_DISTANCE: {
			float flDist = vHitboxPos.DistanceTo(vEyeOrigin);
			if (flDist < flBest[MODE_DISTANCE]) {
				flBest[MODE_DISTANCE] = flDist;
				pTarget = pPlayer;
			}
		}
			break;
		default:
			pTarget = nullptr;
			break;
		}

		PlayerInfo pinfo;
		g_Interface.pEngine->GetPlayerInfo(i, &pinfo);

		// this is a priorityplayer, if he is in fov range focus him
		if (g_PlayerList.settings[pinfo.isteamid].bPrioritizePlayer) {
			pTarget = pPlayer;
			break;
		}
	}

	return pTarget;
}

void RageBot::UpdateHitboxes() {
	vAllowedHitboxes.clear();
	for (int i = 0; i < 4; i++) {
		if (g_Config.arrints[XOR("ragehitboxes")].val[i + (4 * iActiveWeapon)]) {
			switch (i) {
			case 0:
				vAllowedHitboxes.push_back(HITBOX_HEAD);
				break;
			case 1:
				vAllowedHitboxes.push_back(HITBOX_CHEST);
				vAllowedHitboxes.push_back(HITBOX_LOWER_CHEST);
				vAllowedHitboxes.push_back(HITBOX_UPPER_CHEST);
				break;
			case 2:
				vAllowedHitboxes.push_back(HITBOX_STOMACH);
				vAllowedHitboxes.push_back(HITBOX_PELVIS);
				break;
			case 3:
				vAllowedHitboxes.push_back(HITBOX_LEFT_CALF);
				vAllowedHitboxes.push_back(HITBOX_LEFT_THIGH);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_CALF);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_THIGH);
				break;
			}
		}
	}
}

bool RageBot::GunReady() {
	float flServerTime = Game::g_pLocal->iTickBase() * g_Interface.pGlobalVars->flIntervalPerTick;
	return Game::g_pLocal->pGetActiveWeapon()->flNextPrimary() <= flServerTime;
}

bool RageBot::HitChance(Player* player, CUserCmd* cmd, Vec3D vTargetPoint) {
	Entity* pWpn = Game::g_pLocal->pGetActiveWeapon();
	CCSWeaponData* pWpnData = pWpn->GetWeaponData();

	// create vars we will need later on
	constexpr float HITCHANCE_MAX = 100.f;
	constexpr int   SEED_MAX = 256;

	Vec3D  vecStart = Game::g_pLocal->vEyeOrigin(), vecAimPoint, vecWepSpread, vecSpreadView, vecDir, vecForward, vecRight, vecUp, vecEnd, vecAngle, vecTraceEnd;
	float   flInaccuracy, flSpread, flInaccuracyVal, flSpreadVal;
	int     iTotalHits = 0, iNeededHits = std::floor((g_Config.arrints["ragehitchance"].val[iActiveWeapon] * (SEED_MAX / HITCHANCE_MAX)));
	trace_t trTraceSpread;
	Ray_t   rRaySpread;

	// get angle
	g_Math.CalcAngle(vecStart, vTargetPoint, vecAimPoint);

	// directional vectors
	g_Math.AngleVector4(vecAimPoint, &vecForward, &vecRight, &vecUp);
	g_Math.FastVecNormalize(vecForward); g_Math.FastVecNormalize(vecRight); g_Math.FastVecNormalize(vecUp);

	// store off inaccuracy
	flInaccuracy = pWpn->Inaccuracy();
	flSpread = pWpn->GetSpread();

	// update accuracy
	pWpn->UpdateAccuracyPenalty();

	// iterate all seeds;
	for (int i = 0; i < SEED_MAX; ++i) {

		cmd->random_seed = i + 1;
		float a = g_Math.RandomFloat(0.f, 1.f);
		float b = g_Math.RandomFloat(0.f, 1.f);
		float c = g_Math.RandomFloat(0.f, M_PI2);
		float d = g_Math.RandomFloat(0.f, M_PI2);

		flInaccuracyVal = flInaccuracy * a;
		flSpreadVal = flSpread * b;

		if (pWpn->iItemDefinitionIndex() == WEAPON_REVOLVER) {
			a = 1.f - a * a;
			a = 1.f - b * b;
		}

		// get spread vector
		vecWepSpread = { (cos(c) * flSpreadVal) + (cos(d) * flInaccuracyVal), (sin(c) * flSpreadVal) + (sin(d) * flInaccuracyVal), 0.f };

		// get direction of spread
		vecDir.x = vecForward.x + (vecRight.x * vecWepSpread.x) + (vecUp.x * vecWepSpread.y);
		vecDir.y = vecForward.y + (vecRight.y * vecWepSpread.x) + (vecUp.y * vecWepSpread.y);
		vecDir.z = vecForward.z + (vecRight.z * vecWepSpread.x) + (vecUp.z * vecWepSpread.y);
		g_Math.FastVecNormalize(vecDir);

		g_Math.VectorAngles(vecDir, vecSpreadView);

		vecAngle = vecAimPoint - (vecSpreadView - vecAimPoint);
		g_Math.AngleVector2(vecAngle, vecEnd);
		g_Math.FastVecNormalize(vecEnd);

		// get trace end
		vecTraceEnd = vecStart + (vecEnd * pWpnData->flRange);

		// setup ray
		rRaySpread.Init(vecStart, vecTraceEnd);

		// setup trace
		g_Interface.pEngineTrace->ClipRayToEntity(rRaySpread, MASK_SHOT_HULL | CONTENTS_HITBOX, (IHandleEntity*)player, &trTraceSpread);

		// we hit our target
		if (trTraceSpread.pHitEntity == player)
			iTotalHits++;

		//we made it.
		if (iTotalHits >= iNeededHits) {
			return true;
		}

		// we cant make it anymore
		if (iTotalHits + (SEED_MAX - i) < iNeededHits) {
			return false;
		}
	}

	return true;
}

void RageBot::RunAimbot(CUserCmd* cmd) {
	// save old move values to be able to normally move when shooting
	Vec3D vOldViewangles = cmd->viewangles;
	float flOldForward = cmd->forwardmove;
	float flOldSide = cmd->sidemove;

    if (!g_Config.ints[XOR("ragebot")].val || g_Config.ints[XOR("trustfactor")].val)
        return;

    if (!g_InputMgr.GetHotkeyState(g_Config.arrints[XOR("ragebotkey")].val, XOR("Ragebot")))
        return;

	// paranoia mode
	if (g_Misc.iParanoiaSpecs > 0 && g_Config.ints[XOR("paranoia")].val)
		return;

	Player* pTargetPlayer = GetTargetPlayer();
	if (!pTargetPlayer)
		return;

	Entity* pActiveWeapon = Game::g_pLocal->pGetActiveWeapon();
	if (!pActiveWeapon)
		return;

	// only run aimbot on actual weapons, not grenades etc.
	int iWeaponType = pActiveWeapon->iGetWeaponType();
	switch (iWeaponType) {
	case Entity::WEAPONTYPE_MACHINEGUN:
	case Entity::WEAPONTYPE_RIFLE:
	case Entity::WEAPONTYPE_SHOTGUN:
	case Entity::WEAPONTYPE_PISTOL:
	case Entity::WEAPONTYPE_SNIPER:
	{
		if (!pActiveWeapon->iClip()) {
			cmd->buttons |= CUserCmd::IN_RELOAD;
			return;
		}
		break;
	}
	default:
		return;
	}

	if (iWeaponType == Entity::WEAPONTYPE_SNIPER)
		iActiveWeapon = 0;
	if (iWeaponType == Entity::WEAPONTYPE_MACHINEGUN || iWeaponType == Entity::WEAPONTYPE_RIFLE)
		iActiveWeapon = 1;
	if (iWeaponType == Entity::WEAPONTYPE_SHOTGUN || iWeaponType == Entity::WEAPONTYPE_PISTOL)
		iActiveWeapon = 2;

	int iTargetHitbox = HITBOX_NONE;
	UpdateHitboxes();
	if (g_Config.ints[XOR("autowall")].val) {
		float flBestDamage = (float)g_Config.arrints[XOR("ragedmg")].val[iActiveWeapon];
		for (auto hitbox : vAllowedHitboxes) {
			Vec3D vHitboxPos = pTargetPlayer->vGetHitboxPos(hitbox);
			FireBulletData_t data;
			float flDamage = g_Penetration.GetDamage(Game::g_pLocal, vHitboxPos, data);
			if (flDamage > flBestDamage) {
				flBestDamage = flDamage;
				iTargetHitbox = hitbox;
			}
		}
	}
	else {
		float flBestDamage = (float)g_Config.arrints[XOR("ragedmg")].val[iActiveWeapon];
		for (auto hitbox : vAllowedHitboxes) {
			if (pTargetPlayer->bIsPointVisible(Game::g_pLocal->vEyeOrigin(), pTargetPlayer->vGetHitboxPos(hitbox))) {
				CCSWeaponData* data = pActiveWeapon->GetWeaponData();
				if (!data)
					return;
				float flDamage = g_Penetration.ScaleDamage(pTargetPlayer, (float)data->iDamage, data->flArmorRatio, hitbox);
				if (flDamage > flBestDamage) {
					flBestDamage = flDamage;
					iTargetHitbox = hitbox;
				}
			}
		}
	}

	if (iTargetHitbox == HITBOX_NONE)
		return;

	Vec3D vTargetSpot = pTargetPlayer->vGetHitboxPos(iTargetHitbox);

	/*FIXME FIXME FIXME*/
	//if (!HitChance(pTargetPlayer, cmd, vTargetSpot))
	//	return;

	Vec3D vTargetAngle;
	g_Math.CalcAngle(Game::g_pLocal->vEyeOrigin(), vTargetSpot, vTargetAngle);

	if (g_Config.ints[XOR("compensaterecoil")].val)
		vTargetAngle -= Game::g_pLocal->vGetAimPunchAngle() * 2.f;

	cmd->viewangles = vTargetAngle;

	if (!g_Config.ints[XOR("ragesilent")].val)
		g_Interface.pEngine->SetViewAngles(vTargetAngle);

	if (g_Config.ints[XOR("autoshoot")].val && GunReady())
		cmd->buttons |= CUserCmd::IN_ATTACK;

	g_Math.CorrectMovement(vOldViewangles, cmd, flOldForward, flOldSide);
}
