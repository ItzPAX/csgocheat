#pragma once
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/color/color.h"

class IVDebugOverlay {
public:
	virtual void PadFn() = 0;
	virtual void AddBoxOverlay(const Vec3D& origin, const Vec3D& mins, const Vec3D& max, Vec3D const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddTriangleOverlay(const Vec3D& p1, const Vec3D& p2, const Vec3D& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const Vec3D& origin, const Vec3D& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void PadFn1() = 0;
	virtual void PadFn2() = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const Vec3D& start, const Vec3D& end, const Vec3D& mins, const Vec3D& max, const Vec3D& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const Vec3D& origin) = 0;
	virtual int ScreenPosition(const Vec3D& point, Vec3D& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, Vec3D& screen) = 0;

	virtual void* GetFirst(void) = 0;
	virtual void* GetNext(void* current) = 0;
	virtual void ClearDeadOverlays(void) = 0;
	virtual void ClearAllOverlays() = 0;

	virtual void PadFn3() = 0;
	virtual void PadFn4() = 0;

	virtual void AddLineOverlayAlpha(const Vec3D& origin, const Vec3D& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const Vec3D& origin, const Vec3D& mins, const Vec3D& max, Vec3D const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;

	virtual void PadFn5() = 0;
};