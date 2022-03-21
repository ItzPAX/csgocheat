#pragma once
#include "includes.h"

namespace Variables {
	// misc
	inline bool bBunnyHop = false;
	inline bool bLagcomp = false;
	inline bool bAccurateLagcomp = false;

	// chams
	inline int iChamType = 0;

	inline bool bEnemyChamsVis = false;
	inline bool bEnemyChamsInvis = false;
	inline bool bLagcompChams = false;

	inline Color cVisColor;
	inline float flVisCol[4] = { 1.f,0.3f,0.6f,1.f };
	inline Color cInvisColor;
	inline float flInvisCol[4] = { 0.6f,0.9f,0.3f,1.f };
		   
	// visuals
	inline bool bThirdPerson = false;

	// esp
	inline bool bNameEsp = false;
	inline bool bBoxEsp = false;
	inline bool bHealthEsp = false;

	// aimbot
	inline bool bNonSticky = false;
	inline bool bDistanceBasedFov = false;
	inline bool bAimbot = false;
	inline bool bWaitAfterTargetting = false;
	inline bool bStandaloneRCS = false;
	inline float flSmoothing = 10.f;
	inline float flCorrecting = 40.f;
	inline float flReactionTime = 0.f;
	inline float flFov = 10.f;

	/*TEMPORARY FIX*/
	inline int iAllowedHitboxes[] = {HitboxChest, HitboxHead, HitboxLowerChest, HitboxPelvis, HitboxStomach, HitboxUpperChest};
}