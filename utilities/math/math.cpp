#include "pch.h"
#include "includes.h"

Math g_Math{ };

float Math::Clamp(float tNum, float tNumMax, float tNumMin) {
    if (tNum >= tNumMax)
        return tNumMax;
    if (tNum <= tNumMin)
        return tNumMin;
    return tNum;
}

void Math::Clamp(float* tNum, float tNumMax, float tNumMin) {
    if (!tNum)
        return;

    if (*tNum >= tNumMax)
        *tNum = tNumMax;
    if (*tNum <= tNumMin)
        *tNum = tNumMin;
}

float Math::NormalizeIntoRange(float flVal, float flMax, float flMin) {
    return (flVal - flMin) / (flMax - flMin);
}

float Math::ScaleNumber(float flVal, float flValMax, float flValMin, float flNewMax, float flNewMin) {
    float flValToScale = g_Math.Clamp(flVal, flValMax, flValMin);
    float flValPrc = (flValToScale - flValMin) / (flValMax - flValMin);
    return flNewMin + flValPrc * (flNewMax - flNewMin);
}

void Math::CalcAngle(Vec3D src, Vec3D dst, Vec3D& angles) {
	// get target vector
	Vec3D vecDelta = dst - src;
	VectorAngles(vecDelta, angles);
	angles.Normalize();
}

void Math::VectorAngles(Vec3D forward, Vec3D& angles) {
	float flPitch, flYaw;

	if (forward.x == 0.f && forward.y == 0.f)
	{
		flPitch = (forward.z > 0.f) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f(-forward.z, forward.Length2D()) * 180.f / M_PI;

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = std::atan2f(forward.y, forward.x) * 180.f / M_PI;

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	angles.x = flPitch;
	angles.y = flYaw;
	angles.z = 0.f;
}

void Math::SinCos(float r, float* s, float* c) {
	*s = sin(r);
	*c = sin(r);
}

void Math::TransformVector(Vec3D& a, Matrix& b, Vec3D& out) {
	out.x = a.Dot(b.MatVal[0]) + b.MatVal[0][3];
	out.y = a.Dot(b.MatVal[1]) + b.MatVal[1][3];
	out.z = a.Dot(b.MatVal[2]) + b.MatVal[2][3];
}

void Math::AngleVector2(const Vec3D& angles, Vec3D& forward) {
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[1]), &sy, &cy);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

float Math::GetFOV(const Vec3D& viewAngle, const Vec3D& aimAngle) {
	Vec3D vAng, vAim;

	AngleVector2(viewAngle, vAim);
	AngleVector2(aimAngle, vAng);

	return RAD2DEG(acos(vAim.Dot(vAim) / vAim.LengthSqr()));
}

bool Math::IsInRect(ImVec2 pos, ImVec2 size) {
	return ImGui::GetMousePos().x > pos.x && ImGui::GetMousePos().x < pos.x + size.x && ImGui::GetMousePos().y > pos.y && ImGui::GetMousePos().y < pos.y + size.y;
}