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
#include "core/menu/menu.h"
#include "core/features/header/visuals.h"

// externs
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace Game {
	inline CUserCmd* pCmd;
	inline Player* pLocal;
	inline int iScreenX;
	inline int iScreenY;
}