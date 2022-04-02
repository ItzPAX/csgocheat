#pragma once
#include "includes.h"
#include "pch.h"

bool IntegrityCheck() {
    BYTE integritybytes[] = {0x46, 0x55, 0x43, 0x4B, 0x4F, 0x46, 0x46, 0x59, 0x4F, 0x55, 0x49, 0x44, 0x49, 0x4F, 0x54};
    uintptr_t integrity_addr = (uintptr_t)GetModuleHandle(XOR("csgo.exe"));
    integrity_addr += 0x19;
    if (memcmp((void*)integrity_addr, integritybytes, sizeof(integritybytes)) == NULL)
    {
        return true;
    }
    return false;
}

void PerformSecurity() {
    if (!IntegrityCheck())
        goto exit;

    return;
exit:
    memcpy(&g_Interface, nullptr, sizeof(g_Interface));
    memcpy(&g_HookManager, nullptr, sizeof(g_Interface));
    memcpy(&g_DirectX, nullptr, sizeof(g_Interface));
}