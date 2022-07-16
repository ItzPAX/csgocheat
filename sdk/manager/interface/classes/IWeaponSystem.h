#pragma once
#include "sdk/structs/weapondata.h"

class IWeaponSystem {
	virtual void pad0() = 0;
	virtual void pad1() = 0;
public:
	virtual CCSWeaponData* GetWeaponData(unsigned int idx) const = 0;
};