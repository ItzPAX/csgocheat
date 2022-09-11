#pragma once
#include "pch.h"
#include "includes.h"

struct MaterialFile {
	std::string filepath;
	std::string filename;
	unsigned int timestamp;
};

class ChamCreator {
private:
	std::string folder;
	ImVec2 WindowSize = { 750,500 };
	ImGuiStyle* style;

	static int Sort(MaterialFile materialfile1, MaterialFile materialfile2) {
		return materialfile1.timestamp < materialfile2.timestamp;
	}
	void GatherFileInformation();


public:
	std::vector<MaterialFile> materialfiles;
	std::vector<std::string> vMaterialList;
	bool bCreatorOpened = false;
	bool bMaterialsUpdated = true;

	ChamCreator() {
		static char path[MAX_PATH];
		std::string ext = XOR(".ini");

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
			folder = std::string(path) + XOR("\\raybot\\materials\\");
	}

	void GetMaterialsFromFiles();

	void GenerateDefaultMaterialFiles();
	void UpdateMaterialList();
	void RenderCreator();
};

extern ChamCreator g_ChamCreator;