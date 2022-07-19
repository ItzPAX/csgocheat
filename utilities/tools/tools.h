#pragma once
#include <TlHelp32.h>
#include <Psapi.h>

class Tools {
private:
	MODULEINFO GetModuleInfo(const char* szModule);

public:
	void WriteToMemory(const char* addr, const char* src, int size);
	DWORD SignatureScanEverywhere(const char* pSig, const char* mask);
	DWORD SignatureScan(const char* pModule, const char* pSig, const char* mask);

	template <typename T, typename U>
	T CallVirtual(U base, int index);
};

extern Tools g_Tools;