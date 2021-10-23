#pragma once

// defines
#define MAX_PLAYERS				65

#define TICK_INTERVAL			(g_Interface.pGlobalVars->flIntervalPerTick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

// typedef
using ulong = unsigned long;
using ushort = unsigned short;
using uchar = unsigned char;

// sdk
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"
#include "sdk/classes/entity.h"
#include "sdk/classes/cusercmd.h"
#include "sdk/classes/entityclass.h"

// includes
#include "utilities/render/render.h"
#include "core/init/init.h"
#include "utilities/structures/vec3d/vec3d.h"
#include "utilities/structures/vec2d/vec2d.h"
#include "utilities/math/math.h"
#include "core/hooks/hooks.h"
#include "utilities/tools/tools.h"
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"
#include "utilities/structures/color/color.h"
#include "utilities/structures/vec4d/vec4d.h"
#include "utilities/structures/matrix/matrix.h"

// features
#include "core/features/variables/variables.h"
#include "core/menu/menu.h"
#include "core/features/header/visuals.h"
#include "core/features/header/chams.h"

// externs
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// globally available shit
namespace Game {
	inline CUserCmd* g_pCmd;
	inline Player* g_pLocal;
	inline int g_iScreenX;
	inline int g_iScreenY;
}