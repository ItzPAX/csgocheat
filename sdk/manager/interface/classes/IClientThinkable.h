#pragma once
#include "IClientUnknown.h"

// Entities that implement this interface can be put into the client think list.
class IClientThinkable
{
public:
	// Gets at the containing class...
	virtual IClientUnknown * GetIClientUnknown() = 0;

	virtual void				ClientThink() = 0;

	// Called when you're added to the think list.
	// GetThinkHandle's return value must be initialized to INVALID_THINK_HANDLE.
	virtual void				Temp() = 0;
	virtual void				Temp1() = 0;

	// Called by the client when it deletes the entity.
	virtual void				Release() = 0;
};