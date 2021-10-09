#pragma once

class Init {
public:
	// setup everything
	static ulong __stdcall InitAll(void* p);

	// unload everything
	static void Unload(HINSTANCE hInstance);
};

extern Init g_Init;