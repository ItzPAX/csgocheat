#include "../header/antiaim.h"

AntiAim g_AntiAim;

void AntiAim::SetMode() {
	// Standing
	if (Game::g_pLocal->iFlags() & Entity::FL_ONGROUND && Game::g_pLocal->vGetVelocity().Length() < 0.1f) {
		iMode = MODE_STANDING;
		return;
	}

	if (!(Game::g_pLocal->iFlags() & Entity::FL_ONGROUND)) {
		iMode = MODE_INAIR;
		return;
	}

	if (Game::g_pLocal->vGetVelocity().Length() > 0.1f && Game::g_pLocal->iFlags() & Entity::FL_ONGROUND) {
		iMode = MODE_WALKING;
		return;
	}
}

void AntiAim::DoDesync(CUserCmd* cmd, bool bShouldSendPacket) {
	if (!g_Config.ints[XOR("antiaim")].val || g_Config.ints[XOR("trustfactor")].val)
		return;

	// paranoia mode
	if (g_Misc.iParanoiaSpecs > 0 && g_Config.ints[XOR("paranoia")].val)
		return;

	int movetype = g_NetVars.GetOffsetDirect(XOR("DT_BaseEntity"), XOR("movetype"));

	if (cmd->buttons & CUserCmd::IN_ATTACK && g_Ragebot.GunReady() ||
		!Game::g_pLocal->pGetActiveWeapon() ||
		Game::g_pLocal->pGetActiveWeapon()->GetWeaponData()->nWeaponType == Entity::WEAPONTYPE_KNIFE ||
		Game::g_pLocal->pGetActiveWeapon()->GetWeaponData()->nWeaponType == Entity::WEAPONTYPE_GRENADE ||
		cmd->buttons & CUserCmd::IN_USE ||
		!Game::g_pLocal->bIsAlive() || 
		movetype == Entity::MOVETYPE_LADDER ||
		movetype == Entity::MOVETYPE_NOCLIP)
		return;

	SetMode();

	Vec3D vEyeOrigin = Game::g_pLocal->vEyeOrigin();
	Vec3D vViewangles; g_Interface.pEngine->GetViewAngles(vViewangles);
	Player* pTarget = nullptr;
	float flOrientation = 0;

	if (g_Config.ints[XOR("attargets")].val) {
		float flBestDelta = FLT_MAX;

		// get best player
		for (int i = 1; i <= g_Interface.pGlobalVars->iMaxClients; i++) {
			Player* pPlayer = reinterpret_cast<Player*>(g_Interface.pClientEntityList->GetClientEntity(i));

			if (!pPlayer || !pPlayer->bIsPlayer() || pPlayer->bDormant() || !pPlayer->bIsAlive() || pPlayer == Game::g_pLocal || !pPlayer->bIsEnemy(Game::g_pLocal) || pPlayer->bHasGunGameImmunity())
				continue;

			Vec3D vAngle;
			Vec3D vHitboxPos = pPlayer->vGetHitboxPos(HITBOX_HEAD);
			g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
			vAngle.Clamp();

			float flCurrDelta = (vAngle - vViewangles).Clamped().Length();
			if (flCurrDelta < flBestDelta) {
				flBestDelta = flCurrDelta;
				pTarget = pPlayer;
			}
		}

		// no player found
		if (!pTarget)
			flOrientation = vViewangles.y;
		else {
			Vec3D vHitboxPos = pTarget->vGetHitboxPos(HITBOX_HEAD), vAngle;
			g_Math.CalcAngle(vEyeOrigin, vHitboxPos, vAngle);
			vAngle.Clamp();
			flOrientation = vAngle.y;
		}
	}
	else
		flOrientation = vViewangles.y;


	cmd->viewangles.x = (float)g_Config.arrints[XOR("pitch")].val[iMode] + ((bool)g_Config.arrints[XOR("jitter")].val[iMode] ? g_Math.RandomFloat(-10.f, 0.f) : 0.f);
	cmd->viewangles.y = flOrientation + (float)g_Config.arrints[XOR("yaw")].val[iMode];
	if (!bShouldSendPacket)
		cmd->viewangles.y += (float)g_Config.arrints[XOR("desyncdelta")].val[iMode] / 2;
	if (bShouldSendPacket)
		cmd->viewangles.y -= (float)g_Config.arrints[XOR("desyncdelta")].val[iMode] / 2;
}