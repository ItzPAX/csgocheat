#pragma once
#include "includes.h"

template <typename T>
struct ConfigVar {
	ConfigVar() = default;
	ConfigVar(std::string _category, std::string _name, T _val, int _size = 0) {
		category = _category;
		name = _name;
		val = _val;
		size = _size;
	}

	std::string category, name;
	T val;
	int size;
};

struct CfgStatus {
	bool error;
	std::string msg;
};

class Config {
private:
	CfgStatus status;

	// CHEAT VARS GO HERE
private:
	int goofyahhcrosshair;
	int bunnyhop;
	int lagcomp;
	int spectatorlist;
	int hotkeylist;
	int paranoia;
	int trustfactor;

	int chamtype;
	int enemychamsvis;
	float enemyviscol[4];
	int enemychamsinvis;
	float enemyinviscol[4];
	int lagcompchamstype;
	int lagcompchams;

	int localchams;
	float localcol[4];

	int friendlychamsvis;
	float friendlyviscol[4];
	int friendlychamsinvis;
	float friendlyinviscol[4];

	int watermark;

	int nameesp;
	int boxesp;
	int healthesp;
	int thirdperson;
	int thirdpersonkey[3];
	int enemyglow;
	float enemyglowcol[4];
	int friendlyglow;
	float friendlyglowcol[4];
	int weaponglow;
	float weaponglowcol[4];

	int ragebot;
	int ragebotkey[3];
	int targetmode;
	int autowall;
	int ragedmg[3];
	int ragehitchance[3];
	// SIZE = WEAPONTYPES[3] * HITBOXES[4]
	int ragehitboxes[3 * 4];
	int teamcheck;
	int silent;
	int autoshoot;
	int compensaterecoil;

	int legitbot;
	int legitbotkey[3];
	int legithitboxes[4];
	int legitlagcompmode;
	float legitsmoothing[3];
	float legitfov[3];
	float legitrcs[3];
	float legitrandom[3];
	int triggerbot;
	int triggerbotkey[3];

	// GRAPHS
	int legitrandomization;
	float legitgraphmax;
	double legitgraph[8];

	// access variables using these maps
public:
	std::unordered_map<std::string, ConfigVar<int>> ints;
	std::unordered_map<std::string, ConfigVar<float>> floats;

	std::unordered_map<std::string, ConfigVar<int*>> arrints;
	std::unordered_map<std::string, ConfigVar<float*>> arrfloats;

	std::unordered_map<std::string, ConfigVar<double*>> graphs;

public:
	std::vector<std::string> configs;
	std::string activeconfig;

	int iSelConfig = 0;
	void Save(std::string name);
	void Save(int index) {
		if (index >= configs.size()) {
			status = CfgStatus{ true, XOR("Invalid Config selected") };
			return;
		}

		std::string name = configs[index];
		Save(name);
	}
	void Load(int index);
	void Delete(int index);
	Config() { Init(); activeconfig = "[Default]"; }

	CfgStatus Status() { return status; }

private:
	void Init();

	// normal vars
	void SetupVal(int& i, int def, std::string category, std::string name);
	void SetupVal(float& f, float def, std::string category, std::string name);

	// arrays
	void SetupVal(int* i, std::vector<int> defs, int size, std::string category, std::string name);
	void SetupVal(float* f, std::vector<float> defs, int size, std::string category, std::string name);

	// graphs
	void SetupVal(double* d, std::vector<double> defs, std::string category, std::string name);
};

extern Config g_Config;