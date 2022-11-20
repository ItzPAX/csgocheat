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

void ChamCreator::UpdatePreviewMaterial() {
	if (pPreviewMaterial && pEmptyMat) {
		pPreviewMaterial->DecrementReferenceCount();
		pEmptyMat->DecrementReferenceCount();
	}

	// load empty/nonexistant material
	if (basematerial.empty()) {
		if (strstr(texturegroup.c_str(), XOR("auto")))
			pEmptyMat = g_Interface.pMaterialSystem->FindMaterial(basematerial.c_str(), nullptr);
		else
			pEmptyMat = g_Interface.pMaterialSystem->FindMaterial(basematerial.c_str(), texturegroup.c_str());
	}

	// load the base material
	if (strstr(texturegroup.c_str(), XOR("auto")))
		pPreviewMaterial = g_Interface.pMaterialSystem->FindMaterial(basematerial.c_str(), nullptr);
	else
		pPreviewMaterial = g_Interface.pMaterialSystem->FindMaterial(basematerial.c_str(), texturegroup.c_str());

	// load extra info

	// load material into cham system and inc reference count
	if (pPreviewMaterial && pEmptyMat) {
		pPreviewMaterial->IncrementReferenceCount();
		pEmptyMat->IncrementReferenceCount();
	}
}

void ChamCreator::SavePreviewMaterial() {
	std::string file;
	if (materialname.empty()) {
		status = CcStatus{ true, XOR("Couldn't save Material: ") + materialname };
		return;
	}

	// store preview material
	file = folder + materialname.c_str() + XOR(".ini");
	WritePrivateProfileString(XOR("Base"), XOR("material"), basematerial.c_str(), file.c_str());
	WritePrivateProfileString(XOR("Base"), XOR("group"), texturegroup.c_str(), file.c_str());
	WritePrivateProfileString(XOR("Time"), XOR("creationtime"), std::to_string(time(0)).c_str(), file.c_str());

	if (std::filesystem::exists(file))
		status = CcStatus{ false, XOR("Saved Material: ") + materialname };
	else
		status = CcStatus{ true, XOR("Couldn't save Material: ") + materialname };

	// let the chamsystem know we have a new material
	GetMaterialsFromFiles();
}

void ChamCreator::ApplySettingsFromFile(std::string name) {
	std::string file;

	// store preview material
	file = folder + name.c_str() + XOR(".ini");
	char fileval[64] = { '\0' };

	GetPrivateProfileString(XOR("Base"), XOR("material"), "", fileval, sizeof(fileval), file.c_str());
	basematerial = fileval;

	GetPrivateProfileString(XOR("Base"), XOR("group"), "", fileval, sizeof(fileval), file.c_str());
	texturegroup = fileval;

	materialname = name;
}

void ChamCreator::CheckForNewFiles() {
	auto dirIter = std::filesystem::directory_iterator(folder);

	int fileCount = std::count_if(
		begin(dirIter),
		end(dirIter),
		[](auto& entry) { return entry.is_regular_file(); }
	);

	if (fileCount != iOldDirSize)
		GetMaterialsFromFiles();

	iOldDirSize = fileCount;
}

void ChamCreator::GetMaterialsFromFiles() {
	g_Chams.bMatsInitialized = false;

	// clear mats
	for (int i = 0; i < g_Chams.pMats.size(); i++)
		g_Chams.pMats.at(i)->DecrementReferenceCount();
	g_Chams.pMats.clear();

	GatherFileInformation();

	for (auto ent : materialfiles) {
		char material[64] = { '\0' };
		char group[64] = { '\0' };
		char addon[64] = { '\0' };
		IMaterial* pMat;

		// load the base material
		GetPrivateProfileString(XOR("Base"), XOR("material"), "", material, sizeof(material), ent.filepath.c_str());
		GetPrivateProfileString(XOR("Base"), XOR("group"), "", group, sizeof(group), ent.filepath.c_str());
		GetPrivateProfileString(XOR("Info"), XOR("addon"), "", addon, sizeof(addon), ent.filepath.c_str());

		if (strstr(group, XOR("auto")))
			pMat = g_Interface.pMaterialSystem->FindMaterial(material, nullptr);
		else
			pMat = g_Interface.pMaterialSystem->FindMaterial(material, group);

		// load extra info


		// load material into cham system and inc reference count
		g_Chams.pMats.push_back(pMat);
		pMat->IncrementReferenceCount();
	}

	g_Chams.bMatsInitialized = true;
}

void ChamCreator::GenerateDefaultMaterialFiles() {
	std::string file;

	// create debugambientcube
	file = folder + XOR("debugambientcube.ini");
	WritePrivateProfileString(XOR("Base"), XOR("material"), XOR("debug/debugambientcube"), file.c_str());
	WritePrivateProfileString(XOR("Base"), XOR("group"), TEXTURE_GROUP_OTHER, file.c_str());
	WritePrivateProfileString(XOR("Info"), XOR("addon"), std::to_string(0).c_str(), file.c_str());
	WritePrivateProfileString(XOR("Time"), XOR("creationtime"), std::to_string(0).c_str(), file.c_str());

	// create debugdrawflat
	file = folder + XOR("debugdrawflat.ini");
	WritePrivateProfileString(XOR("Base"), XOR("material"), XOR("debug/debugdrawflat"), file.c_str());
	WritePrivateProfileString(XOR("Base"), XOR("group"), TEXTURE_GROUP_OTHER, file.c_str());
	WritePrivateProfileString(XOR("Info"), XOR("addon"), std::to_string(0).c_str(), file.c_str());
	WritePrivateProfileString(XOR("Time"), XOR("creationtime"), std::to_string(0).c_str(), file.c_str());
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

	ImGui::Checkbox(XOR("Auto Texturegroup"), &autotexturegroup);
	if (!autotexturegroup) {
		ImGui::InputText(XOR("Group"), &texturegroup); ImGui::HelpMarker(XOR("if you don't know this, just leave it on auto mode!"));
	}
	else
		texturegroup = XOR("auto");
	ImGui::InputText(XOR("Base-Material"), &basematerial);
	ImGui::Checkbox(XOR("Addon? [WIP]"), &isaddon); ImGui::HelpMarker(XOR("Is this material an addon and should be rendered on top of another material?"));

	ImGui::ColorEdit4(XOR("Preview Col"), flPrevCol, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

	ImGui::NewLine();
	ImGui::InputText(XOR("Name"), &materialname);
	if (ImGui::Button(XOR("Add Material"), ImVec2(-1.f, 0.f)))
		SavePreviewMaterial();

	ImGui::PushStyleColor(ImGuiCol_Text, status.error ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255));
	ImGui::Text(status.msg.c_str());
	ImGui::PopStyleColor();
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


	UpdatePreviewMaterial();
}