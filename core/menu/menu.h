#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

class Menu {
private:
	void RenderClickableButtons(std::vector<std::string> labels, int* p, ImVec2 size, int padding = 0) {
		for (int i = 0; i < labels.size(); i++) {
			if (*p == i) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.54f, 0.2f, 0.89f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.27f, 0.99f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.84f, 0.47f, 1.f, 1.00f));
			}
			if (ImGui::Button(labels.at(i).c_str(), ImVec2(size.x / labels.size() - padding, 25.f))) *p = i;
			if (*p == i) ImGui::PopStyleColor(3);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

public:
	inline static bool bInitialized = false;
	inline static bool bToggled = false;

	inline static int iCurrentTab = 0;

	inline static ImGuiStyle* style;

	void Render();
	void Draw();
	void Init();
};

extern Menu g_Menu;