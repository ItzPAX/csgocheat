#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include "xorstr.h"

struct APIHook {
	std::string _module;
	std::string _api;
	uintptr_t apiAddress;
	char originalBytes[12];
	char hookedBytes[12];
};

std::vector<APIHook> apiHooks = {};

HANDLE hCsgo;

//thanks copilot
DWORD GetProcessIDByName(std::wstring name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (std::wstring(entry.szExeFile) == name)
			{
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
		}
	}
	return 0;
}

int inject(std::string dllPath)
{
	LPVOID loadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA(XOR("kernel32.dll")), XOR("LoadLibraryA"));
	if (!LoadLibraryA)
		return -2;
	
	printf(XOR("[*] loadlibrary: 0x%p\n"), loadLibraryA);
	
	LPVOID dllPathAddress = VirtualAllocEx(hCsgo, NULL, dllPath.length(), MEM_COMMIT, PAGE_READWRITE);
	if (!dllPathAddress)
		return -3;
	
	WriteProcessMemory(hCsgo, dllPathAddress, dllPath.c_str(), dllPath.length(), NULL);
	printf(XOR("[*] written dllpath at: 0x%p\n"), dllPathAddress);
	
	HANDLE threadHandle = CreateRemoteThread(hCsgo, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryA, dllPathAddress, NULL, NULL);
	if (!threadHandle)
		return -4;
	
	printf(XOR("[*] created remote thread: 0x%p\n"), threadHandle);
	
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);
	VirtualFreeEx(hCsgo, dllPathAddress, NULL, MEM_RELEASE);
	
	return 0;
}

void init_vector()
{
	//for struct init
	char empty[12];
	memset(empty, 0x00, 12);

	//place all hooks to be handled in here
	apiHooks.push_back({ XOR("kernel32.dll"), XOR("LoadLibraryExW") });
	apiHooks.push_back({ XOR("kernelbase.dll"), XOR("LoadLibraryExW") });
	apiHooks.push_back({ XOR("ntdll.dll"), XOR("LdrLoadDll") });
	apiHooks.push_back({ XOR("ntdll.dll"), XOR(	"NtOpenFile") });

	for (int i = 0; i <= apiHooks.size() - 1; i++)
	{
		APIHook hook = apiHooks[i];
		FARPROC apiAddress = GetProcAddress(GetModuleHandleA(hook._module.c_str()), hook._api.c_str());

		hook.apiAddress = (uintptr_t)apiAddress;
		memcpy(hook.originalBytes, apiAddress, 12);

		apiHooks[i] = hook;
	}
}


void patch_hooks()
{
	for (int i = 0; i <= apiHooks.size() - 1; i++)
	{
		APIHook hook = apiHooks[i];
		
		DWORD oldProt;
		SIZE_T trash;
		VirtualProtectEx(hCsgo, (LPVOID)hook.apiAddress, 12, PAGE_EXECUTE_READWRITE, &oldProt);
		ReadProcessMemory(hCsgo, (LPVOID)hook.apiAddress, &hook.hookedBytes, sizeof(hook.hookedBytes), &trash);
		WriteProcessMemory(hCsgo, (LPVOID)hook.apiAddress, &hook.originalBytes, sizeof(hook.originalBytes), &trash);
		VirtualProtectEx(hCsgo, (LPVOID)hook.apiAddress, 12, oldProt, &oldProt);
		
		apiHooks[i] = hook;
	}
}

void restore_hooks()
{
	for (int i = 0; i <= apiHooks.size() - 1; i++)
	{
		APIHook hook = apiHooks[i];

		DWORD oldProt;
		SIZE_T trash;
		VirtualProtectEx(hCsgo, (LPVOID)hook.apiAddress, 12, PAGE_EXECUTE_READWRITE, &oldProt);
		WriteProcessMemory(hCsgo, (LPVOID)hook.apiAddress, &hook.hookedBytes, sizeof(hook.hookedBytes), &trash);
		VirtualProtectEx(hCsgo, (LPVOID)hook.apiAddress, 12, oldProt, &oldProt);

		apiHooks[i] = hook;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 1) {
		printf(XOR("[!] no parameters provided (injector.exe *dllpath*)\n"));
		return -1;
	}
	
	hCsgo = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIDByName(L"csgo.exe"));
	if (!hCsgo) {
		printf(XOR("[!] could not open or find csgo.exe! please run injector as admin\n"));
		return -2;
	}
	
	init_vector();
	
	printf(XOR("[*] patching hooked api functions...\n\n"));
	patch_hooks();

	int result = inject(argv[1]);
	printf(XOR("[*] injection result: %i\n"), result);
	
	printf(XOR("\n[*] restoring hooked api functions...\n"));
	restore_hooks();
}
