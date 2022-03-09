#pragma once
#include "IHandleEntity.h"
#include "IClientEntity.h"
#include "ICollideable.h"
#include "IClientNetworkable.h"
#include "IClientThinkable.h"

// This is the client's version of IUnknown. We may want to use a QueryInterface-like
// mechanism if this gets big.
class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable * GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual Entity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};