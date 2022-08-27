#pragma once
#include "core/hooks/directx/directx.h"
#include "utilities/structures/vec2d/vec2d.h"

struct vertex {
	FLOAT x, y, z, rhw;
	DWORD color;
};

class Render {
private:
	inline static ID3DXLine* g_pLine;
	inline static LPDIRECT3DVERTEXBUFFER9 g_pVB;    // Buffer to hold vertices
	inline static LPDIRECT3DINDEXBUFFER9  g_pIB;    // Buffer to hold indices

	// fonts
public:
	inline static LPD3DXFONT pEspFont;

public:
	inline static bool bInitialized = false;

public:
	static bool InitRenderer();
	static void DrawFilledRect(int x, int y, int w, int h, Color _col);
	static void DrawLine(int x1, int y1, int x2, int y2, int width, Color _col);
	static void DrawLine(Vec2D src, Vec2D dst, int width, Color _col);	// overload to use vectors
	static void DrawOutlinedRect(int x, int y, int w, int h, Color _col);
	static void Text(LPD3DXFONT font, const char* text, float x, float y, Color _col, DWORD alignment = DT_CENTER);
	static Vec2D TextSize(LPD3DXFONT font, const char* text);
	static void TextSize(LPD3DXFONT font, const char* text, Vec2D& vec);
};

extern Render g_Render;