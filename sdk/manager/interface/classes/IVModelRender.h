#pragma once
#include "IStudioRender.h"

class IVModelRender {
public:
	void OverrideMaterial(IMaterial* pMat) {
		using fn = void(__thiscall*)(IVModelRender*, IMaterial*, int, int);
		return (*(fn**)this)[1](this, pMat, 0, 0);
	}
};