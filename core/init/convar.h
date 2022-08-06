#pragma once
#include "includes.h"

bool InitConvars() {
	ConVar* rPortalTestEnts = g_Interface.pICVar->FindVar(XOR("r_PortalTestEnts"));
	rPortalTestEnts->SetValue(false);

	ConVar* rPortalsOpenAll = g_Interface.pICVar->FindVar(XOR("r_portalsopenall"));
	rPortalTestEnts->SetValue(true);

	ConVar* pClWeaponsShowAccuracy = g_Interface.pICVar->FindVar(XOR("cl_weapon_debug_show_accuracy"));
	pClWeaponsShowAccuracy->SetValue(2);

	g_Interface.pICVar->FindVar(XOR("cl_extrapolate"))->SetValue(false);

	return true;
}