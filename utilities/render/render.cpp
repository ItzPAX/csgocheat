#include "pch.h"
#include "render.h"

Render g_Render;

bool Render::InitRenderer() {
	// Init fonts here
	return true;
}

void Render::DrawFilledRect(int x, int y, int w, int h, Color _col) {
	D3DCOLOR _Col = D3DCOLOR_ARGB((int)_col.a, (int)_col.r, (int)_col.g, (int)_col.b);

	Vertex V[4] = { {x, y + h, 0.0f, 0.0f, _Col}, { x, y, 0.0f, 0.0f, _Col }, { x + w, y + h, 0.0f, 0.0f, _Col }, { x + w, y, 0.0f, 0.0f, _Col } };
	g_DirectX.pDevice->SetTexture(0, NULL);
	g_DirectX.pDevice->SetPixelShader(0);
	g_DirectX.pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_DirectX.pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_DirectX.pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_DirectX.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_DirectX.pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_DirectX.pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
	return;
}

void Render::DrawLine(int x1, int y1, int x2, int y2, int width, Color _col) {
	if (!g_Render.g_pLine)
		D3DXCreateLine(g_DirectX.pDevice, &g_Render.g_pLine);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);

	g_Render.g_pLine->SetWidth(width);
	g_Render.g_pLine->Draw(Line, 2, D3DCOLOR_ARGB((int)_col.a, (int)_col.r, (int)_col.g, (int)_col.b));
}

void Render::DrawLine(Vec2D src, Vec2D dst, int width, Color _col) {
	DrawLine(src.x, src.y, dst.x, dst.y, width, _col);
}

void Render::DrawOutlinedRect(int x, int y, int w, int h, Color _col) {
	DrawLine(x, y, x + w, y, 1, _col);
	DrawLine(x, y, x, y + h, 1, _col);
	DrawLine(x + w, y, x + w, y + h, 1, _col);
	DrawLine(x, y + h, x + w, y + h, 1, _col);
}

void Render::Text(LPD3DXFONT font, const char* text, float x, float y, int scale, Color _col) {
	if (!font)
		D3DXCreateFont(g_DirectX.pDevice, 10, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH || FF_DONTCARE, "Visitor TT2 (BRK)", &g_Render.pEspFont);

	RECT rect;

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	g_Render.pEspFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB((int)_col.a, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	g_Render.pEspFont->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB((int)_col.a, (int)_col.r, (int)_col.g, (int)_col.b));
}

Vec2D Render::TextSize(LPD3DXFONT font, const char* text) {
	if (!g_DirectX.pDevice || !font || !text)
		return Vec2D();

	RECT rect = { 0,0,0,0 };
	g_Render.pEspFont->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT, D3DCOLOR_ARGB(0,0,0,0));

	return Vec2D(rect.right - rect.left, rect.bottom - rect.top);
}

void Render::TextSize(LPD3DXFONT font, const char* text, Vec2D& vec) {
	vec = TextSize(font, text);
}