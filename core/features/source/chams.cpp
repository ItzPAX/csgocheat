#include "pch.h"
#include "includes.h"

Chams g_Chams;

void Chams::OverrideMaterial(int iMatIndex, float* col) {
	pMats.at(iMatIndex)->AlphaModulate(col[3]);

	if (iMatIndex < 2)
		pMats.at(iMatIndex)->ColorModulate(col[0], col[1], col[2]);

	else {
		bool bFound = false;
		auto pEnvmaptint = pMats.at(iMatIndex)->FindVar(XOR("$envmaptint"), &bFound);
		if (bFound)
			pEnvmaptint->SetVector(col[0], col[1], col[2]);
	}

	pMats.at(iMatIndex)->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, false);
	g_Interface.pStudioRender->ForcedMaterialOverride(pMats.at(iMatIndex));
}

void Chams::OverrideMaterial(int iMatIndex, Color col) {
	pMats.at(iMatIndex)->AlphaModulate(col.rgba[3]);
	pMats.at(iMatIndex)->ColorModulate(col.rgba[0], col.rgba[1], col.rgba[2]);

	pMats.at(iMatIndex)->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, false);
	g_Interface.pStudioRender->ForcedMaterialOverride(pMats.at(iMatIndex));
}

void Chams::DrawChams(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags) {
	if (!Game::g_pLocal || !info.m_pRenderable)
		return;

	if (pMats.size() < g_Config.ints[XOR("chamtype")].val)
		return;

	if (strstr(info.m_pStudioHdr->cNameCharArray, XOR("arms")) && g_Config.ints[XOR("handchams")].val) {
		OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("handcol")].val);
		c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}

	IMatRenderContext* pRenderContext{ g_Interface.pMaterialSystem->GetRenderContext() };
	if (!pRenderContext)
		return;

	Entity* pEntity = info.m_pRenderable->GetIClientUnknown()->GetBaseEntity();
	// ent is valid
	if (!pEntity)
		return;

	// we have a valid player
	if (!pEntity->bIsPlayer())
		return;

	Player* pPlayer = reinterpret_cast<Player*>(pEntity);

	// the player is alive
	if (!pPlayer->bIsAlive())
		return;

	bool bEnemy = pPlayer->bIsEnemy(Game::g_pLocal);

	// enemy chams
	if (bEnemy) {
		// backtrack chams
		if (g_Config.ints[XOR("lagcompchams")].val && g_Backtrack.deqLagRecords[pPlayer->iIndex()].size() > 3) {
			if (g_Config.ints[XOR("lagcompchamstype")].val == 1) {
				for (int i = 0; i < g_Backtrack.deqLagRecords[pPlayer->iIndex()].size(); i++) {
					if (!g_Backtrack.ValidTick(g_Backtrack.deqLagRecords[pPlayer->iIndex()][i]) || !g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].boneMat)
						continue;
					OverrideMaterial(iRenderedChamType, Color(255 - (i * (255 / g_Backtrack.deqLagRecords[pPlayer->iIndex()].size())), i * (255 / g_Backtrack.deqLagRecords[pPlayer->iIndex()].size()), 255.f, 30.f).ToPercent());
					c_oDrawModel(pEcx, pEdx, pResults, info, g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].boneMat, pFlexWeights, pFlexDelayedWeights, g_Backtrack.deqLagRecords[pPlayer->iIndex()][i].vOrigin, flags);
				}
			}
			else {
				OverrideMaterial(iRenderedChamType, Color(0.f, 255.f, 255.f, 30.f).ToPercent());
				c_oDrawModel(pEcx, pEdx, pResults, info, g_Backtrack.deqLagRecords[pPlayer->iIndex()].back().boneMat, pFlexWeights, pFlexDelayedWeights, g_Backtrack.deqLagRecords[pPlayer->iIndex()].back().vOrigin, flags);
			}
		}

		// vis chams
		if (g_Config.ints[XOR("enemychamsvis")].val) {
			OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("enemyviscol")].val);
			
			if (g_Config.ints[XOR("enemychamsinvis")].val)
				pRenderContext->SetStencilState(GenerateStenctilState(false));

			c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

			// xqz chams
			if (g_Config.ints[XOR("enemychamsinvis")].val) {
				g_Interface.pStudioRender->ForcedMaterialOverride(nullptr);

				OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("enemyinviscol")].val);
				pRenderContext->SetStencilState(GenerateStenctilState(true));

				//Set depth
				pRenderContext->DepthRange(0.f, 0.02f);
				c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
				pRenderContext->DepthRange(0.f, 1.f);
			}
		}
	}

	// team chams
	if (!bEnemy && pPlayer != Game::g_pLocal) {
		// vis chams
		if (g_Config.ints[XOR("friendlychamsvis")].val) {
			OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("friendlyviscol")].val);
			
			if (g_Config.ints[XOR("friendlychamsinvis")].val)
				pRenderContext->SetStencilState(GenerateStenctilState(false));

			c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

			// xqz chams
			if (g_Config.ints[XOR("friendlychamsinvis")].val) {
				g_Interface.pStudioRender->ForcedMaterialOverride(nullptr);

				OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("friendlyinviscol")].val);
				pRenderContext->SetStencilState(GenerateStenctilState(true));

				//Set depth
				pRenderContext->DepthRange(0.f, 0.02f);
				c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
				pRenderContext->DepthRange(0.f, 1.f);
			}
		}
	}

	// local chams
	if (pPlayer == Game::g_pLocal) {
		// vis chams
		if (g_Config.ints[XOR("localchams")].val) {
			OverrideMaterial(iRenderedChamType, g_Config.arrfloats[XOR("localcol")].val);
			pRenderContext->SetStencilState(GenerateStenctilState(false));

			c_oDrawModel(pEcx, pEdx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		}
	}
}