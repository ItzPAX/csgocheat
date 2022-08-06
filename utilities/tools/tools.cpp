#include "pch.h"
#include "includes.h"

Tools g_Tools{ };

MODULEINFO Tools::GetModuleInfo(const char* szModule) {
	MODULEINFO modInfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modInfo;

	
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

//Example of how to use syscalls instead of winapi:
void Tools::WriteToMemory(const char* addr, const char* src, int size) {
	DWORD oProc;

	HANDLE hProc = GetCurrentProcess();
	NtProtectVirtualMemory(hProc, (LPVOID*)addr, (SIZE_T*)size, PAGE_EXECUTE_READWRITE, &oProc);		//VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy((LPVOID)addr, src, size);
	NtProtectVirtualMemory(hProc, (LPVOID*)addr, (SIZE_T*)size, oProc, &oProc);							//VirtualProtect((LPVOID)addr, size, oProc, &oProc);
}

DWORD Tools::SignatureScanEverywhere(const char* pSig, const char* mask) {
	MODULEINFO mInfo = GetModuleInfo(NULL);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++) {
		bool bFound = true;
		for (DWORD j = 0; j < patternLength; j++) {
			bFound &= mask[j] == '?' || pSig[j] == *(char*)(base + i + j);
		}

		if (bFound)
			return base + i;
	}
	
	return 0;
}

DWORD Tools::SignatureScan(const char* pModule, const char* pSig, const char* mask) {
	MODULEINFO mInfo = GetModuleInfo(pModule);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++) {
		bool bFound = true;
		for (DWORD j = 0; j < patternLength; j++) {
			bFound &= mask[j] == '?' || pSig[j] == *(char*)(base + i + j);
		}

		if (bFound)
			return base + i;
	}
}

template<typename T, typename U>
T Tools::CallVirtual(U base, int index) {
	using original_fn = T & (__thiscall*)(void*);
	return (*(original_fn**)base)[index](base);
}