#include "includes.h"

// here we can draw the shit
void cheatEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	Render::DrawFilledRect(Vec2D(10, 10), Vec2D(50, 50), D3DCOLOR_ARGB(255, 255, 255, 255), o_pDevice);
}