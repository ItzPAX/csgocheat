#pragma once
#include <TlHelp32.h>
#include <Psapi.h>

class Tools {
private:
	MODULEINFO GetModuleInfo(const char* szModule);

public:
	void WriteToMemory(const char* addr, const char* src, int size);
	DWORD SignatureScan(const char* pModule, const char* pSig, const char* mask);
};

extern Tools g_Tools;