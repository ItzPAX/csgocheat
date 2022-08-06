#include "pch.h"
#include "includes.h"

Resolver g_Resolver;

void Resolver::ResolvePlayer(Player* p)
{
	int rside = g_Math.RandomFloat(0, 2);

	switch (rside)
	{
	case 0:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw + 25.351;
		break;
	case 1:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw - 25.351;
		break;
	default:
		p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw;
		break;
	//case 2:
	//	p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw + 50;
	//	break;
	//case 3:
	//	p->GetAnimState()->flGoalFeetYaw = p->GetAnimState()->flLastFeetYaw - 50;
	//	break;
	}
}