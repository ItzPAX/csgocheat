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

	if (g_Menu.iCurrentTab == 0) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.54f, 0.2f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.27f, 0.99f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.84f, 0.47f, 1.f, 1.00f));
	}
	if (ImGui::Button(XOR("Aimbot"), ImVec2((vSize.x / 3) - style->WindowPadding.x, 25))) g_Menu.iCurrentTab = 0;
	if (g_Menu.iCurrentTab == 0) ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (g_Menu.iCurrentTab == 1) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.54f, 0.2f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.27f, 0.99f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.84f, 0.47f, 1.f, 1.00f));
	}
	if (ImGui::Button(XOR("ESP"), ImVec2((vSize.x / 3) - style->WindowPadding.x, 25))) g_Menu.iCurrentTab = 1;
	if (g_Menu.iCurrentTab == 1) ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (g_Menu.iCurrentTab == 2) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.54f, 0.2f, 0.89f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.64f, 0.27f, 0.99f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.84f, 0.47f, 1.f, 1.00f));
	}
	if (ImGui::Button(XOR("Misc"), ImVec2((vSize.x / 3) - style->WindowPadding.x, 25))) g_Menu.iCurrentTab = 2;
	if (g_Menu.iCurrentTab == 2) ImGui::PopStyleColor(3);

	ImGui::NewLine();

	switch (g_Menu.iCurrentTab) {
		//AIMBOT
	case 0: {
		ImGui::Text(XOR("Main Aimbot"));
		ImGui::BeginChild(XOR("Main-Aimbot"), ImVec2(0.f, 0.f), true);
		ImGui::Checkbox(XOR("Aimbot"), &Variables::bAimbot);
		ImGui::Checkbox(XOR("Non-Sticky Aimbot"), &Variables::bNonSticky);
		ImGui::Checkbox(XOR("Wait after retargetting"), &Variables::bWaitAfterRetargetting);
		ImGui::SliderFloat(XOR("Aimbot ReactionTime"), &Variables::flReactionTime, 0.f, 4.f, "%.2fs", 0.25f);
		ImGui::SliderFloat(XOR("Aimbot Smoothing"), &Variables::flSmoothing, 1.f, 100.f, "%.0f%", 1.f);
		ImGui::SliderFloat(XOR("RCS Correction"), &Variables::flCorrecting, 0.f, 100.f, "%.0f%", 1.f);
		ImGui::Checkbox(XOR("Standalone RCS"), &Variables::bStandaloneRCS);
		ImGui::SliderFloat(XOR("Aimbot FOV"), &Variables::flFov, 0.f, 180.f, "%.0f%", 1.f);
		DrawExtendableGraph(XOR("[Extended] AimbotCurve"), XOR("Distance"), XOR("Speed"), XOR("AimbotCurve"), Variables::bAimbotCurveExtended, Variables::vAimbotCurve);
		ImGui::EndChild();
	}
		  break;
		  //ESP
	case 1: {
		ImGui::Text(XOR("Main ESP"));
		ImGui::BeginChild(XOR("Main-ESP"), ImVec2(0.f, 0.f), true);
		ImGui::Checkbox(XOR("Box ESP"), &Variables::bBoxEsp);
		ImGui::Checkbox(XOR("Name ESP"), &Variables::bNameEsp);
		ImGui::Checkbox(XOR("Health ESP"), &Variables::bHealthEsp);

		static const char* pChamTypes[] = { "Material", "Flat" };
		ImGui::Combo(XOR("Chams Type"), &Variables::iChamType, pChamTypes, IM_ARRAYSIZE(pChamTypes));

		ImGui::Checkbox(XOR("Enemy Chams Vis		  "), &Variables::bEnemyChamsVis);
		ImGui::SameLine();
		ImGui::ColorEdit4(XOR("Vis Col"), Variables::flVisCol, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		Variables::cVisColor.SetFromPercent(Variables::flVisCol);

		ImGui::Checkbox(XOR("Enemy Chams Invis		"), &Variables::bEnemyChamsInvis);
		ImGui::SameLine();
		ImGui::ColorEdit4(XOR("Invis Col"), Variables::flInvisCol, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		Variables::cInvisColor.SetFromPercent(Variables::flInvisCol);

		ImGui::Checkbox(XOR("Lagcomp Chams"), &Variables::bLagcompChams);

		ImGui::EndChild();
	}
		  break;
	case 2: {
		ImGui::Text(XOR("Main Misc"));
		ImGui::BeginChild(XOR("Main-Misc"), ImVec2(0.f, 0.f), true);
		ImGui::Checkbox(XOR("Bunnyhop"), &Variables::bBunnyHop);
		ImGui::Checkbox(XOR("Lagcompensation"), &Variables::bLagcomp);
		ImGui::Checkbox(XOR("Accurate Lagcomp"), &Variables::bAccurateLagcomp);
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