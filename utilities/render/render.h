#pragma once
#include "includes.h"
#include "pch.h"

#define CIRCLEPOINTS 64

struct Vertex {
	float x, y, z, rhw;
	uint32_t color;
};

namespace Render {
	__forceinline void DrawFilledRect(Vec2D pos, Vec2D size, D3DCOLOR col, LPDIRECT3DDEVICE9 pDevice) {
		D3DRECT rect = { pos.x,pos.y,pos.x + size.x, pos.y + size.y };
		pDevice->Clear(1, &rect, D3DCLEAR_TARGET, col, 0, 0);
	}
}