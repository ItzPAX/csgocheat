#include "pch.h"
#include "includes.h"
#include "graph.h"

Menu g_Menu;

void Menu::Render() {
	// apply style
	style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	//style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	//style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(6, 4);
	style->ItemInnerSpacing = ImVec2(4, 3);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	//style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	//style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.0f, 0.69f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.07f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.64f, 0.27f, 0.8f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.34f, 0.0f, 0.69f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.07f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.64f, 0.27f, 0.8f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	// start frame and render to screen
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// call our menu
	ImGui::Begin(XOR("RayBot"));
	g_Menu.Draw();
	ImGui::End();

	// Render dear imgui into screen
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw() {
	ImVec2 vSize = ImGui::GetWindowSize();
	if (!style)
		ImGui::Render();

	RenderClickableButtons({ XOR("Legitbot"), XOR("ESP"), XOR("Misc") }, &g_Menu.iCurrentTab, vSize, style->WindowPadding.x);
	ImGui::NewLine();

	switch (g_Menu.iCurrentTab) {
		//AIMBOT
	case 0: {
		ImGui::BeginChild(XOR("Main-Legitbot"), ImVec2(vSize.x / 2 - style->WindowPadding.x - style->FramePadding.x, 0.f), true);
		ImGui::Text(XOR("Main Legitbot"));
		ImGui::Checkbox(XOR("Legitbot"), (bool*)&g_Config.ints["legitbot"].val);
		DrawExtendableGraph(XOR("[Extended] AimbotCurve"), XOR("Distance"), XOR("Speed"), XOR("AimbotCurve"), g_LegitBot.bGraphExtended, g_LegitBot.vAimbotCurve, g_Config.graphs["legitgraph"].val);
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Weapon-Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - style->FramePadding.x, 0.f), true);
		ImGui::Text(XOR("Weapon-Config"));
		RenderClickableButtons({ XOR("Sniper"), XOR("Rifle"), XOR("Pistol") }, &g_LegitBot.iSelWeapon, ImVec2{vSize.x / 2, vSize.y}, style->WindowPadding.x + style->FramePadding.x);
		ImGui::SliderFloat(XOR("Legitbot Smoothing"), &g_Config.arrfloats["legitsmoothing"].val[g_LegitBot.iSelWeapon], 1.f, 100.f, "%.0f%", 1.f);
		ImGui::SliderFloat(XOR("Legitbot FOV"), &g_Config.arrfloats["legitfov"].val[g_LegitBot.iSelWeapon], 0.f, 180.f, "%.0f%", 1.f);
		if (g_LegitBot.iSelWeapon != 0)
			ImGui::SliderFloat(XOR("Legitbot RCS"), &g_Config.arrfloats["legitrcs"].val[g_LegitBot.iSelWeapon], 0.f, 100.f, "%.0f%", 1.f);
		ImGui::EndChild();
	}
		  break;
		  //ESP
	case 1: {
		ImGui::BeginChild(XOR("Main-ESP"), ImVec2(0.f, 0.f), true);
		ImGui::Text(XOR("Main ESP"));
		ImGui::Checkbox(XOR("Box ESP"), (bool*)&g_Config.ints["boxesp"].val);
		ImGui::Checkbox(XOR("Name ESP"), (bool*)&g_Config.ints["nameesp"].val);
		ImGui::Checkbox(XOR("Health ESP"), (bool*)&g_Config.ints["healthesp"].val);

		static const char* pChamTypes[] = { "debugambientcube", "debugdrawflat" };
		ImGui::Combo(XOR("Chams Type"), &g_Config.ints["chamtype"].val, pChamTypes, IM_ARRAYSIZE(pChamTypes));

		ImGui::Checkbox(XOR("Enemy Chams Vis		  "), (bool*)&g_Config.ints["enemychamsvis"].val);
		ImGui::SameLine();
		ImGui::ColorEdit4(XOR("Vis Col"), g_Config.arrfloats["enemyviscol"].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		g_Chams.cEnemyVisColor.SetFromPercent(g_Config.arrfloats["enemyviscol"].val);

		ImGui::Checkbox(XOR("Enemy Chams Invis		"), (bool*)&g_Config.ints["enemychamsinvis"].val);
		ImGui::SameLine();
		ImGui::ColorEdit4(XOR("Invis Col"), g_Config.arrfloats["enemyinviscol"].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		g_Chams.cEnemyInvisColor.SetFromPercent(g_Config.arrfloats["enemyinviscol"].val);

		ImGui::Checkbox(XOR("Lagcomp Chams"), (bool*)&g_Config.ints["lagcompchams"].val);

		ImGui::EndChild();
	}
		  break;
		  //MISC
	case 2: {
		ImGui::BeginChild(XOR("Main-Misc"), ImVec2(vSize.x / 2 - style->WindowPadding.x - style->FramePadding.x, 0.f), true);
		ImGui::Text(XOR("Main Misc"));
		ImGui::Checkbox(XOR("Bunnyhop"), (bool*)&g_Config.ints["bunnyhop"].val);
		ImGui::Checkbox(XOR("Lagcompensation"), (bool*)&g_Config.ints["lagcomp"].val);
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Weapon-Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - style->FramePadding.x, 0.f), true);
		ImGui::Text(XOR("Config"));
		static const char* pConfigSlot[] = { "Slot1", "Slot2", "Slot3", "Slot4" };
		ImGui::Combo(XOR("Config Slot"), &g_Config.iSelConfig, pConfigSlot, IM_ARRAYSIZE(pConfigSlot));
		if (ImGui::Button("Save", ImVec2(vSize.x / 4 - style->WindowPadding.x - style->FramePadding.x - style->CellPadding.x, 25.f)))
			g_Config.Save(pConfigSlot[g_Config.iSelConfig]);
		ImGui::SameLine();
		if (ImGui::Button("Load", ImVec2(vSize.x / 4 - style->WindowPadding.x - style->FramePadding.x - style->CellPadding.x, 25.f)))
			g_Config.Load(pConfigSlot[g_Config.iSelConfig]);
		ImGui::EndChild();
	}

		  break;
	}
}

void Menu::Init() {
	// init imgui
	ImGui::CreateContext();
	ImPlot::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true;

	ImGui_ImplWin32_Init(g_DirectX.window);
	ImGui_ImplDX9_Init(g_DirectX.pDevice);

	bInitialized = true;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized ImGui/ImPlot\n");
}