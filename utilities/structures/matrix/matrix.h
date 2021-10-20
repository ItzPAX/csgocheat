#pragma once
#pragma warning( disable : 26451 )
#include "includes.h"

struct Matrix {
	Matrix() { }
	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		MatVal[0][0] = m00;	MatVal[0][1] = m01; MatVal[0][2] = m02; MatVal[0][3] = m03;
		MatVal[1][0] = m10;	MatVal[1][1] = m11; MatVal[1][2] = m12; MatVal[1][3] = m13;
		MatVal[2][0] = m20;	MatVal[2][1] = m21; MatVal[2][2] = m22; MatVal[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vec3D& x, const Vec3D& y, const Vec3D& z, const Vec3D& origin) {
		MatVal[0][0] = x.x; MatVal[0][1] = y.x; MatVal[0][2] = z.x; MatVal[0][3] = origin.x;
		MatVal[1][0] = x.y; MatVal[1][1] = y.y; MatVal[1][2] = z.y; MatVal[1][3] = origin.y;
		MatVal[2][0] = x.z; MatVal[2][1] = y.z; MatVal[2][2] = z.z; MatVal[2][3] = origin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	Matrix(const Vec3D& x, const Vec3D& y, const Vec3D& z, const Vec3D& origin) {
		Init(x, y, z, origin);
	}

	inline void SetOrigin(Vec3D const& p) {
		MatVal[0][3] = p.x;
		MatVal[1][3] = p.y;
		MatVal[2][3] = p.z;
	}

	inline void Invalidate(void) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				MatVal[i][j] = NULL;
			}
		}
	}

	float* operator[](int i) { return MatVal[i]; }
	const float* operator[](int i) const { return MatVal[i]; }
	float* Base() { return &MatVal[0][0]; }
	const float* Base() const { return &MatVal[0][0]; }

	float MatVal[3][4];
};