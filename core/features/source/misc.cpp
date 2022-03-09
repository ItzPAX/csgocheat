#include "../header/misc.h"

Misc g_Misc;

void Misc::BunnyHop(CUserCmd* cmd) {
	if (!Variables::bBunnyHop)
		return;

	if (!(Game::g_pLocal->iFlags() & FL_ONGROUND))
		cmd->buttons &= ~InJump;
}