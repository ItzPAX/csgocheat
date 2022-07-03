#include "pch.h"
#include "includes.h"

Config g_Config;

void Config::Init() {
	// call setupvalue for every variable
	SetupVal(bunnyhop, 1, XOR("misc"), XOR("bunnyhop"));
	SetupVal(lagcomp, 1, XOR("misc"), XOR("lagcomp"));

	SetupVal(chamtype, 0, XOR("chams"), XOR("chamtype"));

	SetupVal(enemychamsvis, 1, XOR("chams"), XOR("enemychamsvis"));
	SetupVal(enemyviscol, { 1.f,0.3f,0.6f,1.f }, 4, XOR("chams"), XOR("enemyviscol"));
	SetupVal(enemychamsinvis, 1, XOR("chams"), XOR("enemychamsinvis"));
	SetupVal(enemyinviscol, { 0.6f,0.9f,0.3f,1.f }, 4, XOR("chams"), XOR("enemyinviscol"));
	SetupVal(lagcompchamstype, 1, XOR("chams"), XOR("lagcompchamstype"));
	SetupVal(lagcompchams, 1, XOR("chams"), XOR("lagcompchams"));

	SetupVal(localchams, 1, XOR("chams"), XOR("localchams"));
	SetupVal(localcol, { 0.3f, 0.8f, 0.5f, 1.f }, 4, XOR("chams"), XOR("localcol"));

	SetupVal(friendlychamsvis, 1, XOR("chams"), XOR("friendlychamsvis"));
	SetupVal(friendlyviscol, { 0.3f, 0.8f, 0.9f, 1.f }, 4, XOR("chams"), XOR("friendlyviscol"));
	SetupVal(friendlychamsinvis, 0, XOR("chams"), XOR("friendlychamsinvis"));
	SetupVal(friendlyinviscol, { 0.3f, 0.5f, 0.9f, 1.f }, 4, XOR("chams"), XOR("friendlyinviscol"));

	SetupVal(boxesp, 1, XOR("esp"), XOR("boxesp"));
	SetupVal(nameesp, 1, XOR("esp"), XOR("nameesp"));
	SetupVal(healthesp, 1, XOR("esp"), XOR("healthesp"));

	SetupVal(legitbot, 1, XOR("legitbot"), XOR("legitbot"));
	SetupVal(legitlagcompmode, 0, XOR("legitbot"), XOR("legitlagcompmode"));
	SetupVal(legithitboxes, { 0,0,0,0 }, 4, XOR("legitbot"), XOR("legithitboxes"));
	SetupVal(legitgraph, { 0.f, 1.f, 0.25f, 1.f, 0.75f, 1.f, 1.f, 1.f }, XOR("legitbot"), XOR("legitgraph"));
	SetupVal(legitfov, { 8.f, 8.f, 8.f }, 3, XOR("legitbot"), XOR("legitfov"));
	SetupVal(legitsmoothing, { 10.f, 10.f, 10.f }, 3, XOR("legitbot"), XOR("legitsmoothing"));
	SetupVal(legitrcs, { 25.f, 25.f, 25.f }, 3, XOR("legitbot"), XOR("legitrcs"));
}

void Config::Save(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + name + XOR(".ini");
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
	status = CfgStatus{ false, XOR("Saved Config: ") + name };
}

void Config::Load(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + name + XOR(".ini");
	}

	if (!std::filesystem::exists(file)) {
		status = CfgStatus{ true, XOR("File not found: ") + name };
		return;
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
	status = CfgStatus{ false, XOR("Loaded Config: ") + name };
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