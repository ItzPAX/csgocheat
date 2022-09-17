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

	void MultiSelectCombo(const char* label, std::vector<std::string> items, bool* sel, int size, int iMul = 0) {
		static std::string preview;
		static std::vector<std::string> vec;
		if (ImGui::BeginCombo(label, preview.c_str())) {
			for (int i = 0; i < size; i++) {
				ImGui::Selectable(items[i].c_str(), &sel[(i * 4) + (size * iMul * 4)], ImGuiSelectableFlags_DontClosePopups);
			}
			ImGui::EndCombo();
		}
		vec.clear();
		for (int i = 0; i < size; i++) {
			if (sel[(i * 4) + (size * iMul * 4)])
				vec.push_back(items[i]);
		}

		preview = "";
		if (vec.size() == 0)
			preview = "None";
		for (int i = 0; i < vec.size(); i++) {
			if (vec.size() == 1)
				preview += vec[i];
			else if (!(i == vec.size() - 1))
				preview += vec[i] + ',';
			else
				preview += vec[i];
		}
	}

public:
	using tGraphFunction = void(void);

	inline static bool bInitialized = false;
	inline static bool bToggled = true;

	inline static int iCurrentTab = 3;

	inline static ImGuiStyle* style;

	void ApplyStyle();
	void Render();
	void Draw();
	void Init();
};

extern Menu g_Menu;