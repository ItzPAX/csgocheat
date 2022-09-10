#pragma once
#include "pch.h"
#include "includes.h"

class ChamCreator {
private:

public:
	std::vector<std::string> vMaterialList;
	bool bCreatorOpened = false;

	void UpdateMaterialList();
};

extern ChamCreator g_ChamCreator;