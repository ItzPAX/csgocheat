#include "pch.h"
#include "includes.h"

Resolver g_Resolver;

void Resolver::ResolvePlayer(Player* p)
{
	if (!g_Config.ints[XOR("resolver")].val)
		return;

	int rside = g_Math.RandomFloat(0, 2);

	switch (rside)
	{
	case 0:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw + 25;
		break;
	case 1:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw - 25;
		break;
	default:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw;
		break;
	}
}