#pragma once

class PreviewModel
{
public:
	void Instance();
	ITexture* GetTexture() { return this->m_PreviewTexture; };
	void SetGlow(int glow) { this->m_iGlow = glow; };

	bool bDrawingModel = false;

private:
	ITexture* m_PreviewTexture = nullptr;
	ITexture* m_CubemapTexture = nullptr;
	C_MergedMDL* m_PreviewModel = nullptr;

	CViewSetup m_ViewSetup = { };

	int32_t m_iGlow = 0;
};

extern PreviewModel g_PrevModel;