#pragma once
#include "includes.h"

namespace Variables {
	// chams
	inline int iChamType = 0;

	inline bool bEnemyChamsVis = true;
	inline bool bEnemyChamsInvis = true;

	inline Color cVisColor;
	inline float flVisCol[4] = { 1.f,0.3f,0.6f,1.f };
	inline Color cInvisColor;
	inline float flInvisCol[4] = { 0.6f,0.9f,0.3f,1.f };
		   
	// esp
	inline bool bNameEsp = true;
	inline bool bBoxEsp = true;
	inline bool bHealthEsp = true;

	// aimbot
	inline bool bNonSticky = true;
	inline bool bDistanceBasedFov = true;
	inline bool bAimbot = false;
	inline bool bWaitAfterTargetting = true;
	inline bool bStandaloneRCS = false;
	inline float flSmoothing = 10.f;
	inline float flCorrecting = 40.f;
	inline float flReactionTime = 0.f;
	inline float flFov = 10.f;

	/*TEMPORARY FIX*/
	inline int iAllowedHitboxes[] = {HitboxChest, HitboxHead, HitboxLowerChest, HitboxPelvis, HitboxStomach, HitboxUpperChest};
}