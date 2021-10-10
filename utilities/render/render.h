#pragma once
#include "pch.h"
#include "includes.h"

namespace Render {
	__forceinline void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col, LPDIRECT3DDEVICE9 pDevice) {
		D3DRECT rect = { x,y,x + w, y + h };
		pDevice->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);
	}
}