#include "pch.h"
#include "includes.h"

Config g_Config;

void Config::Init() {
	// call setupvalue for every variable
	SetupVal(bunnyhop, 1, "misc", "bunnyhop");
	SetupVal(lagcomp, 1, "misc", "lagcomp");

	SetupVal(chamtype, 0, "chams", "chamtype");
	SetupVal(enemychamsvis, 1, "chams", "enemychamsvis");
	SetupVal(enemyviscol, { 1.f,0.3f,0.6f,1.f }, 4, "chams", "enemyviscol");
	SetupVal(enemychamsinvis, 1, "chams", "enemychamsinvis");
	SetupVal(enemyinviscol, { 0.6f,0.9f,0.3f,1.f }, 4, "chams", "enemyinviscol");
	SetupVal(lagcompchams, 1, "chams", "lagcompchams");

	SetupVal(boxesp, 1, "esp", "boxesp");
	SetupVal(nameesp, 1, "esp", "nameesp");
	SetupVal(healthesp, 1, "esp", "healthesp");

	SetupVal(legitbot, 1, "legitbot", "legitbot");
	SetupVal(legitgraph, { 0.f, 1.f, 0.25f, 0.1f, 0.75f, 0.1f, 1.f, 1.f }, "legitbot", "legitgraph");
	SetupVal(legitfov, { 8.f, 8.f, 8.f }, 3, "legitbot", "legitfov");
	SetupVal(legitsmoothing, { 10.f, 10.f, 10.f }, 3, "legitbot", "legitsmoothing");
	SetupVal(legitrcs, { 25.f, 25.f, 25.f }, 3, "legitbot", "legitrcs");
}

void Config::Save(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + "\\raybot\\";
		file = folder + name + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto& value : ints) {
		WritePrivateProfileString(value.second.category.c_str(), value.second.name.c_str(), std::to_string(value.second.val).c_str(), file.c_str());
	}
	for (auto& value : floats) {
		WritePrivateProfileString(value.second.category.c_str(), value.second.name.c_str(), std::to_string(value.second.val).c_str(), file.c_str());
	}

	for (auto& value : arrints) {
		for (int i = 0; i < value.second.size; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) +"]";
			WritePrivateProfileString(value.second.category.c_str(), basename.c_str(), std::to_string(value.second.val[i]).c_str(), file.c_str());
		}
	}
	for (auto& value : arrfloats) {
		for (int i = 0; i < value.second.size; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) + "]";
			WritePrivateProfileString(value.second.category.c_str(), basename.c_str(), std::to_string(value.second.val[i]).c_str(), file.c_str());
		}
	}

	for (auto& value : graphs) {
		for (int i = 0; i < 8; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) + "]";
			WritePrivateProfileString(value.second.category.c_str(), basename.c_str(), std::to_string(value.second.val[i]).c_str(), file.c_str());
		}
	}
}

void Config::Load(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + "\\raybot\\";
		file = folder + name + ".ini";
	}

	char fileval[32] = { '\0' };

	for (auto& value : ints) {
		GetPrivateProfileString(value.second.category.c_str(), value.second.name.c_str(), "", fileval, sizeof(fileval), file.c_str());
		value.second.val = atoi(fileval);
	}
	for (auto& value : floats) {
		GetPrivateProfileString(value.second.category.c_str(), value.second.name.c_str(), "", fileval, sizeof(fileval), file.c_str());
		value.second.val = atof(fileval);
	}

	for (auto& value : arrints) {
		for (int i = 0; i < value.second.size; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) + "]";
			GetPrivateProfileString(value.second.category.c_str(), basename.c_str(), "", fileval, sizeof(fileval), file.c_str());
			value.second.val[i] = atoi(fileval);
		}
	}
	for (auto& value : arrfloats) {
		for (int i = 0; i < value.second.size; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) + "]";
			GetPrivateProfileString(value.second.category.c_str(), basename.c_str(), "", fileval, sizeof(fileval), file.c_str());
			value.second.val[i] = atof(fileval);
		}
	}

	for (auto& value : graphs) {
		for (int i = 0; i < 8; i++) {
			std::string basename = value.second.name;
			basename += '[' + std::to_string(i) + "]";
			GetPrivateProfileString(value.second.category.c_str(), basename.c_str(), "", fileval, sizeof(fileval), file.c_str());
			value.second.val[i] = atof(fileval);
		}
	}
}

void Config::SetupVal(int& i, int def, std::string category, std::string name) {
	i = def;
	ints[name] = ConfigVar(category, name, i);
}

void Config::SetupVal(float& f, float def, std::string category, std::string name) {
	f = def;
	floats[name] = ConfigVar(category, name, f);
}

void Config::SetupVal(int* i, std::vector<int> defs, int size, std::string category, std::string name) {
	for (int j = 0; j < size; j++)
		i[j] = defs[j];
	arrints[name] = ConfigVar(category, name, i, size);
}

void Config::SetupVal(float* f, std::vector<float> defs, int size, std::string category, std::string name) {
	for (int i = 0; i < size; i++)
		f[i] = defs[i];
	arrfloats[name] = ConfigVar(category, name, f, size);
}

void Config::SetupVal(double* d, std::vector<double> defs, std::string category, std::string name) {
	for (int i = 0; i < 8; i++)
		d[i] = defs[i];
	graphs[name] = ConfigVar(category, name, d);
}