#pragma once
#include "utilities/structures/color/color.h"

#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24

struct ViewPlane_t
{
	ViewPlane_t() = default;

	ViewPlane_t(const Vec3D& vecNormal, const float flDistance) :
		vecNormal(vecNormal), flDistance(flDistance) { }

	Vec3D vecNormal;
	float flDistance;
};

enum
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_TOP = 2,
	FRUSTUM_BOTTOM = 3,
	FRUSTUM_NEARZ = 4,
	FRUSTUM_FARZ = 5,
	FRUSTUM_NUMPLANES = 6
};

using Frustum_t = ViewPlane_t[FRUSTUM_NUMPLANES];
class CViewSetup;
class WorldListInfo_t;

class IRenderView {
public:
	virtual void			DrawBrushModel(Entity* pBaseEntity, Model* pModel, const Vec3D& vecOrigin, const Vec3D& angView, bool bSort) = 0;
	virtual void			DrawIdentityBrushModel(void* pList, Model* pModel) = 0;
	virtual void			TouchLight(struct dlight_t* dLight) = 0;
	virtual void			Draw3DDebugOverlays() = 0;
	virtual void			SetBlend(float flBlend) = 0;
	virtual float			GetBlend() = 0;
	virtual void			SetColorModulation(float const* flBlend) = 0;
	virtual void			GetColorModulation(float* flBlend) = 0;
	virtual void			SceneBegin() = 0;
	virtual void			SceneEnd() = 0;
	virtual void			GetVisibleFogVolume(const Vec3D& vecEyePoint, void* pInfo) = 0;
	virtual void*			CreateWorldList() = 0;
	virtual void			BuildWorldLists_Epilogue(void* pList, WorldListInfo_t* pInfo, bool bShadowDepth) = 0;
	virtual void			BuildWorldLists(void* pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const void* pVisData = nullptr, bool bShadowDepth = false, float* pReflectionWaterHeight = nullptr) = 0;
	virtual void			DrawWorldLists(void* pList, unsigned long flags, float flWaterZAdjust) = 0;
	virtual void			GetWorldListIndicesInfo(void* pIndicesInfoOut, void* pList, unsigned long nFlags) = 0;
	virtual void			DrawTopView(bool bEnable) = 0;
	virtual void			TopViewNoBackfaceCulling(bool bDisable) = 0;
	virtual void			TopViewNoVisCheck(bool bDisable) = 0;
	virtual void			TopViewBounds(Vec2D const& vecMins, Vec2D const& vecMaxs) = 0;
	virtual void			SetTopViewVolumeCuller(const void* pVolumeCuller) = 0;
	virtual void			DrawLights() = 0;
	virtual void			DrawMaskEntities() = 0;
	virtual void			DrawTranslucentSurfaces(void* pList, int* pSortList, int iSortCount, unsigned long fFlags) = 0;
	virtual void			DrawLineFile() = 0;
	virtual void			DrawLightmaps(void* pList, int iPageID) = 0;
	virtual void			ViewSetupVis(bool bNoVis, int nOrigins, const Vec3D vecOrigin[]) = 0;
	virtual bool			AreAnyLeavesVisible(int* pLeafList, int nLeaves) = 0;
	virtual	void			VguiPaint() = 0;
	virtual void			ViewDrawFade(uint8_t* pColor, IMaterial* pMaterial) = 0;
	virtual void			OLD_SetProjectionMatrix(float flFov, float zNear, float zFar) = 0;
	virtual Vec3D			GetLightAtPoint(Vec3D& vecPosition) = 0;
	virtual int				GetViewEntity() = 0;
	virtual bool			IsViewEntity(int nEntityIndex) = 0;
	virtual float			GetFieldOfView() = 0;
	virtual unsigned char** GetAreaBits() = 0;
	virtual void			SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;
	virtual void			InstallBrushSurfaceRenderer(void* pBrushRenderer) = 0;
	virtual void			DrawBrushModelShadow(IClientRenderable* pRenderable) = 0;
	virtual	bool			LeafContainsTranslucentSurfaces(void* pList, int nSortIndex, unsigned long fFlags) = 0;
	virtual bool			DoesBoxIntersectWaterVolume(const Vec3D& vecMins, const Vec3D& vecMaxs, int nLeafWaterDataID) = 0;
	virtual void			SetAreaState(unsigned char uAreaBits[MAX_AREA_STATE_BYTES], unsigned char uAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES]) = 0;
	virtual void			VGui_Paint(int nMode) = 0;
	virtual void			Push3DView(IMatRenderContext* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, Frustum_t frustumPlanes) = 0;
	virtual void			Push2DView(IMatRenderContext* pRenderContext, const CViewSetup& view, int nFlags, ITexture* pRenderTarget, Frustum_t frustumPlanes) = 0;
	virtual void			PopView(IMatRenderContext* pRenderContext, Frustum_t frustumPlanes) = 0;
	virtual void			SetMainView(const Vec3D& vecOrigin, const Vec3D& angView) = 0;
	virtual void			ViewSetupVisEx(bool bNoVis, int nOrigins, const Vec3D arrOrigin[], unsigned int& uReturnFlags) = 0;
	virtual void			OverrideViewFrustum(Frustum_t custom) = 0;
	virtual void			DrawBrushModelShadowDepth(IClientEntity* pEntity, Model* pModel, const Vec3D& vecOrigin, const Vec3D& angView, int nDepthMode) = 0;
	virtual void			UpdateBrushModelLightmap(Model* pModel, IClientRenderable* pRenderable) = 0;
	virtual void			BeginUpdateLightmaps() = 0;
	virtual void			EndUpdateLightmaps() = 0;
	virtual void			OLD_SetOffCenterProjectionMatrix(float flFOV, float flNearZ, float flFarZ, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight) = 0;
	virtual void			OLD_SetProjectionMatrixOrtho(float flLeft, float flTop, float flRight, float flBottom, float flNearZ, float flFarZ) = 0;
	virtual void			PadFn0() = 0;
	virtual void			GetMatricesForView(const CViewSetup& view, void* pWorldToView, void* pViewToProjection, void* pWorldToProjection, void* pWorldToPixels) = 0;
	virtual void			DrawBrushModelEx(IClientEntity* pEntity, Model* pModel, const Vec3D& vecOrigin, const Vec3D& angView, int nMode) = 0;
	virtual bool			DoesBrushModelNeedPowerOf2Framebuffer(const Model* pModel) = 0;
	virtual void			DrawBrushModelArray(IMatRenderContext* pContext, int nCount, const void* pInstanceData, int nModelTypeFlags) = 0;
};