#pragma once
#include "IMaterial.h"

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS		32

// These are given to FindMaterial to reference the texture groups that Show up on the
#define TEXTURE_GROUP_LIGHTMAP				"Lightmaps"
#define TEXTURE_GROUP_WORLD				"World textures"
#define TEXTURE_GROUP_MODEL				"Model textures"
#define TEXTURE_GROUP_VGUI				"VGUI textures"
#define TEXTURE_GROUP_PARTICLE				"Particle textures"
#define TEXTURE_GROUP_DECAL				"Decal textures"
#define TEXTURE_GROUP_SKYBOX				"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS			"ClientEffect textures"
#define TEXTURE_GROUP_OTHER				"Other textures"
#define TEXTURE_GROUP_PRECACHED				"Precached"
#define TEXTURE_GROUP_CUBE_MAP				"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET			"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED			"Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER		"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER		"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER		"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER			"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL			"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS			"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS			"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE		"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS			"Morph Targets"

using MaterialHandle = unsigned short;

class IMaterialSystem {
public:
	IMaterial* FindMaterial(char const* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0) {
		using fn = IMaterial * (__thiscall*)(IMaterialSystem*, char const*, const char*, bool, const char*);
		return (*(fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}
	MaterialHandle FirstMaterial() {
		using fn = MaterialHandle(__thiscall*)(IMaterialSystem*);
		return (*(fn**)this)[86](this);
	}
	MaterialHandle NextMaterial(MaterialHandle handle) {
		using fn = MaterialHandle(__thiscall*)(IMaterialSystem*, MaterialHandle);
		return (*(fn**)this)[87](this, handle);
	}
	MaterialHandle InvalidMaterial() {
		using fn = MaterialHandle(__thiscall*)(IMaterialSystem*);
		return (*(fn**)this)[88](this);
	}
	IMaterial* GetMaterial(MaterialHandle handle) {
		using fn = IMaterial * (__thiscall*)(IMaterialSystem*, MaterialHandle);
		return (*(fn**)this)[89](this, handle);
	}
	int GetMaterialsCount() {
		using fn = int(__thiscall*)(IMaterialSystem*);
		return (*(fn**)this)[90](this);
	}

	bool is_pre_cached() {
		using fn = int(__thiscall*)(IMaterialSystem*);
		return (*(fn**)this)[90](this);
	}
};