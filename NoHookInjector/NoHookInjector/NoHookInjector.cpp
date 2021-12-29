#include "syscalls.h"
#include <Windows.h>
#include <winnt.h>
#include <TlHelp32.h>
#include <iostream>
#include <filesystem>
#include "RemoteOps.cpp"

namespace fs = std::filesystem;
HANDLE hProcess = NULL;
int pid = 0;

LPVOID ll_address;
PVOID ll_hooked_bytes;
DWORD ll_offset = 0x204F0;

HANDLE NtOpenProcessByName(LPCTSTR name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (!lstrcmpi(entry.szExeFile, name))
            {
                OBJECT_ATTRIBUTES poa;
                InitializeObjectAttributes(&poa, NULL, NULL, NULL, NULL, NULL);
                CLIENT_ID targetPid = { 0 };
                targetPid.UniqueProcess = (void*)entry.th32ProcessID;
                pid = entry.th32ProcessID;
                NtOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &poa, &targetPid);

                //hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                return hProcess;
            }
        }
    }

    CloseHandle(snapshot);
}

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;
    IsWow64Process(hProcess, &bIsWow64);
    return bIsWow64;
}

void InitializePatch()
{
    ll_address = GetRemoteProcAddress(hProcess, GetRemoteModuleHandle(hProcess, "KERNEL32.DLL"), "LoadLibraryA", 0, FALSE);
    NtReadVirtualMemory(hProcess, ll_address, &ll_hooked_bytes, 12, nullptr);
}


void PatchLL()
{
    if (IsWow64()) {
        BYTE originalBytes[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };
        std::cout << NtWriteVirtualMemory(hProcess, ll_address, &originalBytes, 5, NULL) << std::endl;
    }
    else
    {
        BYTE originalBytes[7];
        NtReadVirtualMemory(hProcess, GetProcAddress(GetModuleHandle("KERNEL32.DLL"), "LoadLibraryA"), &originalBytes, 7, nullptr);
        NtWriteVirtualMemory(hProcess, ll_address, &originalBytes, 7, NULL);
    }
}

void RestoreLL()
{
    NtWriteVirtualMemory(hProcess, ll_address, ll_hooked_bytes, 12, NULL);
}

uintptr_t GetModuleBaseAddress(LPCSTR lpDllName) {

    HANDLE h;
    uintptr_t dll = 0;

    MODULEENTRY32 me32;

    me32.dwSize = sizeof(MODULEENTRY32);

    h = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

    if (h == INVALID_HANDLE_VALUE) {
        ::CloseHandle(h);
        return dll;
    }

    if (!::Module32First(h, &me32)) {
        ::CloseHandle(h);
        return dll;
    }

    do {
        std::cout << me32.szModule << std::endl;
        if (strcmp(me32.szModule, lpDllName) == 0) {
            dll = (uintptr_t)me32.modBaseAddr;
            break;
        }
    } while (::Module32Next(h, &me32));

    ::CloseHandle(h);

    return dll;
}

std::string dllPath = "";
int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "[!] Usage: RayInjector.exe *process.exe* *dllpath*" << std::endl;
        return -1;
    }

    dllPath = argv[2];
    if (argv[2][1] != ':')  //Check if not full path
    {
        dllPath = fs::current_path().string() + "\\" + argv[2];
    }

    hProcess = NtOpenProcessByName(argv[1]);
    std::cout << "[*] NtOpenProcess called!" << std::endl;

    InitializePatch();
    PatchLL();

    HANDLE hThread = NULL;
    LPVOID lpAllocationStart = nullptr;
    SIZE_T szAllocationSize = strlen(dllPath.c_str());
    LPVOID lpStartAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

    NtAllocateVirtualMemory(hProcess, &lpAllocationStart, 0, (PSIZE_T)&szAllocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpAllocationStart != NULL)
    {
        std::cout << "[+] NtAllocateVirtualMemory allocated memory on " << std::hex << lpAllocationStart << std::endl;
    }
    else
    {
        std::cout << "[!] NtAllocateVirtualMemory failed!" << std::endl;
        return -3;
    }

    NTSTATUS wvmStatus = NtWriteVirtualMemory(hProcess, lpAllocationStart, (PVOID)dllPath.c_str(), strlen(dllPath.c_str()), nullptr);
    if (wvmStatus == (NTSTATUS)0x00000000L)
    {
        std::cout << "[+] NtWriteVirtualMemory succeeded! (" << wvmStatus << ")" << std::endl;

    }
    else 
    {
        std::cout << "[!] NtWriteVirtualMemory failed! (" << wvmStatus << ")" << std::endl;
        return -4;
    }

    NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, hProcess, ll_address, lpAllocationStart, FALSE, 0, 0, 0, nullptr);
    if (hThread != NULL)
    {
        std::cout << "[+] NtCreateThreadEx succeeded! (" << hThread << ")" << std::endl;
    }
    else
    {
        std::cout << "[!] NtCreateThreadEx failed!" << std::endl;
        return -5;
    }

    RestoreLL();
    std::cout << "[*] Restored potential LoadLibrary-hook!" << std::endl;

    CloseHandle(hThread);
    CloseHandle(hProcess);
    std::cout << "[*] Closed all handles!" << std::endl;
}
