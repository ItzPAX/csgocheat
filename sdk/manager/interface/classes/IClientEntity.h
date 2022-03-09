#pragma once

#include "IClientRenderable.h"
#include "IClientUnknown.h"

//-----------------------------------------------------------------------------
// Purpose: All client entities must implement this interface.
//-----------------------------------------------------------------------------
class IClientEntity
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;

	// Network origin + angles
	virtual const Vec3D& GetAbsOrigin(void) const = 0;
	virtual const Vec3D& GetAbsAngles(void) const = 0;

	virtual void* GetMouth(void) = 0;
};