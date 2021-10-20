#pragma once
#include "core/hooks/directx/directx.h"
#include "utilities/structures/vec2d/vec2d.h"

class Render {
private:
	ID3DXLine* g_pLine;

public:
	inline static LPD3DXFONT pEspFont;

public:
	static bool InitRenderer();
	static void DrawFilledRect(int x, int y, int w, int h, Color _col);
	static void DrawLine(int x1, int y1, int x2, int y2, int width, Color _col);
	static void DrawLine(Vec2D src, Vec2D dst, int width, Color _col);	// overload to use vectors
	static void DrawOutlinedRect(int x, int y, int w, int h, Color _col);
	static void Text(LPD3DXFONT font, const char* text, float x, float y, int scale, Color _col);
	static Vec2D TextSize(LPD3DXFONT font, const char* text);
	static void TextSize(LPD3DXFONT font, const char* text, Vec2D& vec);
};

extern Render g_Render;