#pragma once

class vec2 {
public:
	float x, y;

public:
	// constructors
	vec2();
	vec2(float vx, float vy);
	vec2(const vec2& v);

	// operators
	vec2  operator+ (const vec2& vec);
	vec2& operator+= (const vec2& vec);
	vec2  operator- (const vec2& vec);
	vec2& operator-= (const vec2& vec);
	vec2  operator* (float val);
	vec2& operator*= (float val);
	vec2  operator/ (float val);
	vec2& operator/= (float val);
	vec2& operator= (const vec2& vec);

	// functions
};