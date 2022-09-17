#pragma once
#include "pch.h"
#include "includes.h"

struct MaterialFile {
	std::string filepath;
	std::string filename;
	unsigned int timestamp;
};

struct CcStatus {
	bool error;
	std::string msg;
};

class ChamCreator {
private:
	std::string folder;
	ImVec2 WindowSize = { 750,500 };
	ImGuiStyle* style;

	std::string basematerial;
	std::string texturegroup = TEXTURE_GROUP_OTHER;
	std::vector<std::string> addonmaterials;

	bool autotexturegroup = true;

	std::string materialname;
	CcStatus status;

	static int Sort(MaterialFile materialfile1, MaterialFile materialfile2) {
		return materialfile1.timestamp < materialfile2.timestamp;
	}
	void GatherFileInformation();
	void UpdatePreviewMaterial();
	void SavePreviewMaterial();

public:
	std::vector<MaterialFile> materialfiles;
	std::vector<std::string> vMaterialList;
	bool bCreatorOpened = false;
	float flPrevCol[4] = { 1.f, 1.f, 1.f, 1.f };

	IMaterial* pEmptyMat;
	IMaterial* pPreviewMaterial;

	CcStatus menustatus;

	ChamCreator() {
		static char path[MAX_PATH];
		std::string ext = XOR(".ini");

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
			folder = std::string(path) + XOR("\\raybot\\materials\\");
	}

	void ApplySettingsFromFile(std::string name);

	void GetMaterialsFromFiles();

	void GenerateDefaultMaterialFiles();
	void UpdateMaterialList();
	void RenderCreator();
};

extern ChamCreator g_ChamCreator;