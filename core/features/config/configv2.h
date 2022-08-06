#pragma once
#include "includes.h"

enum ConfigCategory
{
	legitbot = 0,
	triggerbot = 1,

	ragebot = 2,
	antiaim = 3,

	esp = 4,
	chams = 5,

	misc = 6
};

template <typename T>
struct ConfigSetting
{
	ConfigSetting() = default;
	
	ConfigCategory category;
	T name;
	void* value;
};


class config {

public:
	template <typename T>
	T get(ConfigSetting<T> setting);
};