#pragma once
#include "includes.h"

namespace Variables {
	// chams
	inline bool bEnemyChamsVis = true;
	inline bool bEnemyChamsInvis = true;
		   
	// esp
	inline bool bNameEsp = true;
	inline bool bBoxEsp = true;
	inline bool bHealthEsp = true;

	// aimbot
	inline bool bNonSticky = true;
	inline bool bDistanceBasedFov = true;
	inline bool bAimbot = false;
	inline float flSmoothing = 10.f;
	inline float flCorrecting = 40.f;
	inline float flFov = 10.f;

	/*TEMPORARY FIX*/
	inline int iAllowedHitboxes[] = {HitboxChest, HitboxHead, HitboxLowerChest, HitboxPelvis, HitboxStomach, HitboxUpperChest};
}