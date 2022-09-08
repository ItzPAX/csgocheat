#pragma once
#include "includes.h"

void cDrawModel(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags) {
	if (!g_Chams.bMatsInit) // mats havent been initialized
		return;

	g_Chams.DrawChams(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
}