#pragma once
#include "pch.h"
#include "includes.h"

namespace Render {
	__forceinline void DrawFilledRect(int x, int y, int w, int h, Color col) {
		g_Interface.pSurface->SetDrawingColor(col.r, col.g, col.b, col.a);
		g_Interface.pSurface->DrawFilledRectangle(x, y, w, h);
	}

	__forceinline void DrawLine(int x1, int y1, int x2, int y2, Color col) {
		g_Interface.pSurface->SetDrawingColor(col.r, col.g, col.b, col.a);
		g_Interface.pSurface->DrawLine(x1, y1, x2, y2);
	}
}