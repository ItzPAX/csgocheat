#include "pch.h"
#include "includes.h"

Menu g_Menu;

void Menu::Render() {
	// start frame and render to screen
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	g_Menu.Draw();

	// Render dear imgui into screen
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw() {
	// genuine menu here
	ImGui::Begin("RayBot");
	ImGui::Text("NLD_HD");
	ImGui::End();
}

void Menu::Init() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = true;

	ImGui_ImplWin32_Init(g_DirectX.window);
	ImGui_ImplDX9_Init(g_DirectX.pDevice);

	bInitialized = true;
}