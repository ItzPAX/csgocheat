#pragma once

// globally available shit
class CUserCmd;
class Player;
struct LagRecord;

namespace Game {
	inline CUserCmd* g_pCmd;
	inline Player* g_pLocal;

	inline HINSTANCE g_hInstance;
}

// defines
#define MAX_PLAYERS				65

#define TICK_INTERVAL			(g_Interface.pGlobalVars->flIntervalPerTick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

// typedef
using ulong = unsigned long;
using ushort = unsigned short;
using uchar = unsigned char;

// structs & math
#include "utilities/structures/vec4d/vec4d.h"
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/vec2d/vec2d.h"
#include "utilities/structures/vecaligned/vecaligned.h"
#include "utilities/structures/color/color.h"
#include "utilities/structures/matrix/matrix.h"
#include "utilities/math/math.h"
#include "sdk/classes/bitbuf.h"

// sdk
#include "sdk/structs/shareddefs.h"
#include "sdk/structs/weapondata.h"
#include "sdk/classes/IInterpolatedVar.h"
#include "sdk/manager/interface/classes/ICollideable.h"
#include "sdk/manager/interface/classes/IClientUnknown.h"
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"
#include "sdk/structs/models.h"
#include "sdk/classes/entity.h"
#include "sdk/classes/cusercmd.h"
#include "sdk/structs/surfacedata.h"
#include "sdk/classes/INetChannel.h"
#include "sdk/classes/convar.h"
#include "sdk/classes/anim.h"
#include "sdk/classes/view_shared.h"

// includes
#include "utilities/tools/tools.h"
#include "utilities/tools/xorstr.h"
#include "utilities/render/render.h"
#include "core/init/init.h"
#include "core/hooks/hooks.h"
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"

// features
#include "core/features/header/resolver.h"
#include "core/features/header/prediction.h"
#include "core/features/header/ragebot.h"
#include "core/features/header/penetration.h"
#include "core/features/header/inputmgr.h"
#include "core/features/config/config.h"
#include "core/features/header/backtrack.h"
#include "core/features/header/playerlist.h"
#include "core/menu/menu.h"
#include "core/features/header/visuals.h"
#include "core/features/header/chams.h"
#include "core/features/header/legitbot.h"
#include "core/features/header/misc.h"
#include "core/features/header/antiaim.h"

#include "csgocheat/Syscalls.h"

// externs
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);