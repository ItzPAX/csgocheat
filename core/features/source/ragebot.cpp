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

void RageBot::GetMultipoints(std::vector<int> hitboxes, Player* pPlayer, float flScale, std::vector<Vec3D>& multipoints) {
	if (hitboxes.empty()) {
		return;
	}

	// store bone matrix
	std::array< Matrix, 128 > cCachedBones;
	if (!pPlayer->SetupBones(cCachedBones.data(), cCachedBones.size(), BONE_USED_BY_HITBOX, 0.f)) {
		return;
	}

	const Model* pModel = pPlayer->mGetModel();

	if (!pModel) {
		return;
	}

	StudioHDR* pStudioHdr = g_Interface.pModelInfo->GetStudioModel(pModel);
	if (!pStudioHdr) {
		return;
	}

	int idx = 0;
	for (auto hitbox : hitboxes) {
		StudioBox* pHitbox = pStudioHdr->Hitbox(hitbox, 0);
		if (!pHitbox) {
			return;
		}

		// create matrix with bone position stored
		Matrix& mat = cCachedBones.at(pHitbox->iBone);

		// fix capsule -> aabb
		float mod = pHitbox->flRadius != -1.f ? pHitbox->flRadius : 0.f;
		Vec3D vecMins, vecMaxs;

		// transform vector, important step
		g_Math.TransformVector(pHitbox->vMins, mat, vecMins);
		g_Math.TransformVector(pHitbox->vMaxs, mat, vecMaxs);

		// calculate center and how much one percent would equal (for scaling ofc)
		Vec3D center = (vecMins + vecMaxs) / 2.f;
		float flOnePerc = pHitbox->flRadius / 100.f;

		for (int i = 0; i < 4; i++)
			multipoints.push_back(center);

		// here you can add or remove hitboxes you want multipoint for
		switch (hitbox) {

		case HITBOX_HEAD:
			//        vector id | pitch/yaw/roll
			// THIS SHIT SCALABLE MY NIGGA

			// here we get the position of the most right, left, top and bottom point
			multipoints.at(idx)[0] += (flOnePerc * flScale); // left ear, left point
			multipoints.at(idx + 1)[0] -= (flOnePerc * flScale); // right ear, right point
			multipoints.at(idx + 2)[2] += (flOnePerc * flScale); // forehead, highest point
			multipoints.at(idx + 3)[2] -= (flOnePerc * flScale); // chin, lowest point
			break;

		case HITBOX_STOMACH:
		case HITBOX_PELVIS:
		case HITBOX_CHEST:
			//        vector id | pitch/yaw/roll
			// THIS SHIT SCALABLE MY NIGGA

			// here we get the position of the most right, left, top and bottom point
			multipoints.at(idx)[0] += (flOnePerc * flScale); // left ear, left point
			multipoints.at(idx + 1)[0] -= (flOnePerc * flScale); // right ear, right point
			multipoints.at(idx + 2)[2] += (flOnePerc * flScale); // forehead, highest point
			multipoints.at(idx + 3)[2] -= (flOnePerc * flScale); // chin, lowest point
			break;

			// still need to fix these, since these are not capusles unlike the other hitboxes so mp not proper for feet rn
		case HITBOX_LEFT_CALF:
		case HITBOX_LEFT_THIGH:
		case HITBOX_LEFT_FOOT:
		case HITBOX_RIGHT_CALF:
		case HITBOX_RIGHT_THIGH:
		case HITBOX_RIGHT_FOOT:
			//        vector id | pitch/yaw/roll
			// THIS SHIT SCALABLE MY NIGGA
			// 
			// here we get the position of the most right, left, top and bottom point
			multipoints.at(idx)[0] += (flOnePerc * flScale); // left ear, left point
			multipoints.at(idx + 1)[0] -= (flOnePerc * flScale); // right ear, right point
			multipoints.at(idx + 2)[2] += (flOnePerc * flScale); // forehead, highest point
			multipoints.at(idx + 3)[2] -= (flOnePerc * flScale); // chin, lowest point
			break;
		}
		idx++;
	}

	return;
}

Vec3D RageBot::ExtrapolatePlayer(Player* pTarget, Vec3D vAimPos) {
	return vAimPos + (pTarget->vGetVelocity() * g_Interface.pGlobalVars->flIntervalPerTick);
}

LagRecord* RageBot::GetBestRecord(Player* pTarget) {
	int iRecord = INVALID;
	float flBestDmg = g_Config.arrints[XOR("ragedmg")].val[iActiveWeapon];

	int iTargetIndex = pTarget->iIndex();

	for (int i = 0; i < g_Backtrack.deqLagRecords[iTargetIndex].size(); i++) {
		LagRecord record = g_Backtrack.deqLagRecords[iTargetIndex][i];
		if (!g_Backtrack.ValidTick(record))
			continue;

		Vec3D vAngle;
		Vec3D vHitboxPos = record.GetHitboxPos(HITBOX_HEAD).vPos;

		FireBulletData_t t;
		float flDamage = g_Penetration.GetDamage(Game::g_pLocal, vHitboxPos, t);

		if (flDamage > flBestDmg) {
			flBestDmg = flDamage;
			iRecord = i;
		}
	}

	if (iRecord != INVALID)
		return &g_Backtrack.deqLagRecords[iTargetIndex][iRecord];

	return &g_Backtrack.deqLagRecords[iTargetIndex].front();
}

bool RageBot::GunReady() {
	if (!Game::g_pLocal->pGetActiveWeapon() || Game::g_pLocal->pGetActiveWeapon()->iClip() <= 0)
		return false;

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

	LagRecord* pTargetRecord = nullptr;
	if (g_Backtrack.deqLagRecords[pTargetPlayer->iIndex()].size() < 3)
		return;

	pTargetRecord = &g_Backtrack.deqLagRecords[pTargetPlayer->iIndex()].front();
	if (g_Config.ints[XOR("lagcomp")].val)
		pTargetRecord = GetBestRecord(pTargetPlayer);

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
		if (!pActiveWeapon->iClip())
			return;
		
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

	CCSWeaponData* data = pActiveWeapon->GetWeaponData();
	if (!data)
		return;

	Vec3D vTargetHitbox = Vec3D(0.f,0.f,0.f);
	bool bTargetFound = false;
	std::vector<Vec3D> vAimPoints;

	UpdateHitboxes();
	Vec3D vAbsOrigin = pTargetPlayer->vAbsOrigin();
	pTargetPlayer->SetPosition(pTargetPlayer->vOrigin());
	
	// update player to selected lagrecord
	LagRecord backuprecord(pTargetPlayer);
	if (g_Config.ints[XOR("lagcomp")].val)
		pTargetRecord->ApplyToPlayer(pTargetPlayer);

	if (g_Config.ints[XOR("multipoint")].val)
		GetMultipoints(vAllowedHitboxes, pTargetPlayer, g_Config.floats[XOR("multipointscale")].val, vAimPoints);
	else {
		for (auto hitbox : vAllowedHitboxes)
			vAimPoints.push_back(pTargetPlayer->vGetHitboxPos(hitbox));
	}

	if (g_Config.ints[XOR("autowall")].val) {
		float flBestDamage = (float)g_Config.arrints[XOR("ragedmg")].val[iActiveWeapon];
		for (auto hitbox : vAimPoints) {
			FireBulletData_t data;
			float flDamage = g_Penetration.GetDamage(Game::g_pLocal, hitbox, data);
			if (flDamage > flBestDamage) {
				flBestDamage = flDamage;
				vTargetHitbox = hitbox;
				bTargetFound = true;
			}
		}
	}
	else {
		float flBestDamage = (float)g_Config.arrints[XOR("ragedmg")].val[iActiveWeapon];
		for (auto hitbox : vAimPoints) {
			if (pTargetPlayer->bIsPointVisible(Game::g_pLocal->vEyeOrigin(),hitbox)) {
				float flDamage = (float)data->iDamage;
				if (flDamage > flBestDamage) {
					flBestDamage = flDamage;
					vTargetHitbox = hitbox;
					bTargetFound = true;
				}
			}
		}
	}

	// restore player
	backuprecord.ApplyToPlayer(pTargetPlayer);
	pTargetPlayer->SetPosition(vAbsOrigin);

	if (!bTargetFound)
		return;

	if (!HitChance()) {
		if (g_Config.ints[XOR("autoscope")].val && !Game::g_pLocal->bIsScoped())
			cmd->buttons |= CUserCmd::IN_ZOOM;
		return;
	}

	vTargetHitbox = ExtrapolatePlayer(pTargetPlayer, vTargetHitbox);

	Vec3D vTargetAngle;
	g_Math.CalcAngle(Game::g_pLocal->vEyeOrigin(), vTargetHitbox, vTargetAngle);

	if (g_Config.ints[XOR("compensaterecoil")].val)
		vTargetAngle -= Game::g_pLocal->vGetAimPunchAngle() * 2.f;

	cmd->viewangles = vTargetAngle;

	if (!g_Config.ints[XOR("ragesilent")].val)
		g_Interface.pEngine->SetViewAngles(vTargetAngle);

	if (g_Config.ints[XOR("autoshoot")].val) {
		if (GunReady() && pActiveWeapon->iClip() > 0)
			cmd->buttons |= CUserCmd::IN_ATTACK;

		if (pActiveWeapon->iClip() == 0)
			cmd->buttons |= CUserCmd::IN_RELOAD;
	}

	g_Backtrack.ApplyRecord(cmd, pTargetRecord);
}
