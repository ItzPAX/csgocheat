#pragma once

class Init {
public:
	// setup everything
	static ulong __stdcall OnInject(void* p);

	// unload everything
	static void OnUnload(HINSTANCE hInstance);
};

extern Init g_Init;