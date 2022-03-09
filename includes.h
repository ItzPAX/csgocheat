#pragma once

// globally available shit
class CUserCmd;
class Player;
namespace Game {
	inline CUserCmd* g_pCmd;
	inline Player* g_pLocal;
	inline int g_iScreenX;
	inline int g_iScreenY;

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
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/vec2d/vec2d.h"
#include "utilities/structures/vecaligned/vecaligned.h"
#include "utilities/structures/color/color.h"
#include "utilities/structures/matrix/matrix.h"
#include "utilities/math/math.h"

// sdk
#include "sdk/manager/interface/classes/ICollideable.h"
#include "sdk/manager/interface/classes/IClientUnknown.h"
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"
#include "sdk/structs/models.h"
#include "sdk/classes/entity.h"
#include "sdk/classes/cusercmd.h"
#include "sdk/classes/entityclass.h"
#include "sdk/classes/INetChannelInfo.h"
#include "sdk/classes/convar.h"

// includes
#include "utilities/tools/tools.h"
#include "utilities/tools/xorstr.h"
#include "utilities/render/render.h"
#include "core/init/init.h"
#include "core/hooks/hooks.h"
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"

// features
#include "core/features/variables/variables.h"
#include "core/menu/menu.h"
#include "core/features/header/visuals.h"
#include "core/features/header/chams.h"
#include "core/features/header/aimbot.h"
#include "core/features/header/backtrack.h"
#include "core/features/header/misc.h"

// externs
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);