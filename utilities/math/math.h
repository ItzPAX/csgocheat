#pragma once

// Math defines
#define M_PI 3.1415926535897932384
#define M_PI2 M_PI * 2
#define DEG2RAD(x) x * M_PI/180.f
#define RAD2DEG(x) x * 180.f/M_PI

class Math {
public:
	float Clamp(float tNum, float tNumMax, float tNumMin);
	void Clamp(float* tNum, float tNumMax, float tNumMin);

	float NormalizeIntoRange(float flVal, float flMax, float flMin);
	float ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin);

	void CorrectMovement(Vec3D vOldAngles, CUserCmd* cmd, float flOldForward, float flOldSide);

	void CalcAngle(Vec3D src, Vec3D dst, Vec3D& angles);
	void VectorAngles(Vec3D forward, Vec3D& angles);

	void SinCos(float r, float* s, float* c);

	void TransformVector(Vec3D& a, Matrix& b, Vec3D& out);

	float RandomFloat(float min, float max);

	void FastRSquare(float a, float* out);
	float FastVecNormalize(Vec3D& vec);

	void AngleVector2(const Vec3D& angles, Vec3D& forward);
	void AngleVector4(Vec3D& angles, Vec3D* forward, Vec3D* right, Vec3D* up);
	float GetFOV(const Vec3D& viewAngle, const Vec3D& aimAngle);

	bool IsInRect(ImVec2 pos, ImVec2 size);
};

extern Math g_Math;