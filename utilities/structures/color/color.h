#pragma once
#include "includes.h"

class Color {
public:
	uchar r, g, b, a;
	Color() = default;
	Color(uchar cr, uchar cg, uchar cb, uchar ca = 255) {
		r = cr; g = cg; b = cb; a = ca;
	}

	
	// basic predefined colors
	static Color Black(uchar a = 255) { return Color(0, 0, 0, a); }
	static Color White(uchar a = 255) { return Color(255, 255, 255, a); }
	static Color Blue(uchar a = 255) { return Color(0, 0, 255, a); }
	static Color Red(uchar a = 255) { return Color(255, 0, 0, a); }
	static Color Green(uchar a = 255) { return Color(0, 255, 0, a); }
};