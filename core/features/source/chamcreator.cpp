#include "../header/chamcreator.h"

ChamCreator g_ChamCreator;

void ChamCreator::UpdateMaterialList() {
	vMaterialList.clear();
	static char path[MAX_PATH];
	std::string folder;
	std::string ext = XOR(".ini");

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
		folder = std::string(path) + XOR("\\raybot\\materials\\");
	
	if (!std::filesystem::exists(folder))
		CreateDirectory(folder.c_str(), NULL);

	for (auto& p : std::filesystem::directory_iterator(folder)) {
		if (p.path().extension() == ext)
			vMaterialList.push_back(p.path().stem().string());
	}
	vMaterialList.push_back("[+] Add Material");
}