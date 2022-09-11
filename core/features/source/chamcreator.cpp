#include "../header/chamcreator.h"

ChamCreator g_ChamCreator;

void ChamCreator::GatherFileInformation() {
	std::string ext = XOR(".ini");
	if (!std::filesystem::exists(folder))
		CreateDirectory(folder.c_str(), NULL);

	// get every file
	materialfiles.clear();
	for (auto& p : std::filesystem::directory_iterator(folder)) {
		if (p.path().extension() != ext)
			continue;
		std::string path = p.path().string();

		// Get time of creation
		char fileval[32] = { '\0' };
		GetPrivateProfileString(XOR("Time"), XOR("creationtime"), "", fileval, sizeof(fileval), path.c_str());

		MaterialFile file;
		file.timestamp = (unsigned int)std::stoi(fileval);
		file.filepath = path;
		file.filename = p.path().stem().string();

		materialfiles.push_back(file);
	}

	std::sort(materialfiles.begin(), materialfiles.end(), Sort);

	// get index from menu but dont update if we have add material selected
	if (g_Config.ints[XOR("chamtype")].val < g_ChamCreator.vMaterialList.size() && g_ChamCreator.vMaterialList.at(g_Config.ints[XOR("chamtype")].val) != XOR("[+] Add Material"))
		g_Chams.iRenderedChamType = g_Config.ints[XOR("chamtype")].val;

	// also check if we want to open the preview / creator mode
	if (g_Config.ints[XOR("chamtype")].val == materialfiles.size()) {
		bCreatorOpened = true;
		g_Config.ints[XOR("chamtype")].val = g_Chams.iRenderedChamType;
	}
}

void ChamCreator::GetMaterialsFromFiles() {
	GatherFileInformation();

	// clear mats
	for (int i = 0; i < g_Chams.pMats.size(); i++)
		g_Chams.pMats.at(i)->DecrementReferenceCount();
	g_Chams.pMats.clear();

	for (auto ent : materialfiles) {
		char fileval[32] = { '\0' };
		IMaterial* pMat;

		// load the base material
		GetPrivateProfileString(XOR("Base"), XOR("material"), "", fileval, sizeof(fileval), ent.filepath.c_str());
		pMat = g_Interface.pMaterialSystem->FindMaterial(fileval, TEXTURE_GROUP_OTHER);

		// load extra info

		// load material into cham system and inc reference count
		g_Chams.pMats.push_back(pMat);
		pMat->IncrementReferenceCount();
	}
}

void ChamCreator::GenerateDefaultMaterialFiles() {
	std::string file;

	// create debugambientcube
	file = folder + XOR("debugambientcube.ini");
	WritePrivateProfileString(XOR("Base"), XOR("material"), XOR("debug/debugambientcube"), file.c_str());
	WritePrivateProfileString(XOR("Time"), XOR("creationtime"), std::to_string(std::time(0)).c_str(), file.c_str());

	// create debugdrawflat
	file = folder + XOR("debugdrawflat.ini");
	WritePrivateProfileString(XOR("Base"), XOR("material"), XOR("debug/debugdrawflat"), file.c_str());
	WritePrivateProfileString(XOR("Time"), XOR("creationtime"), std::to_string(std::time(0)).c_str(), file.c_str());
}

void ChamCreator::UpdateMaterialList() {
	GatherFileInformation();
	vMaterialList.clear();
	for (auto ent : materialfiles) {
		vMaterialList.push_back(ent.filename);
	}
	vMaterialList.push_back("[+] Add Material");
}

void ChamCreator::RenderCreator() {
	if (!bCreatorOpened || !g_Menu.bToggled)
		return;

	style = &ImGui::GetStyle();
	if (!style)
		return;

	ImGui::SetNextWindowSize(WindowSize, ImGuiCond_Once);
	ImGui::Begin(XOR("ChamCreator"), &bCreatorOpened, ImGuiWindowFlags_NoResize);
	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPos = ImGui::GetWindowPos();

	ImGui::BeginChild(XOR("ChamOptions"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	ImGui::Text(XOR("Cham options"));
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild(XOR("ChamPreview"), ImVec2(vSize.x / 2 - style->WindowPadding.x - 2, 0.f), true);
	ImGui::Text(XOR("Cham preview"));

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImVec2(vPos.x + style->FramePadding.x + (vSize.x / 2 - style->WindowPadding.x - 2) + 25, vPos.y + 70), 
		ImVec2(vPos.x + style->FramePadding.x + (vSize.x / 2 - style->WindowPadding.x - 2) + g_PrevModel.GetTexture()->GetActualWidth() + 15, vPos.y + g_PrevModel.GetTexture()->GetActualHeight() - 100),
		ImColor(style->Colors[ImGuiCol_WindowBg].x - 0.02f, style->Colors[ImGuiCol_WindowBg].y - 0.02f, style->Colors[ImGuiCol_WindowBg].z - 0.02f));

	if (g_PrevModel.GetTexture())
	{
		ImGui::GetWindowDrawList()->AddImage(
			g_PrevModel.GetTexture()->pTextureHandles[0]->texture_ptr,
			ImVec2(vPos.x + 20 + (vSize.x / 2 - style->WindowPadding.x - 2), vPos.y - 75),
			ImVec2(vPos.x + 20 + (vSize.x / 2 - style->WindowPadding.x - 2) + g_PrevModel.GetTexture()->GetActualWidth(), vPos.y + g_PrevModel.GetTexture()->GetActualHeight() - 75),
			ImVec2(0, 0), ImVec2(1, 1),
			ImColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	ImGui::EndChild();
	ImGui::End();
}