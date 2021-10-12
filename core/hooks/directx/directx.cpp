#include "pch.h"
#include "includes.h"
#include "directx.h"

DirectX g_DirectX{ };

BOOL CALLBACK EnumWind(HWND handle, LPARAM lp) {
	DWORD pProcId;
	GetWindowThreadProcessId(handle, &pProcId);

	if (GetCurrentProcessId() != pProcId)
		return true;

	g_DirectX.window = handle;
	return false;
}

HWND DirectX::GetProcessWindow() {
	window = NULL;
	EnumWindows(EnumWind, NULL);

	RECT size;
	GetWindowRect(window, &size);
	iWindowWidth = size.right - size.left;
	iWindowHeight = size.bottom - size.top;

	QUERY_USER_NOTIFICATION_STATE pquns;
	SHQueryUserNotificationState(&pquns);

	g_Interface.pEngine->GetScreenSize(iWindowWidth, iWindowHeight);

	return window;
}

bool DirectX::GetD3D9Device(void** pTable, size_t size) {
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}