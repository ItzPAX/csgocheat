#include "pch.h"
#include "includes.h"


Config g_Config;

void Config::Init() {
	// call setupvalue for every variable
	SetupVal(goofyahhcrosshair, 1, XOR("misc"), XOR("goofyahhcrosshair"));
	SetupVal(bunnyhop, 1, XOR("misc"), XOR("bunnyhop"));
	SetupVal(lagcomp, 1, XOR("misc"), XOR("lagcomp"));
	SetupVal(spectatorlist, 1, XOR("misc"), XOR("spectatorlist"));
	SetupVal(hotkeylist, 0, XOR("misc"), XOR("hotkeylist"));
	SetupVal(paranoia, 1, XOR("misc"), XOR("paranoia"));
	SetupVal(trustfactor, 0, XOR("misc"), XOR("trustfactor"));
	SetupVal(playerfov, 100, XOR("misc"), XOR("playerfov"));
	SetupVal(viewmodelfov, 100, XOR("misc"), XOR("viewmodelfov"));

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

	SetupVal(watermark, 1, XOR("visuals"), XOR("watermark"));
	SetupVal(aspectartio, 1.25, XOR("misc"), XOR("aspectratio"));

	SetupVal(boxesp, 1, XOR("esp"), XOR("boxesp"));
	SetupVal(nameesp, 1, XOR("esp"), XOR("nameesp"));
	SetupVal(healthesp, 1, XOR("esp"), XOR("healthesp"));
	SetupVal(weaponesp, 1, XOR("esp"), XOR("weaponesp"));
	SetupVal(thirdperson, 1, XOR("esp"), XOR("thirdperson"));
	SetupVal(thirdpersonkey, { VK_MBUTTON, ImGuiHotkeyMode_TOGGLE, 1 }, 3, XOR("esp"), XOR("thirdpersonkey"));
	SetupVal(enemyglow, 1, XOR("esp"), XOR("enemyglow"));
	SetupVal(enemyglowcol, { 0.8f, 0.2f, 0.4f, 0.7f }, 4, XOR("esp"), XOR("enemyglowcol"));
	SetupVal(friendlyglow, 1, XOR("esp"), XOR("friendlyglow"));
	SetupVal(friendlyglowcol, { 0.2f, 0.4f, 0.8f, 0.7f }, 4, XOR("esp"), XOR("friendlyglowcol"));
	SetupVal(weaponglow, 1, XOR("esp"), XOR("weaponglow"));
	SetupVal(weaponglowcol, { 0.1f, 0.9f, 0.2f, 0.5f }, 4, XOR("esp"), XOR("weaponglowcol"));
	SetupVal(novisualrecoil, 1, XOR("esp"), XOR("novisualrecoil"));

	SetupVal(antiaim, 0, XOR("antiaim"), XOR("antiaim"));
	SetupVal(attargets, 1, XOR("antiaim"), XOR("attargets"));
	SetupVal(changepitch, { 0,0,0 }, 3, XOR("antiaim"), XOR("changepitch"));
	SetupVal(pitch, { 89,89,89 }, 3, XOR("antiaim"), XOR("pitch"));
	SetupVal(changeyaw, { 0,0,0 }, 3, XOR("antiaim"), XOR("changeyaw"));
	SetupVal(yaw, { 180,180,180 }, 3, XOR("antiaim"), XOR("yaw"));
	SetupVal(desyncdelta, { 30,30,30 }, 3, XOR("antiaim"), XOR("desyncdelta"));
	SetupVal(jitter, { 30,30,30 }, 3, XOR("antiaim"), XOR("jitter"));
	SetupVal(fakelag, 1, XOR("antiaim"), XOR("fakelag"));

	SetupVal(ragebot, 0, XOR("ragebot"), XOR("ragebot"));
	SetupVal(ragebotkey, { 0, ImGuiHotkeyMode_KEYDOWN, 0 }, 3, XOR("ragebot"), XOR("ragebotkey"));
	SetupVal(resolver, 0, XOR("ragebot"), XOR("resolver"));
	SetupVal(targetmode, 0, XOR("ragebot"), XOR("targetmode"));
	SetupVal(autowall, 1, XOR("ragebot"), XOR("autowall"));
	SetupVal(ragedmg, { 20, 10, 25 }, 3, XOR("ragebot"), XOR("ragedmg"));
	SetupVal(ragehitchance, { 20, 10, 25 }, 3, XOR("ragebot"), XOR("ragehitchance"));
	SetupVal(ragehitboxes, { 0,0,0,0,0,0,0,0,0,0,0,0 }, 3 * 4, XOR("ragebot"), XOR("ragehitboxes"));
	SetupVal(multipoint, 1, XOR("ragebot"), XOR("multipoint"));
	SetupVal(multipointscale, 70.f, XOR("ragebot"), XOR("multipointscale"));
	SetupVal(autoscope, 1, XOR("ragebot"), XOR("autoscope"));
	SetupVal(teamcheck, 1, XOR("ragebot"), XOR("teamcheck"));
	SetupVal(silent, 1, XOR("ragebot"), XOR("ragesilent"));
	SetupVal(autoshoot, 1, XOR("ragebot"), XOR("autoshoot"));
	SetupVal(compensaterecoil, 1, XOR("ragebot"), XOR("compensaterecoil"));

	SetupVal(legitbot, 1, XOR("legitbot"), XOR("legitbot"));
	SetupVal(legitbotkey, { VK_LBUTTON, ImGuiHotkeyMode_KEYDOWN, 0 }, 3, XOR("legitbot"), XOR("legitbotkey"));
	SetupVal(legitlagcompmode, 0, XOR("legitbot"), XOR("legitlagcompmode"));
	SetupVal(legithitboxes, { 0,0,0,0 }, 4, XOR("legitbot"), XOR("legithitboxes"));
	SetupVal(legitrandomization, 0.f, XOR("legitbot"), XOR("legitrandomization"));
	SetupVal(legitgraphmax, 2.f, XOR("legitbot"), XOR("legitgraphmax"));
	SetupVal(legitgraph, { 0.f, 1.f, 0.25f, 1.f, 0.75f, 1.f, 1.f, 1.f }, XOR("legitbot"), XOR("legitgraph"));
	SetupVal(legitfov, { 8.f, 8.f, 8.f }, 3, XOR("legitbot"), XOR("legitfov"));
	SetupVal(legitsmoothing, { 10.f, 10.f, 10.f }, 3, XOR("legitbot"), XOR("legitsmoothing"));
	SetupVal(legitrcs, { 25.f, 25.f, 25.f }, 3, XOR("legitbot"), XOR("legitrcs"));
	SetupVal(legitrandom, { 5.f,5.f,5.f }, 3, XOR("legitbot"), XOR("legitrandom"));
	SetupVal(triggerbot, 0, XOR("triggerbot"), XOR("triggerbot"));
	SetupVal(triggerbotkey, { VK_XBUTTON1, ImGuiHotkeyMode_KEYDOWN, 1 }, 3, XOR("triggerbot"), XOR("triggerbotkey"));
}

void Config::Save(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (name == "") {
		status = CfgStatus{ true, XOR("Invalid Name: ") };
		return;
	}

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
	
	if (std::filesystem::exists(file)) {
		status = CfgStatus{ false, XOR("Saved Config: ") + name };
	}
	else {
		status = CfgStatus{ true, XOR("Couldn't create Config: ") + name };
	}
}

void Config::Load(int index) {
	if (index >= configs.size()) {
		status = CfgStatus{ true, XOR("Invalid Config selected") };
		return;
	}

	std::string name = configs[index];
	Load(name);
}

void Config::Load(std::string name) {
	static char path[MAX_PATH];
	std::string folder, file;
	activeconfig = '[' + name + "]";

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

void Config::Delete(int index) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (index >= configs.size()) {
		status = CfgStatus{ true, XOR("Invalid Config selected") };
		return;
	}

	std::string name = configs[index];

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + name + XOR(".ini");
	}

	if (!std::filesystem::exists(file)) {
		status = CfgStatus{ true, XOR("File not found: ") + name };
		return;
	}

	int iStatus = remove(file.c_str());
	if (iStatus == 0)
		status = CfgStatus{ false, XOR("Deleted Config: ") + name };
	else
		status = CfgStatus{ true, XOR("Error deleting Config: ") + name };
}

void Config::MakeDefault(int index) {
	static char path[MAX_PATH];
	std::string folder, file;

	if (index >= configs.size()) {
		status = CfgStatus{ true, XOR("Invalid Config selected") };
		return;
	}

	std::string name = configs[index];

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + XOR("default.info");
	}

	std::ofstream outFile(file);
	if (outFile.is_open())
		outFile << configs[index];
}

std::string Config::GetDefault() {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + XOR("default.info");
	}

	std::ifstream f(file);
	std::ostringstream ss;
	ss << f.rdbuf();

	return ss.str();
}

void Config::LoadDefault() {
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
		folder = std::string(path) + XOR("\\raybot\\");
		file = folder + XOR("default.info");
	}

	std::ifstream f(file);
	std::ostringstream ss;
	ss << f.rdbuf();

	Load(ss.str());
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