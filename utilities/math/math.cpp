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

//aimtux
void Math::CorrectMovement(Vec3D vOldAngles, CUserCmd* cmd, float flOldForward, float flOldSide) {
	float delta_view;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * flOldForward + cos(DEG2RAD(delta_view + 90.f)) * flOldSide;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * flOldForward + sin(DEG2RAD(delta_view + 90.f)) * flOldSide;
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

float Math::RandomFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

void Math::FastRSquare(float a, float* out) {
	const auto xx = _mm_load_ss(&a);
	auto xr = _mm_rsqrt_ss(xx);
	auto xt = _mm_mul_ss(xr, xr);
	xt = _mm_mul_ss(xt, xx);
	xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
	xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
	xr = _mm_mul_ss(xr, xt);
	_mm_store_ss(out, xr);
}

float Math::FastVecNormalize(Vec3D& vec) {
	const auto sqrlen = vec.LengthSqr() + 1.0e-10f;
	float invlen;
	FastRSquare(sqrlen, &invlen);
	vec.x *= invlen;
	vec.y *= invlen;
	vec.z *= invlen;
	return sqrlen * invlen;
}

void Math::AngleVector2(const Vec3D& angles, Vec3D& forward) {
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[1]), &sy, &cy);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void Math::AngleVector4(Vec3D& angles, Vec3D* forward, Vec3D* right, Vec3D* up) {
	float sp, sy, sr, cp, cy, cr;

	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
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