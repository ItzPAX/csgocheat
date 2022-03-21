#include "../header/antiaim.h"

void AntiAim::DoDesync(CUserCmd* cmd, bool bShouldSendPacket) {
	if (!bShouldSendPacket)
		cmd->viewangles.y += 58.f;
	if (bShouldSendPacket)
		cmd->viewangles.y -= 58.f;
}