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

		switch (g_Config.ints[XOR("targetmode")].val) {
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
				vAllowedHitboxes.push_back(HITBOX_LEFT_FOOT);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_CALF);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_THIGH);
				vAllowedHitboxes.push_back(HITBOX_RIGHT_FOOT);
				break;
			}
		}
	}
}

bool RageBot::GunReady() {
	float flServerTime = Game::g_pLocal->iTickBase() * g_Interface.pGlobalVars->flIntervalPerTick;
	return Game::g_pLocal->pGetActiveWeapon()->flNextPrimary() <= flServerTime;
}

bool RageBot::HitChance() {
	Entity* weapon = Game::g_pLocal->pGetActiveWeapon();
	if (!weapon)
		return false;

	float hitchance = 1.f / weapon->Inaccuracy();
	if (hitchance >= (float)g_Config.arrints[XOR("ragehitchance")].val[iActiveWeapon])
		return true;

	return false;
}

void RageBot::RunAimbot(CUserCmd* cmd) {
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
		CCSWeaponData* data = pActiveWeapon->GetWeaponData();
		if (!data)
			return;

		for (auto hitbox : vAllowedHitboxes) {
			if (pTargetPlayer->bIsPointVisible(Game::g_pLocal->vEyeOrigin(), pTargetPlayer->vGetHitboxPos(hitbox))) {
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

	if (!HitChance())
		return;

	Vec3D vTargetAngle;
	g_Math.CalcAngle(Game::g_pLocal->vEyeOrigin(), vTargetSpot, vTargetAngle);

	if (g_Config.ints[XOR("compensaterecoil")].val)
		vTargetAngle -= Game::g_pLocal->vGetAimPunchAngle() * 2.f;

	cmd->viewangles = vTargetAngle;

	if (!g_Config.ints[XOR("ragesilent")].val)
		g_Interface.pEngine->SetViewAngles(vTargetAngle);

	if (g_Config.ints[XOR("autoshoot")].val && GunReady())
		cmd->buttons |= CUserCmd::IN_ATTACK;
}
