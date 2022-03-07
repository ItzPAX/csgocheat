#pragma once
#include "includes.h"

class Color {
public:
	// 0-r | 1-g | 2-b | 3-a
	float rgba[4];
	
	Color() = default;
	Color(float cr, float cg, float cb, float ca = 255) {
		rgba[0] = cr; rgba[1] = cg; rgba[2] = cb; rgba[3] = ca;
	}

	Color ToPercent() {
		return Color(rgba[0] / 255.f, rgba[1] / 255.f, rgba[2] / 255.f, rgba[3] / 255.f);
	}

	void SetFromPercent(float* flVals) {
		this->rgba[0] = flVals[0] * 255.f; this->rgba[1] = flVals[1] * 255.f; this->rgba[2] = flVals[2] * 255.f; this->rgba[3] = flVals[3] * 255.f;
	}

	// imgui translation stuff
	ImVec4 ToVec4D() {
		return ImVec4(this->rgba[0], this->rgba[1], this->rgba[2], this->rgba[3]);
	}
	Color FromVec4D(ImVec4 vec) {
		return Color(vec.x, vec.y, vec.z, vec.w);
	}

	// basic predefined colors
	static Color Black(float a = 255) { return Color(0, 0, 0, a); }
	static Color White(float a = 255) { return Color(255, 255, 255, a); }
	static Color Blue(float a = 255) { return Color(0, 0, 255, a); }
	static Color Red(float a = 255) { return Color(255, 0, 0, a); }
	static Color Green(float a = 255) { return Color(0, 255, 0, a); }
	static Color None() { return Color(0, 0, 0, 0); }
};