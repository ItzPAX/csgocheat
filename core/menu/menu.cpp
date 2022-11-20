#include "pch.h"
#include "includes.h"
#include "customfunctions.h"
#include "graph.h"

Menu g_Menu;

void Menu::ApplyStyle() {
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
}

void Menu::Render() {
	ApplyStyle();
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = false;

	// start frame and render to screen
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bToggled) {
		io.MouseDrawCursor = true;

		// call our menu
		std::string menuname = XOR("RayBot | ") + g_Config.activeconfig + XOR("###MainWindow");

		ImGui::Begin(menuname.c_str(), &g_Menu.bToggled, ImGuiWindowFlags_NoCollapse);
		g_Menu.Draw();
		ImGui::End();
	}

	g_Visuals.DrawSpectatorList();
	g_Visuals.DrawHotkeyList();

	g_ChamCreator.RenderCreator();

	// Render dear imgui into screen
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Draw() {
	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPos = ImGui::GetWindowPos();
	if (!style)
		ImGui::Render();

	RenderClickableButtons({ XOR("Ragebot"), XOR("Legitbot"), XOR("AntiAim"), XOR("Visuals"), XOR("Misc")}, &g_Menu.iCurrentTab, vSize, style->WindowPadding.x - 4);
	ImGui::NewLine();

	// Render the playerlist
	g_PlayerList.DrawPlayerList();

	switch (g_Menu.iCurrentTab) {
		// RAGEBOT
	case 0: {
		ImGui::BeginChild(XOR("Main-Ragebot"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Main Ragebot"));
		if (!g_Config.ints[XOR("trustfactor")].val) {
			ImGui::Checkbox(XOR("Ragebot"), (bool*)&g_Config.ints[XOR("ragebot")].val);
			ImGui::Hotkey(XOR("Ragebot-Key"), g_Config.arrints[XOR("ragebotkey")].val);
			const char* targetmodes[] = { "Damage", "Crosshair", "Distance" };
			ImGui::Combo(XOR("Target selection"), &g_Config.ints[XOR("targetmode")].val, targetmodes, IM_ARRAYSIZE(targetmodes));
			ImGui::Checkbox(XOR("Resolver"), (bool*)&g_Config.ints[XOR("resolver")].val);
			ImGui::Checkbox(XOR("Autowall"), (bool*)&g_Config.ints[XOR("autowall")].val);
			ImGui::Checkbox(XOR("Autoscope"), (bool*)&g_Config.ints[XOR("autoscope")].val);
			ImGui::Checkbox(XOR("Teamcheck"), (bool*)&g_Config.ints[XOR("teamcheck")].val);
			ImGui::Checkbox(XOR("Silent"), (bool*)&g_Config.ints[XOR("ragesilent")].val);
			ImGui::Checkbox(XOR("Autoshoot"), (bool*)&g_Config.ints[XOR("autoshoot")].val);
			ImGui::Checkbox(XOR("Compensate recoil"), (bool*)&g_Config.ints[XOR("compensaterecoil")].val);
			ImGui::Checkbox(XOR("Multipoint"), (bool*)&g_Config.ints[XOR("multipoint")].val);
			ImGui::SliderFloat(XOR("Multipoint Scale"), &g_Config.floats[XOR("multipointscale")].val, 0.f, 100.f, "%.0f%", 1.f);
		}
		else {
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.f), XOR("Disabled due to Preserve TrustFactor"));
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Weapon-Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Weapon-Config"));
		if (!g_Config.ints[XOR("trustfactor")].val) {
			RenderClickableButtons({ XOR("Sniper"), XOR("Rifle"), XOR("Pistol") }, &g_Ragebot.iMenuWeapon, ImVec2{ vSize.x / 2, vSize.y }, style->WindowPadding.x + 5);
			ImGui::SliderInt(XOR("Minimum damage"), &g_Config.arrints[XOR("ragedmg")].val[g_Ragebot.iMenuWeapon], 1.f, 100.f);
			ImGui::SliderInt(XOR("Minimum hitchance"), &g_Config.arrints[XOR("ragehitchance")].val[g_Ragebot.iMenuWeapon], 1.f, 100.f);
			MultiSelectCombo(XOR("Hitboxes"), { XOR("Head"), XOR("Chest"), XOR("Stomach"), XOR("Legs") }, (bool*)g_Config.arrints[XOR("ragehitboxes")].val, 4, g_Ragebot.iMenuWeapon);
		}
		else {
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.f), XOR("Disabled due to Preserve TrustFactor"));
		}
		ImGui::EndChild();
	}
		break;
		//LEGITBOT
	case 1: {
		ImGui::BeginChild(XOR("Main-Legitbot"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Main Legitbot"));
		ImGui::Checkbox(XOR("Legitbot"), (bool*)&g_Config.ints[XOR("legitbot")].val);
		//ImGui::SameLine();
		ImGui::Hotkey(XOR("Legitbot-Key"), g_Config.arrints["legitbotkey"].val);
		DrawExtendableGraph(XOR("[Extended] AimbotCurve"), XOR("Distance"), XOR("Speed"), XOR("AimbotCurve"), g_LegitBot.bGraphExtended, g_LegitBot.vAimbotCurve, g_Config.graphs[XOR("legitgraph")].val, g_Config.floats[XOR("legitgraphmax")].val, LegitbotFunction, XOR("Control how the aimbot behaves, depending on the distance to the target"));
		MultiSelectCombo(XOR("Hitboxes"), { XOR("Head"), XOR("Chest"), XOR("Stomach"), XOR("Legs") }, (bool*)g_Config.arrints[XOR("legithitboxes")].val, 4);

		static const char* pLagRecordSelection[] = { "Closest", "First", "Last" };
		ImGui::Combo(XOR("Lagcomp Mode"), &g_Config.ints[XOR("legitlagcompmode")].val, pLagRecordSelection, IM_ARRAYSIZE(pLagRecordSelection));
		ImGui::Checkbox(XOR("Triggerbot"), (bool*)&g_Config.ints[XOR("triggerbot")].val);
		//ImGui::SameLine();
		ImGui::Hotkey(XOR("Triggerbot-Key"), g_Config.arrints[XOR("triggerbotkey")].val);
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Weapon-Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		
		ImGui::Text(XOR("Weapon-Config"));
		RenderClickableButtons({ XOR("Sniper"), XOR("Rifle"), XOR("Pistol") }, &g_LegitBot.iMenuWeapon, ImVec2{vSize.x / 2, vSize.y}, style->WindowPadding.x + 5);
		ImGui::SliderFloat(XOR("Legitbot Smoothing"), &g_Config.arrfloats[XOR("legitsmoothing")].val[g_LegitBot.iMenuWeapon], 1.f, 100.f, "%.0f%", 1.f);
		if (g_Config.ints[XOR("trustfactor")].val)
			ImGui::HelpMarker(XOR("For Preserve TrustFactor on a smoothing of more than 10 is highly recommended"));
		ImGui::SliderFloat(XOR("Legitbot FOV"), &g_Config.arrfloats[XOR("legitfov")].val[g_LegitBot.iMenuWeapon], 0.f, 180.f, "%.0f%", 1.f);
		if (!g_Config.ints[XOR("trustfactor")].val)
			ImGui::SliderFloat(XOR("Legitbot RCS"), &g_Config.arrfloats[XOR("legitrcs")].val[g_LegitBot.iMenuWeapon], 0.f, 100.f, "%.0f%", 1.f);
		else {
			ImGui::SliderFloat(XOR("Legitbot Randomization"), &g_Config.arrfloats[XOR("legitrandom")].val[g_LegitBot.iMenuWeapon], 2.f, 20.f, "%.0f%%", 0.2f);
			ImGui::HelpMarker(XOR("To not get flagged by VacNET a minimum of 5% randomization is recommended on every weapon"));
		}
		ImGui::EndChild();
	}
		  break;

		  //ANTIAIM
	case 2: {
		ImGui::BeginChild(XOR("Main-Antiaim"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Main AA"));
		if (!g_Config.ints[XOR("trustfactor")].val) {
			ImGui::Checkbox(XOR("Antiaim"), (bool*)&g_Config.ints[XOR("antiaim")].val);
			ImGui::Checkbox(XOR("At targets"), (bool*)&g_Config.ints[XOR("attargets")].val);
			ImGui::SliderInt(XOR("Fakelag"), &g_Config.ints[XOR("fakelag")].val, 1, 14);
		}
		else {
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.f), XOR("Disabled due to Preserve TrustFactor"));
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Mode-Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Mode Configuration"));
		if (!g_Config.ints[XOR("trustfactor")].val) {
			RenderClickableButtons({ XOR("Standing"), XOR("Moving"), XOR("In air") }, &g_AntiAim.iMenuMode, ImVec2{ vSize.x / 2, vSize.y }, style->WindowPadding.x + 5);
			ImGui::Checkbox(XOR("Change Pitch"), (bool*)&g_Config.arrints[XOR("changepitch")].val[g_AntiAim.iMenuMode]);
			if (g_Config.arrints[XOR("changepitch")].val[g_AntiAim.iMenuMode])
				ImGui::SliderInt(XOR("Pitch"), &g_Config.arrints[XOR("pitch")].val[g_AntiAim.iMenuMode], 0.f, 89.f);

			ImGui::Checkbox(XOR("Change Yaw"), (bool*)&g_Config.arrints[XOR("changeyaw")].val[g_AntiAim.iMenuMode]);
			if (g_Config.arrints[XOR("changeyaw")].val[g_AntiAim.iMenuMode])
				ImGui::SliderInt(XOR("Yaw"), &g_Config.arrints[XOR("yaw")].val[g_AntiAim.iMenuMode], 0.f, 180.f);
			ImGui::SliderInt(XOR("Desync"), &g_Config.arrints[XOR("desyncdelta")].val[g_AntiAim.iMenuMode], 0.f, 58.f);
			ImGui::SliderInt(XOR("Jitter"), &g_Config.arrints[XOR("jitter")].val[g_AntiAim.iMenuMode], 0.f, 180.f);
		}
		else {
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.f), XOR("Disabled due to Preserve TrustFactor"));
		}
		ImGui::EndChild();
	}
		  break;

		  //ESP
	case 3: {
		ImGui::BeginChild(XOR("Main-ESP"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Main-ESP"));
		ImGui::Checkbox(XOR("Box ESP"), (bool*)&g_Config.ints[XOR("boxesp")].val);
		ImGui::Checkbox(XOR("Name ESP"), (bool*)&g_Config.ints[XOR("nameesp")].val);
		ImGui::Checkbox(XOR("Health ESP"), (bool*)&g_Config.ints[XOR("healthesp")].val);
		ImGui::Checkbox(XOR("Weapon ESP"), (bool*)&g_Config.ints[XOR("weaponesp")].val);

		ImGui::SliderInt(XOR("FOV"), &g_Config.ints[XOR("playerfov")].val, 50, 200, "%i");
		ImGui::SliderInt(XOR("Viewmodel FOV"), &g_Config.ints[XOR("viewmodelfov")].val, 50, 200, "%i");

		ImGui::Checkbox(XOR("No Visual Recoil"), (bool*)&g_Config.ints[XOR("novisualrecoil")].val);

		ImGui::Checkbox(XOR("Thirdperson"), (bool*)&g_Config.ints[XOR("thirdperson")].val);
		ImGui::Hotkey(XOR("Thirdperson-Key"), g_Config.arrints[XOR("thirdpersonkey")].val);

		if (!g_Config.ints[XOR("trustfactor")].val) {
			ImGui::Checkbox(XOR("Enemy Glow"), (bool*)&g_Config.ints[XOR("enemyglow")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Enemy Col"), g_Config.arrfloats[XOR("enemyglowcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox(XOR("Friendly Glow"), (bool*)&g_Config.ints[XOR("friendlyglow")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Friendly Col"), g_Config.arrfloats[XOR("friendlyglowcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox(XOR("Weapon Glow"), (bool*)&g_Config.ints[XOR("weaponglow")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Weapon Col"), g_Config.arrfloats[XOR("weaponglowcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Chams"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Chams"));

		g_ChamCreator.UpdateMaterialList();
		std::vector<char*> cstringmat;
		for (size_t i = 0; i < g_ChamCreator.vMaterialList.size(); i++)
			cstringmat.push_back(const_cast<char*>(g_ChamCreator.vMaterialList[i].c_str()));

		ImGui::Combo(XOR("Chams Type"), &g_Config.ints[XOR("chamtype")].val, &cstringmat[0], g_ChamCreator.vMaterialList.size());
		if (ImGui::Button(XOR("Edit"), ImVec2(vSize.x / 4 - style->WindowPadding.x + 5, 0.f))) {
			int iToEdit = g_Config.ints[XOR("chamtype")].val;
			// base mats
			if (iToEdit < 2)
				g_ChamCreator.menustatus = CcStatus{ true, XOR("Cannot edit a base material") };
			else {
				g_ChamCreator.ApplySettingsFromFile(g_ChamCreator.materialfiles.at(iToEdit).filename);
				g_ChamCreator.bCreatorOpened = true;
				g_ChamCreator.menustatus = CcStatus{ false, XOR("Edited Material: ") + g_ChamCreator.materialfiles.at(iToEdit).filename };
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(XOR("Remove"), ImVec2(vSize.x / 4 - style->WindowPadding.x + 5, 0.f))) {
			int iToRemove = g_Config.ints[XOR("chamtype")].val;
			// base mats
			if (iToRemove < 2)
				g_ChamCreator.menustatus = CcStatus{ true, XOR("Cannot delete a base material") };
			else {
				g_Config.ints[XOR("chamtype")].val = 0;
				remove(g_ChamCreator.materialfiles.at(iToRemove).filepath.c_str());
				g_ChamCreator.menustatus = CcStatus{ false, XOR("Removed Material: ") + g_ChamCreator.materialfiles.at(iToRemove).filename };
				g_ChamCreator.GetMaterialsFromFiles();
			}
		}

		ImGui::PushStyleColor(ImGuiCol_Text, g_ChamCreator.menustatus.error ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255));
		ImGui::Text(g_ChamCreator.menustatus.msg.c_str());
		ImGui::PopStyleColor();

		RenderClickableButtons({ XOR("Enemy"), XOR("Local"), XOR("Friendly"), XOR("Misc") }, &g_Chams.iChamsMode, ImVec2{ vSize.x / 2, vSize.y }, style->WindowPadding.x - 1);

		switch (g_Chams.iChamsMode) {
		case 0: {
			ImGui::Checkbox(XOR("Enemy Chams Vis"), (bool*)&g_Config.ints[XOR("enemychamsvis")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Vis Col"), g_Config.arrfloats[XOR("enemyviscol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			if (g_Config.ints[XOR("enemychamsvis")].val) {
				ImGui::Checkbox(XOR("Enemy Chams Invis"), (bool*)&g_Config.ints[XOR("enemychamsinvis")].val);
				ImGui::SameLine(vSize.x / 4);
				ImGui::ColorEdit4(XOR("Invis Col"), g_Config.arrfloats[XOR("enemyinviscol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
			}

			ImGui::Checkbox(XOR("Lagcomp Chams"), (bool*)&g_Config.ints[XOR("lagcompchams")].val);
			if (g_Config.ints[XOR("lagcompchams")].val) {
				static const char* pLagcompchamsMode[] = { "Fast", "Fancy" };
				ImGui::Combo(XOR("Lagcompchams Type"), &g_Config.ints[XOR("lagcompchamstype")].val, pLagcompchamsMode, IM_ARRAYSIZE(pLagcompchamsMode));
			}
		}
			  break;
		case 1: {
			ImGui::Checkbox(XOR("Local Chams"), (bool*)&g_Config.ints[XOR("localchams")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Col"), g_Config.arrfloats[XOR("localcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox(XOR("Hand Chams"), (bool*)&g_Config.ints[XOR("handchams")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Hand Col"), g_Config.arrfloats[XOR("handcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
			  break;
		case 2: {
			ImGui::Checkbox(XOR("Friendly Chams"), (bool*)&g_Config.ints[XOR("friendlychamsvis")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Vis Col"), g_Config.arrfloats[XOR("friendlyviscol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			if (g_Config.ints[XOR("friendlychamsvis")].val) {
				ImGui::Checkbox(XOR("Friendly Chams Invis"), (bool*)&g_Config.ints[XOR("friendlychamsinvis")].val);
				ImGui::SameLine(vSize.x / 4);
				ImGui::ColorEdit4(XOR("Invis Col"), g_Config.arrfloats[XOR("friendlyinviscol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
			}
		}
			  break;
		case 3: {
			ImGui::Checkbox(XOR("UIModel Chams"), (bool*)&g_Config.ints[XOR("uimodelchams")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("UIModel Col"), g_Config.arrfloats[XOR("uimodelcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox(XOR("Weapons Chams"), (bool*)&g_Config.ints[XOR("weaponchams")].val);
			ImGui::SameLine(vSize.x / 4);
			ImGui::ColorEdit4(XOR("Weapons Col"), g_Config.arrfloats[XOR("weaponcol")].val, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
			  break;
		}


		ImGui::EndChild();
	}
		break;

		  //MISC
	case 4: {
		ImGui::BeginChild(XOR("Main-Misc"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Main Misc"));
		ImGui::Checkbox(XOR("Preserve TrustFactor"), (bool*)&g_Config.ints[XOR("trustfactor")].val); ImGui::HelpMarker(XOR("Disables / changes features of the cheat to not get flagged by VacNET (SEVERELY limits the cheats capabilities!!)"));
		ImGui::Checkbox(XOR("Paranaoia Mode"), (bool*)&g_Config.ints[XOR("paranoia")].val); ImGui::HelpMarker(XOR("Disables ESP & Aimbot, while you are being spectated"));
		ImGui::Checkbox(XOR("GoofyAhh Crosshair"), (bool*)&g_Config.ints[XOR("goofyahhcrosshair")].val); ImGui::HelpMarker(XOR("Makes the crosshair goofy for everyone spectating you"));
		if (!g_Config.ints[XOR("trustfactor")].val) {
			ImGui::Checkbox(XOR("Bunnyhop"), (bool*)&g_Config.ints[XOR("bunnyhop")].val);
			ImGui::Checkbox(XOR("Lagcompensation"), (bool*)&g_Config.ints[XOR("lagcomp")].val);
		}

		//@itzpax backtrack und bhop sind nicht detected. backtrack wird von overwatch gefickt und bhop wenn man ne minute lang perfekt durchbhop't
		//@emlin doch sind sie dumbass

		ImGui::SliderFloat(XOR("Aspect Ratio"), &g_Config.floats[XOR("aspectratio")].val, 0.5f, 2.f, "%.2f%", 0.05f);
		ImGui::InputText(XOR("Clantag"), &g_Misc.clantag);
		ImGui::Checkbox(XOR("Watermark"), (bool*)&g_Config.ints[XOR("watermark")].val);
		ImGui::Checkbox(XOR("Spectatorlist"), (bool*)&g_Config.ints[XOR("spectatorlist")].val);
		ImGui::Checkbox(XOR("Hotkeylist"), (bool*)&g_Config.ints[XOR("hotkeylist")].val);
		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::BeginChild(XOR("Config"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
		ImGui::Text(XOR("Config"));

		g_Config.configs.clear();
		static char path[MAX_PATH];
		std::string folder;
		std::string ext = XOR(".ini");

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
			folder = std::string(path) + XOR("\\raybot\\");
		for (auto& p : std::filesystem::directory_iterator(folder)) {
			if (p.path().extension() == ext)
				g_Config.configs.push_back(p.path().stem().string());
		}

		std::vector<char*> cstring;
		std::string defaultname = g_Config.GetDefault();
		for (size_t i = 0; i < g_Config.configs.size(); i++) {
			cstring.push_back(const_cast<char*>(g_Config.configs[i].c_str()));
			if (g_Config.configs[i] == defaultname)
				strcat(cstring[i], XOR(" [DEF]"));
		}

		ImGui::PushItemWidth(-1);
		ImGui::ListBox(XOR(""), &g_Config.iSelConfig, &cstring[0], g_Config.configs.size(), 5);
		if (ImGui::Button(XOR("Make default"), ImVec2(vSize.x / 4.f - 27.f ,25.f)))
			g_Config.MakeDefault(g_Config.iSelConfig);
		ImGui::SameLine();
		if (ImGui::Button(XOR("Remove default"), ImVec2(vSize.x / 4.f - 27.f, 25.f)))
			g_Config.RemoveDefault();
		if (ImGui::Button(XOR("Reset config to default"), ImVec2(-1, 25.f)))
			g_Config.ResetToDefault();

		ImGui::NewLine();
		static std::string name;
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(150, 150, 150, 255));
		ImGui::InputTextWithHint(XOR(""), XOR("Config Name"), &name);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor();
		if (ImGui::Button(XOR("Save"), ImVec2(vSize.x / 6 - style->WindowPadding.x - style->FramePadding.x, 25.f))) {
			if (name.empty())
				g_Config.Save(g_Config.iSelConfig);
		
			else
				g_Config.Save(name);
			name.clear();
		}
		
		ImGui::SameLine();
		if (ImGui::Button(XOR("Load"), ImVec2(vSize.x / 6 - style->WindowPadding.x - style->FramePadding.x, 25.f)))
			g_Config.Load(g_Config.iSelConfig);
		
		ImGui::SameLine();
		if (ImGui::Button(XOR("Delete"), ImVec2(vSize.x / 6 - style->WindowPadding.x - style->FramePadding.x, 25.f)))
			g_Config.Delete(g_Config.iSelConfig);

		ImGui::PushStyleColor(ImGuiCol_Text, g_Config.Status().error ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255));
		ImGui::Text(g_Config.Status().msg.c_str());
		ImGui::PopStyleColor();

		if (g_PlayerList.bListOpened ? ImGui::Button(XOR("Close Playerlist"), ImVec2(-1.f, 0.f)) : ImGui::Button(XOR("Open Playerlist"), ImVec2(-1.f, 0.f)))
			g_PlayerList.bListOpened = !g_PlayerList.bListOpened;
		ImGui::Text(XOR("Build date: "));
		ImGui::SameLine();
		ImGui::Text(XOR(__TIMESTAMP__));
		ImGui::EndChild();
	}

		  break;
	}
}

void Menu::Init() {
	// init imgui
	ImGui::CreateContext();
	ImPlot::CreateContext();

	ImGui_ImplWin32_Init(g_DirectX.window);
	ImGui_ImplDX9_Init(g_DirectX.pDevice);

	bInitialized = true;
	std::cout << XOR("[ RAYBOT ] Successfully Initialized ImGui/ImPlot\n");
}