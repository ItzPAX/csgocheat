#pragma once
#include "pch.h"
#include "includes.h"
#include "core/hooks/directx/directx.h"

namespace Render {
	__forceinline void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col) {
		D3DRECT rect = { x,y,x + w, y + h };
		g_DirectX.pDevice->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);
	}

	__forceinline void DrawLine(int x1, int y1, int x2, int y2, int width, D3DCOLOR col) {
		ID3DXLine* LineL;
		D3DXCreateLine(g_DirectX.pDevice, &LineL);

		D3DXVECTOR2 Line[2];
		Line[0] = D3DXVECTOR2(x1, y1);
		Line[1] = D3DXVECTOR2(x2, y2);
		LineL->SetWidth(width);
		LineL->Draw(Line, 2, col);
		LineL->Release();
	}
}