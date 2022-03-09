#include "pch.h"
#include "includes.h"

Chams g_Chams;

void Chams::OverrideMaterial(int iMatIndex, bool bIgnoreZ, Color col) {
	pMats.at(iMatIndex)->AlphaModulate(col.rgba[3]);
	pMats.at(iMatIndex)->ColorModulate(col.rgba[0], col.rgba[1], col.rgba[2]);

	pMats.at(iMatIndex)->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, bIgnoreZ);
	g_Interface.pModelRender->OverrideMaterial(pMats.at(iMatIndex));
}

void Chams::DrawChams(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags) {
	if (!Game::g_pLocal || !info.m_pRenderable)
		return;
	
	Entity* pEntity = info.m_pRenderable->GetIClientUnknown()->GetBaseEntity();
	if (!pEntity || !pEntity->bIsPlayer())
		return;

	Player* pPlayer = reinterpret_cast<Player*>(pEntity);

	// we have found a valid player
	if (!pPlayer->bIsAlive())
		return;

	bool bEnemy = pPlayer->bIsEnemy(Game::g_pLocal);

	// enemy chams
	if (bEnemy) {

		// backtrack chams
		if (Variables::bLagcompChams) {
			for (int i = 0; i < g_Backtrack.deqLagRecords[pPlayer->iIndex()].size(); i++) {
				if (!g_Backtrack.ValidTick(g_Backtrack.deqLagRecords[pPlayer->iIndex()][i]) || !g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].boneMat)
					continue;
				OverrideMaterial(Variables::iChamType, false, Color(255 - (i * (255 / g_Backtrack.deqLagRecords[pPlayer->iIndex()].size())), i * (255 / g_Backtrack.deqLagRecords[pPlayer->iIndex()].size()), 255, 30));
				c_oDrawModel(pEcx, pEdx, pResults, info, g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].boneMat, pFlexWeights, pFlexDelayedWeights, g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].vOrigin, flags);
			}
		}

		PostLagcompChams:

		// xqz chams
		if (Variables::bEnemyChamsInvis) {
			OverrideMaterial(Variables::iChamType, true, Variables::cInvisColor.ToPercent());
			c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		}

		g_Interface.pModelRender->OverrideMaterial(nullptr); // change overwritten material to default

		if (Variables::bEnemyChamsVis) {
			OverrideMaterial(Variables::iChamType, false, Variables::cVisColor.ToPercent());
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