#pragma once
#include "IMaterial.h"
#include "IMaterialSystem.h"
#include "IAppSystem.h"

using Quaternion = float[4];
using RadEuler = float[3];

// forward declaration
class IMesh;
class IPooledVBAllocator;
struct LightDesc_t;

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800
#define BONE_USED_BY_VERTEX_LOD2        0x00001000
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32

//-----------------------------------------------------------------------------
// DrawModel flags
//-----------------------------------------------------------------------------
enum DrawModelFlags {
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,

	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,

	STUDIORENDER_DRAW_ACCURATETIME = 0x10,		// Use accurate timing when drawing the model.
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,

	STUDIORENDER_DRAW_WIREFRAME = 0x80,

	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,

	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,

	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,

	STUDIORENDER_GENERATE_STATS = 0x8000,
};

enum Hitgroups {
	HITGROUP_GENERIC = 0,
	HITGROUP_HEAD = 1,
	HITGROUP_CHEST = 2,
	HITGROUP_STOMACH = 3,
	HITGROUP_LEFTARN = 4,
	HITGROUP_RIGHTARM = 5,
	HITGROUP_LEFTLEG = 6,
	HITGROUP_RIGHTLEG = 7,
	HITGROUP_GEAR = 10
};

enum Modtypes {
	MOD_BAD = 0,
	MOD_BRUSH,
	MOD_SPRITE,
	MOD_STUDIO
};

enum Hitboxes {
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_STOMACH,
	HITBOX_LOWER_CHEST,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX,
	HITBOX_NONE
};

struct DrawModelResults {
	int m_ActualTriCount;
	int m_TextureMemoryBytes;
	int m_NumHardwareBones;
	int m_NumBatches;
	int m_NumMaterials;
	int m_nLODUsed;
	int m_flLODMetric;
	char pad[8];
	std::vector<IMaterial*> m_Materials;
};

struct StudioBone {
	int iNameIndex;
	inline char* const Name(void) const {
		return ((char*)this) + iNameIndex;
	}
	int iParent;
	int iBoneController[6];

	Vec3D vPos;
	Quaternion qQuat;
	RadEuler vRotation;

	Vec3D vPosScale;
	Vec3D vRotScale;

	Matrix mPoseToBone;
	Quaternion qQuatAlignement;
	int iFlags;
	int iProcType;
	int iProcIndex;
	mutable int iPhysicsBone;

	inline void* Procedure() const {
		if (iProcIndex == 0) return NULL;
		else return(void*)(((unsigned char*)this) + iProcIndex);
	};

	int iSurfacePropIdx;
	inline char* const SurfaceProp(void) const {
		return ((char*)this) + iSurfacePropIdx;
	}
	inline int GetSurfaceProp(void) const {
		return iSurfPropLookup;
	}

	int iContents;
	int iSurfPropLookup;
	int unused[7];
};

struct StudioBox {
	int iBone;
	int iGroup;
	Vec3D vMins;
	Vec3D vMaxs;
	int iNameIndex;
	int pad01[3];
	float flRadius;
	int pad02[4];
};

struct StudioHitboxSet {
	int iNameIndex;
	int iHitboxCount;
	int iHitboxIndex;

	inline char* const Name(void) const {
		return ((char*)this) + iNameIndex;
	}
	inline StudioBox* Hitbox(int i) const {
		return (StudioBox*)(((unsigned char*)this) + iHitboxIndex) + i;
	}
};

class StudioHDR {
public:
	int iId;
	int iVersion;
	long lChecksum;
	char cNameCharArray[64];
	int iLength;
	Vec3D vEyePos;
	Vec3D vIlliumPos;
	Vec3D vHullMins;
	Vec3D vHullMaxs;
	Vec3D vMins;
	Vec3D vMaxs;
	int iFlags;
	int iBonesCount;
	int iBoneIndex;
	int iBoneControllersCount;
	int iBoneControllerIndex;
	int iHitboxSetsCount;
	int iHitboxSetIndex;
	int iLocalAnimCount;
	int iLocalAnimIndex;
	int iLocalSeqCount;
	int iLocalSeqIndex;
	int iActivityListVersion;
	int iEventsIndexed;
	int iTexturesCount;
	int iTextureIndex;

	StudioHitboxSet* HitboxSet(int i) {
		if (i > iHitboxSetsCount) return nullptr;
		return (StudioHitboxSet*)((uint8_t*)this + iHitboxSetIndex) + i;
	}

	StudioBox* Hitbox(int i, int set)
	{
		StudioHitboxSet const* s = HitboxSet(set);
		if (!s) {
			return nullptr;
		}

		return s->Hitbox(i);
	}

	StudioBone* bone(int i) {
		if (i > iBonesCount) return nullptr;
		return (StudioBone*)((uint8_t*)this + iBoneIndex) + i;
	}
};

struct ColorMeshInfo_t {
	// A given color mesh can own a unique Mesh, or it can use a shared Mesh
	// (in which case it uses a sub-range defined by m_nVertOffset and m_nNumVerts)
	IMesh* m_pMesh;
	IPooledVBAllocator* m_pPooledVBAllocator;
	int						m_nVertOffsetInBytes;
	int						m_nNumVerts;
};

struct DrawModelInfo {
	StudioHDR* m_pStudioHdr;
	StudioHWData* m_pHardwareData;
	StudioDecalHandle m_iDecals;
	int m_iSkin;
	int m_iBody;
	int m_iHitboxSet;
	IClientRenderable* m_pRenderable;
};

class IStudioRender : IAppSystem {
public:
	enum EOverrideType : std::int32_t
	{
		OVERRIDE_NORMAL = 0,
		OVERRIDE_BUILD_SHADOWS,
		OVERRIDE_DEPTH_WRITE,
		OVERRIDE_SELECTIVE,
		OVERRIDE_SSAO_DEPTH_WRITE,
	};

public:
	// outdated, maybe find new funcs

	virtual void BeginFrame(void) = 0;
	virtual void EndFrame(void) = 0;

	// Used for the mat_stub console command.
	virtual void Mat_Stub(IMaterialSystem* pMatSys) = 0;

	// Updates the rendering configuration 
	virtual void PadFn0() = 0;
	virtual void PadFn1() = 0;

	// Load, unload model data
	virtual bool LoadModel(StudioHDR* pStudioHdr, void* pVtxData, StudioHWData* pHardwareData) = 0;
	virtual void UnloadModel(StudioHWData* pHardwareData) = 0;

	// Refresh the studiohdr since it was lost...
	virtual void RefreshStudioHdr(StudioHDR* pStudioHdr, StudioHWData* pHardwareData) = 0;

	// This is needed to do eyeglint and calculate the correct texcoords for the eyes.
	virtual void SetEyeViewTarget(const StudioHDR* pStudioHdr, int nBodyIndex, const Vec3D& worldPosition) = 0;

	// Methods related to lighting state
	// NOTE: SetAmbientLightColors assumes that the arraysize is the same as 
	// returned from GetNumAmbientLightSamples
	virtual int GetNumAmbientLightSamples() = 0;
	virtual const Vec3D* GetAmbientLightDirections() = 0;
	virtual void PadFn10() = 0;
	virtual void SetAmbientLightColors(const Vec3D* pAmbientOnlyColors) = 0;
	virtual void PadFn2() = 0;

	// Sets information about the camera location + orientation
	virtual void SetViewState(const Vec3D& viewOrigin, const Vec3D& viewRight,
		const Vec3D& viewUp, const Vec3D& viewPlaneNormal) = 0;

	// Allocates flex weights for use in rendering
	// NOTE: Pass in a non-null second parameter to lock delayed flex weights
	virtual void LockFlexWeights(int nWeightCount, float** ppFlexWeights, float** ppFlexDelayedWeights = NULL) = 0;
	virtual void UnlockFlexWeights() = 0;

	// Used to allocate bone matrices to be used to pass into DrawModel
	virtual Matrix* LockBoneMatrices(int nBoneCount) = 0;
	virtual void UnlockBoneMatrices() = 0;

	// LOD stuff
	virtual int GetNumLODs(const StudioHWData& hardwareData) const = 0;
	virtual float GetLODSwitchValue(const StudioHWData& hardwareData, int lod) const = 0;
	virtual void SetLODSwitchValue(StudioHWData& hardwareData, int lod, float switchValue) = 0;

	// Sets the color/alpha modulation
	virtual void SetColorModulation(float const* pColor) = 0;
	virtual void SetAlphaModulation(float flAlpha) = 0;

	// Draws the model
	virtual void DrawModel(DrawModelResults* pResults, const DrawModelInfo& info, Matrix* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vec3D& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;

	// Methods related to static prop rendering
	virtual void DrawModelStaticProp(const DrawModelInfo& drawInfo, const Matrix& modelToWorld, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL) = 0;
	virtual void DrawStaticPropDecals(const DrawModelInfo& drawInfo, const Matrix& modelToWorld) = 0;
	virtual void DrawStaticPropShadows(const DrawModelInfo& drawInfo, const Matrix& modelToWorld, int flags) = 0;

	// Causes a material to be used instead of the materials the model was compiled with
	virtual void PadFn3() = 0;

	// Create, destroy list of decals for a particular model
	virtual StudioDecalHandle CreateDecalList(StudioHWData* pHardwareData) = 0;
	virtual void DestroyDecalList(StudioDecalHandle handle) = 0;

	// Add decals to a decal list by doing a planar projection along the ray
	// The BoneToWorld matrices must be set before this is called
	virtual void PadFn4() = 0;

	// Compute the lighting at a point and normal
	virtual void ComputeLighting(const Vec3D* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vec3D& pt, const Vec3D& normal, Vec3D& lighting) = 0;

	// Compute the lighting at a point, constant directional component is passed
	// as flDirectionalAmount
	virtual void ComputeLightingConstDirectional(const Vec3D* pAmbient, int lightCount,
		LightDesc_t* pLights, const Vec3D& pt, const Vec3D& normal, Vec3D& lighting, float flDirectionalAmount) = 0;

	// Shadow state (affects the models as they are rendered)
	virtual void PadFn5() = 0; 
	virtual void ClearAllShadows() = 0;

	// Gets the model LOD; pass in the screen size in pixels of a sphere 
	// of radius 1 that has the same origin as the model to get the LOD out...
	virtual int ComputeModelLod(StudioHWData* pHardwareData, float unitSphereSize, float* pMetric = NULL) = 0;

	// Return a number that is usable for budgets, etc.
	// Things that we care about:
	// 1) effective triangle count (factors in batch sizes, state changes, etc)
	// 2) texture memory usage
	// Get Triangles returns the LOD used
	virtual void PadFn6() = 0;
	virtual void PadFn7() = 0;

	// Returns materials used by a particular model
	virtual int GetMaterialList(StudioHDR* pStudioHdr, int count, IMaterial** ppMaterials) = 0;
	virtual void PadFn8() = 0;
	virtual void PadFn9() = 0;

public:
	void ForcedMaterialOverride(IMaterial* material, int type = OVERRIDE_NORMAL, int index = -1) {
		using fn = void (__thiscall*)(void*, IMaterial*, int, int);
		(*(fn**)this)[33](this, material, type, index);
	}

	bool IsForcedMaterialOverride() {
		using fn = bool (__thiscall*)(void*);
		return (*(fn**)this)[34](this);
	}

	void SetAmbientLightColors(const Vec4D* pAmbientOnlyColors)
	{
		using fn = void(__thiscall*)(void*, const Vec4D*);
		(*(fn**)this)[20](this, pAmbientOnlyColors);
	}

	void SetLocalLights(int nLights, const LightDesc_t* pLights)
	{
		using fn = void(__thiscall*)(void*, int, const LightDesc_t*);
		(*(fn**)this)[22](this, nLights, pLights);
	}
};