#pragma once
#include "includes.h"

class Color {
public:
	float r, g, b, a;
	Color() = default;
	Color(float cr, float cg, float cb, float ca = 255) {
		r = cr; g = cg; b = cb; a = ca;
	}

	Color ToPercent() {
		return Color(this->r / 255.f, this->g / 255.f, this->b / 255.f, this->a / 255.f);
	}

	// imgui translation stuff
	ImVec4 ToVec4D() {
		return ImVec4(this->r, this->g, this->b, this->a);
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