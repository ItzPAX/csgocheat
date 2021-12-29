#include "pch.h"
#include "includes.h"

Menu g_Menu;

void Menu::Render() {
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
	// genuine menu here
	ImGui::Checkbox(XOR("Box ESP"), &Variables::bBoxEsp);
	ImGui::Checkbox(XOR("Name ESP"), &Variables::bNameEsp);
	ImGui::Checkbox(XOR("Health ESP"), &Variables::bHealthEsp);

	ImGui::Checkbox(XOR("Enemy Chams Vis"), &Variables::bEnemyChamsVis);
	ImGui::Checkbox(XOR("Enemy Chams Invis"), &Variables::bEnemyChamsInvis);

	ImGui::Checkbox(XOR("Aimbot"), &Variables::bAimbot);
	ImGui::Checkbox(XOR("Non-Sticky Aimbot"), &Variables::bNonSticky);
	ImGui::Checkbox(XOR("Distance Based FOV"), &Variables::bDistanceBasedFov);
	ImGui::Checkbox(XOR("Wait after locked"), &Variables::bWaitAfterTargetting);
	ImGui::SliderFloat(XOR("Aimbot ReactionTime"), &Variables::flReactionTime, 0.f, 4.f, "%.2fs", 0.25f);
	ImGui::SliderFloat(XOR("Aimbot Smoothing"), &Variables::flSmoothing, 1.f, 100.f, "%.0f%", 1.f);
	ImGui::SliderFloat(XOR("RCS Correction"), &Variables::flCorrecting, 0.f, 100.f, "%.0f%", 1.f);
	ImGui::SliderFloat(XOR("Aimbot FOV"), &Variables::flFov, 0.f, 180.f, "%.0f%", 1.f);
}

void Menu::Init() {
	// init imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true;

	ImGui_ImplWin32_Init(g_DirectX.window);
	ImGui_ImplDX9_Init(g_DirectX.pDevice);

	bInitialized = true;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized ImGui\n");
}