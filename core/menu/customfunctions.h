#pragma once
#include "pch.h"
#include "includes.h"

void LegitbotFunction() {
	ImGui::SliderInt(XOR("Randomizatzion"), &g_Config.ints["legitrandomization"].val, 0, 100);
	ImGui::SliderFloat(XOR("Max Speed Factor"), &g_Config.floats["legitgraphmax"].val, 1.f, 10.f, "%.0f%", 1.f);
}