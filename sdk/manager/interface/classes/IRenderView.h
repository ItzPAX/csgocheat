#pragma once
#include "utilities/structures/color/color.h"

class IRenderView {
private:
	virtual void __pad0();
	virtual void __pad1();
	virtual void __pad2();
	virtual void __pad3();

public:
	virtual void SetBlend(float blend) = 0;
	virtual float GetBlend(void) = 0;

	virtual void ModulateColor(float const* blend) = 0;
	virtual void GetColorModulation(float* blend) = 0;
};