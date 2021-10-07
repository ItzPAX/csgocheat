#pragma once

class vec3 {
public:
	float x, y, z;

public:
	// constructors
	vec3();
	vec3(float vx, float vy, float vz);
	vec3(const vec3& v);

	// operators
	vec3  operator+ (const vec3& vec);
	vec3& operator+= (const vec3& vec);
	vec3  operator- (const vec3& vec);
	vec3& operator-= (const vec3& vec);
	vec3  operator* (float val);
	vec3& operator*= (float val);
	vec3  operator/ (float val);
	vec3& operator/= (float val);
	vec3& operator= (const vec3& vec);

	// functions
};