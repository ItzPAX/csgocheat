#pragma once

enum Materials {
	DEFAULT,
	FLAT
};

class Chams {
private:
	friend class HookManager;

private:
	// original drawmodel function
	using tDrawModel = void(__fastcall*)(void*, void*, DrawModelResults*, const DrawModelInfo&, Matrix*, float*, float*, const Vec3D&, int);
	tDrawModel c_oDrawModel = nullptr;

public:
	inline static std::vector<IMaterial*> pMats;
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

	void OverrideMaterial(int iMatIndex, bool bIgnoreZ, Color col);
	void DrawChams(void* pEcx, void* pEdx, DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL);
};

extern Chams g_Chams;