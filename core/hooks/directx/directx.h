#pragma once
#include "includes.h"

class DirectX {
public:
	bool GetD3D9Device(void** pTable, size_t size);
	bool IsWindowFullscreen(HWND windowHandle);
	HWND GetProcessWindow();
};

// extern vars and class for hooking and drawing
extern DirectX g_DirectX;
static HWND window;
static LPDIRECT3DDEVICE9 pDevice = nullptr;
extern inline int iWindowHeight = 0, iWindowWidth = 0;