#pragma once

class Init {
private:
	bool bAllocConsole = true;

public:
	// setup everything and place hooks here
	static ulong __stdcall OnInject(void* p);
};

extern Init g_Init;