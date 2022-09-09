#pragma once

class Chams {
private:
	friend class HookManager;

private:
	inline static std::vector<IMaterial*> pMats;

	ShaderStencilState_t GenerateStenctilState(bool bIgnoreZ, bool disable = false) {
		ShaderStencilState_t state;

		if (disable)
			return state;

		if (!bIgnoreZ) {
			state.m_bEnable = true;
			state.m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
			state.m_PassOp = SHADER_STENCILOP_SET_TO_REFERENCE;
			state.m_FailOp = SHADER_STENCILOP_KEEP;
			state.m_ZFailOp = SHADER_STENCILOP_KEEP;
			state.m_nReferenceValue = 1;
			state.m_nWriteMask = 0xFF;
			state.m_nTestMask = 0x0;
			return state;
		}

		state.m_bEnable = true;
		state.m_CompareFunc = SHADER_STENCILFUNC_EQUAL;
		state.m_PassOp = SHADER_STENCILOP_KEEP;
		state.m_FailOp = SHADER_STENCILOP_KEEP;
		state.m_ZFailOp = SHADER_STENCILOP_KEEP;
		state.m_nReferenceValue = 0;
		state.m_nWriteMask = 0x0;
		state.m_nTestMask = 0xFF;
		return state;
	}

public:
	// original drawmodel function
	using tDrawModel = void(__fastcall*)(void*, void*, DrawModelResults*, const DrawModelInfo&, Matrix*, float*, float*, const Vec3D&, int);
	tDrawModel c_oDrawModel = nullptr;

	enum Materials {
		DEFAULT,
		FLAT
	};

	int iChamsMode = 0;

	inline static bool bMatsInit = false;

	bool InitMaterials() {
		IMaterial* pMat;
		pMat = g_Interface.pMaterialSystem->FindMaterial(XOR("debug/debugambientcube"), TEXTURE_GROUP_OTHER);
		pMats.push_back(pMat);
		pMat->IncrementReferenceCount();

		pMat = g_Interface.pMaterialSystem->FindMaterial(XOR("debug/debugdrawflat"), TEXTURE_GROUP_OTHER);
		pMats.push_back(pMat);
		pMat->IncrementReferenceCount();


		bMatsInit = true;
		return true;
	}

	void OverrideMaterial(int iMatIndex, float* col);
	void OverrideMaterial(int iMatIndex, Color col);
	void DrawChams(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL);
};

extern Chams g_Chams;