#include "pch.h"
#include "includes.h"

Menu g_Menu;

void Menu::Render() {
	// start frame and render to screen
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// call our menu
	ImGui::Begin("RayBot");
	g_Menu.Draw();
	ImGui::End();

	// Render dear imgui into screen
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw() {
	// genuine menu here
	ImGui::Checkbox("Box ESP", &Variables::bBoxEsp);
	ImGui::Checkbox("Name ESP", &Variables::bNameEsp);
	ImGui::Checkbox("Health ESP", &Variables::bHealthEsp);

	ImGui::Checkbox("Enemy Chams Vis", &Variables::bEnemyChamsVis);
	ImGui::Checkbox("Enemy Chams Invis", &Variables::bEnemyChamsInvis);

	ImGui::Checkbox("Aimbot", &Variables::bAimbot);
	ImGui::Checkbox("Non-Sticky Aimbot", &Variables::bNonSticky);
	ImGui::Checkbox("Distance Based FOV", &Variables::bDistanceBasedFov);
	ImGui::SliderFloat("Aimbot Smoothing", &Variables::flSmoothing, 1.f, 100.f, "%.0f%", 1.f);
	ImGui::SliderFloat("RCS Correction", &Variables::flCorrecting, 0.f, 100.f, "%.0f%", 1.f);
	ImGui::SliderFloat("Aimbot FOV", &Variables::flFov, 0.f, 180.f, "%.0f%", 1.f);
}

void Menu::Init() {
	// init imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true;

	ImGui_ImplWin32_Init(g_DirectX.window);
	ImGui_ImplDX9_Init(g_DirectX.pDevice);

	bInitialized = true;
	std::cout << "[ RAYBOT ] Successfully Initialized ImGui\n";
}