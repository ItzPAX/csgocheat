#pragma once
#include "sdk/structs/models.h"
#include "sdk/manager/interface/classes/IStudioRender.h"

class IVModelInfo {
public:
	Model* GetModel(int index) {
		using original_fn = Model* (__thiscall*)(IVModelInfo*, int);
		return (*(original_fn**)this)[1](this, index);
	}
	int GetModelIndex(const char* filename) {
		using original_fn = int(__thiscall*)(IVModelInfo*, const char*);
		return (*(original_fn**)this)[2](this, filename);
	}
	const char* GetModelName(const Model* model) {
		using original_fn = const char* (__thiscall*)(IVModelInfo*, const Model*);
		return (*(original_fn**)this)[3](this, model);
	}
	StudioHDR* GetStudioModel(const Model* model) {
		using original_fn = StudioHDR* (__thiscall*)(IVModelInfo*, const Model*);
		return (*(original_fn**)this)[32](this, model);
	}
};