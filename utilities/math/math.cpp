#include "pch.h"
#include "includes.h"
#include <DirectXMath.h>

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

bool Math::IntersectLineWithBB(Vec3D& vStart, Vec3D& vEndDelta, Vec3D& vMin, Vec3D& vMax) {
	float d1, d2, f;
	bool startsolid = true;
	float t1 = -1.0f, t2 = 1.0f;

	for (int i = 0; i < 6; i++) {
		if (i >= 3) {
			int j = i - 3;

			d1 = vStart[j] - vMax[j];
			d2 = d1 + vEndDelta[j];
		}
		else {
			d1 = -vStart[i] + vMin[i];
			d2 = d1 - vEndDelta[i];
		}

		if (d1 > 0.0f && d2 > 0.0f)
			return false;
		else if (d1 <= 0.0f && d2 <= 0.0f)
			continue;

		if (d1 > 0)
			startsolid = false;

		if (d1 > d2) {
			f = d1;
			if (f < 0.0f)
				f = 0.0f;

			f /= d1 - d2;
			if (f > t1)
				t1 = f;
		}
		else {
			f = d1 / (d1 - d2);
			if (f < t2)
				t2 = f;
		}
	}
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
	// side/forward move correction
	float deltaView;
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
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * flOldForward + cos(DEG2RAD(deltaView + 90.f)) * flOldSide;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * flOldForward + sin(DEG2RAD(deltaView + 90.f)) * flOldSide;
}

void Math::CalcAngle(Vec3D src, Vec3D dst, Vec3D& angles) {
	// get target vector
	Vec3D vecDelta = dst - src;
	VectorAngles(vecDelta, angles);
	//angles.Normalize();
}

void Math::VectorAngles(Vec3D forward, Vec3D& angles) {
	Vec3D view;
	float flTmp;

	if (forward[1] == 0.f && forward[0] == 0.f) {
		view[0] = 0.f;
		view[1] = 0.f;
	}
	else {
		view[1] = RAD2DEG(atan2(forward[1], forward[0]));

		if (view[1] < 0.f)
			view[1] += 360.f;

		flTmp = forward[0] * forward[0] + forward[1] * forward[1];
		view[2] = sqrt(flTmp);

		//view[2] = 1/fastInverseSquare(forward[0] * forward[0] + forward[1] * forward[1]);

		view[0] = RAD2DEG(atan2(forward[2], view[2]));
	}

	angles[0] = -view[0];
	angles[1] = view[1];
	angles[2] = 0.f;
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

void Math::AngleMatrix(const Vec3D& angView, Matrix& matOutput, const Vec3D& vecOrigin) {
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angView.z));

	matOutput.SetForward(Vec3D(cp * cy, cp * sy, -sp));

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	matOutput.SetLeft(Vec3D(sp * srcy - crsy, sp * srsy + crcy, sr * cp));
	matOutput.SetUp(Vec3D(sp * crcy + srsy, sp * crsy - srcy, cr * cp));
	matOutput.SetOrigin(vecOrigin);
}

bool Math::IsInRect(ImVec2 pos, ImVec2 size) {
	return ImGui::GetMousePos().x > pos.x && ImGui::GetMousePos().x < pos.x + size.x && ImGui::GetMousePos().y > pos.y && ImGui::GetMousePos().y < pos.y + size.y;
}