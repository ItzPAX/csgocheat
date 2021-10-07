#include "../../includes.h"
#include "../../pch/pch.h"

Init g_Init{ };

ulong __stdcall Init::OnInject(void* p) {
	return 1;
}