#include "includes.h"
#include "pch.h"

#include "sdk/manager/interface/classes/IRenderView.h"

PreviewModel g_PrevModel;

std::array < Vec4D, 6 > aWhiteArray =
{
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
	Vec4D(0.4f, 0.4f, 0.4f, 1.0f),
};

void PreviewModel::Instance()
{
	if (!g_Menu.bToggled)
		return;

	if (!m_PreviewTexture)
	{
		g_Interface.pMaterialSystem->BeginRenderTargetAllocation();

		m_PreviewTexture = g_Interface.pMaterialSystem->CreateRenderTargetFF(
			XOR("Preview"),
			350, 575,
			RT_SIZE_NO_CHANGE,
			g_Interface.pMaterialSystem->GetBackBufferFormat(),
			MATERIAL_RT_DEPTH_SHARED,
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			CREATERENDERTARGETFLAGS_HDR
		);

		g_Interface.pMaterialSystem->FinishRenderTargetAllocation();
	}

	if (!m_CubemapTexture)
		m_CubemapTexture = g_Interface.pMaterialSystem->FindTexture(XOR("editor/cubemap.hdr"), XOR(TEXTURE_GROUP_CUBE_MAP));

	static auto pCreateModel = g_Tools.SignatureScan(XOR("client.dll"), XOR("\x53\x8B\xD9\x56\x57\x8D\x4B\x04\xC7\x03\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A"), XOR("xxxxxxxxxx????x????x"));
	auto CreateModel = reinterpret_cast<void(__thiscall*)(void*)>(pCreateModel);
	if (!m_PreviewModel)
	{
		m_PreviewModel = static_cast<C_MergedMDL*>(g_Interface.pMemAlloc->Alloc(0x75C));
		CreateModel(m_PreviewModel);

		m_PreviewModel->SetMDL(XOR("models/player/custom_player/uiplayer/animset_uiplayer.mdl"));
		m_PreviewModel->SetMergedMDL(XOR("models/player/custom_player/legacy/ctm_fbi_variantb.mdl"));
		m_PreviewModel->SetMergedMDL(XOR("models/weapons/w_snip_awp.mdl"));

		m_PreviewModel->SetSequence(119, false);
		m_PreviewModel->SetupBonesForAttachmentQueries();
	}

	m_PreviewModel->RootMDL.flTime += g_Interface.pGlobalVars->flFrameTime / 2.0f;

	m_ViewSetup.iX = 0;
	m_ViewSetup.iY = 0;
	m_ViewSetup.iWidth = 350;
	m_ViewSetup.iHeight = 575;
	m_ViewSetup.bOrtho = false;
	m_ViewSetup.flFOV = 70.f;
	m_ViewSetup.vecOrigin = Vec3D(-65.0f, 2.0f, 50);
	m_ViewSetup.angView = Vec3D(0, 0, 0);
	m_ViewSetup.flNearZ = 7.0f;
	m_ViewSetup.flFarZ = 1000.f;
	m_ViewSetup.bDoBloomAndToneMapping = true;

	CMatRenderContextPtr pRenderContext(g_Interface.pMaterialSystem);

	pRenderContext->PushRenderTargetAndViewport();
	pRenderContext->SetRenderTarget(m_PreviewTexture);

	pRenderContext->BindLocalCubemap(m_CubemapTexture);
	pRenderContext->SetLightingOrigin(-65.0f, 2.0f, 50.0f);
	pRenderContext->SetIntRenderingParameter(10, 0);

	Frustum_t dummyFrustum;
	g_Interface.pRenderView->Push3DView(pRenderContext, m_ViewSetup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL, m_PreviewTexture, dummyFrustum);

	pRenderContext->ClearColor4ub(false, false, false, false);
	pRenderContext->ClearBuffers(true, true, true);
	pRenderContext->SetAmbientLightCube(aWhiteArray.data());

	g_Interface.pStudioRender->SetAmbientLightColors(aWhiteArray.data());
	g_Interface.pStudioRender->SetLocalLights(0, nullptr);

	Matrix matPlayerView = { };
	g_Math.AngleMatrix(Vec3D(0, -180.f * sin(g_Interface.pGlobalVars->flCurTime / 2), 0), matPlayerView, Vec3D(0, 0, 0));

	g_Interface.pModelRender->SuppressEngineLighting(true);
	bDrawingModel = true;
	if (g_ChamCreator.pPreviewMaterial != g_ChamCreator.pEmptyMat) {
		g_ChamCreator.pPreviewMaterial->AlphaModulate(g_ChamCreator.flPrevCol[3]);

		bool bFound = false;
		auto pEnvmaptint = g_ChamCreator.pPreviewMaterial->FindVar(XOR("$envmaptint"), &bFound);
		if (bFound)
			pEnvmaptint->SetVector(g_ChamCreator.flPrevCol[0], g_ChamCreator.flPrevCol[1], g_ChamCreator.flPrevCol[2]);

		g_ChamCreator.pPreviewMaterial->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_IGNOREZ, false);
		g_Interface.pStudioRender->ForcedMaterialOverride(g_ChamCreator.pPreviewMaterial);
	}
	else
		g_Interface.pStudioRender->ForcedMaterialOverride(nullptr);

	m_PreviewModel->Draw(matPlayerView); /*FIX: Call Instance() in PaintTraverse*/
	g_Interface.pStudioRender->ForcedMaterialOverride(nullptr);

	bDrawingModel = false;
	g_Interface.pModelRender->SuppressEngineLighting(false);

	g_Interface.pRenderView->PopView(pRenderContext, dummyFrustum);
	pRenderContext->BindLocalCubemap(nullptr);

	pRenderContext->PopRenderTargetAndViewport();
	pRenderContext->Release();
}

void C_MergedMDL::SetupBonesForAttachmentQueries()
{
	static auto pSetupBonesForAttachmentQueries = g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xEC\x14\x83\x3D\x00\x00\x00\x00\x00\x53"), XOR("xxxxxxxx?????x"));
	return ((void(__thiscall*)(void*))(pSetupBonesForAttachmentQueries))(this);
}

void C_MergedMDL::SetMergedMDL(const char* szModelPath, CCustomMaterialOwner* pCustomMaterialOwner, void* pProxyData)
{
	static auto pSetMergedMDL = g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x57\x8B\xF9\x8B\x0D\x00\x00\x00\x00\x85\xC9\x75"), XOR("xxxxxxxx????xxx"));
	return ((void(__thiscall*)(void*, const char*, CCustomMaterialOwner*, void*, bool))(pSetMergedMDL))(this, szModelPath, pCustomMaterialOwner, pProxyData, false);
}