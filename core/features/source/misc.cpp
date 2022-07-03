#include "../header/misc.h"

Misc g_Misc;

void Misc::BunnyHop(CUserCmd* cmd) {
	if (!g_Config.ints[XOR("bunnyhop")].val)
		return;

	if (!(Game::g_pLocal->iFlags() & Entity::FL_ONGROUND))
		cmd->buttons &= ~CUserCmd::IN_JUMP;
}