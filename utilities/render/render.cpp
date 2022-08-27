#include "pch.h"
#include "render.h"

Render g_Render;

bool Render::InitRenderer() {
	// init d3dx line
	D3DXCreateLine(g_DirectX.pDevice, &g_Render.g_pLine);

	// init d3dx fonts
	D3DXCreateFont(g_DirectX.pDevice, 12, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_ROMAN, "Tahoma", &g_Render.pEspFont);

	g_Render.bInitialized = true;
	return true;
}

void Render::DrawFilledRect(int x, int y, int w, int h, Color _col) {
	D3DCOLOR _Col = D3DCOLOR_ARGB((int)_col.rgba[3], (int)_col.rgba[0], (int)_col.rgba[1], (int)_col.rgba[2]);

	vertex V[4];

	V[0].color = V[1].color = V[2].color = V[3].color = _Col;

	V[0].z = V[1].z = V[2].z = V[3].z = 0;
	V[0].rhw = V[1].rhw = V[2].rhw = V[3].rhw = 0;

	V[0].x = x;
	V[0].y = y;
	V[1].x = x + w;
	V[1].y = y;
	V[2].x = x + w;
	V[2].y = y + h;
	V[3].x = x;
	V[3].y = y + h;

	unsigned short indexes[] = { 0, 1, 3, 1, 2, 3 };

	g_DirectX.pDevice->CreateVertexBuffer(4 * sizeof(vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL);
	g_DirectX.pDevice->CreateIndexBuffer(2 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

	VOID* pVertices;
	g_pVB->Lock(0, sizeof(V), (void**)&pVertices, 0);
	memcpy(pVertices, V, sizeof(V));
	g_pVB->Unlock();

	VOID* pIndex;
	g_pIB->Lock(0, sizeof(indexes), (void**)&pIndex, 0);
	memcpy(pIndex, indexes, sizeof(indexes));
	g_pIB->Unlock();

	g_DirectX.pDevice->SetTexture(0, NULL);
	g_DirectX.pDevice->SetPixelShader(NULL);
	g_DirectX.pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_DirectX.pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_DirectX.pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_DirectX.pDevice->SetStreamSource(0, g_pVB, 0, sizeof(vertex));
	g_DirectX.pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_DirectX.pDevice->SetIndices(g_pIB);

	g_DirectX.pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	g_pVB->Release();
	g_pIB->Release();
}

void Render::DrawLine(int x1, int y1, int x2, int y2, int width, Color _col) {
	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);

	g_Render.g_pLine->SetWidth(width);
	g_Render.g_pLine->Draw(Line, 2, D3DCOLOR_ARGB((int)_col.rgba[3], (int)_col.rgba[0], (int)_col.rgba[1], (int)_col.rgba[2]));
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

void Render::Text(LPD3DXFONT font, const char* text, float x, float y, Color _col, DWORD alignment) {
	RECT rect;

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	font->DrawTextA(NULL, text, -1, &rect, alignment | DT_NOCLIP, D3DCOLOR_ARGB((int)_col.rgba[3], 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	font->DrawTextA(NULL, text, -1, &rect, alignment | DT_NOCLIP, D3DCOLOR_ARGB((int)_col.rgba[3], (int)_col.rgba[0], (int)_col.rgba[1], (int)_col.rgba[2]));
}

Vec2D Render::TextSize(LPD3DXFONT font, const char* text) {
	if (!font)
		return Vec2D(0.f, 0.f);

	RECT rect = { 0,0,0,0 };
	font->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT, D3DCOLOR_ARGB(0,0,0,0));

	return Vec2D(rect.right - rect.left, rect.bottom - rect.top);
}

void Render::TextSize(LPD3DXFONT font, const char* text, Vec2D& vec) {
	vec = TextSize(font, text);
}