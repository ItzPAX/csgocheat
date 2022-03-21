#pragma once
#include "includes.h"

bool InitConvars() {
	ConVar* rPortalTestEnts = g_Interface.pICVar->FindVar(XOR("r_PortalTestEnts"));
	rPortalTestEnts->SetValue(false);

	ConVar* rPortalsOpenAll = g_Interface.pICVar->FindVar(XOR("r_portalsopenall"));
	rPortalTestEnts->SetValue(true);

	return true;
}