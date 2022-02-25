#include "pch.h"
#include "includes.h"

Chams g_Chams;

void Chams::OverrideMaterial(int iMatIndex, bool bIgnoreZ, Color col) {
	pMats.at(iMatIndex)->AlphaModulate(col.a);
	pMats.at(iMatIndex)->ColorModulate(col.r, col.g, col.b);

	pMats.at(iMatIndex)->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, bIgnoreZ);
	g_Interface.pModelRender->OverrideMaterial(pMats.at(iMatIndex));
}

void Chams::DrawChams(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags) {
	Entity* pEntity = reinterpret_cast<Entity*>(info.m_pClientEntity);

	// sanity check
	if (!pEntity) {
		return;
	}

	// we have found a player
	if (pEntity->iTeamNum() == 2 || pEntity->iTeamNum() == 3) {

		Player* pPlayer = reinterpret_cast<Player*>(pEntity);

		bool bEnemy = pPlayer->bIsEnemy(Game::g_pLocal);

		// enemy chams
		if (bEnemy) {

			// xqz chams
			if (Variables::bEnemyChamsInvis) {
				OverrideMaterial(Materials::DEFAULT, true, Color(150, 250, 100).ToPercent());
				c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
			}

			g_Interface.pModelRender->OverrideMaterial(nullptr); // change overwritten material to default

			if (Variables::bEnemyChamsVis) {
				OverrideMaterial(Materials::DEFAULT, false, Color(255, 100, 150).ToPercent());
				c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
			}
		}

		// team chams
		if (!bEnemy) {

		}

		// local chams
		if (pPlayer == Game::g_pLocal) {

		}
	}
}