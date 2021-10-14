#pragma once

// defines
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]

// typedef
using ulong = unsigned long;
using ushort = unsigned short;
using uchar = unsigned char;

// sdk
#include "sdk/manager/interface/interface.h"
#include "sdk/manager/netvars/netvars.h"
#include "sdk/classes/entity.h"

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

namespace Game {
	inline Player* pLocal;
	inline int iScreenX;
	inline int iScreenY;
}