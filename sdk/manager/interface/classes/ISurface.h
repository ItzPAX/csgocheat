#pragma once

class ISurface {
public:
	void UnlockCursor() {
		using original_fn = void(__thiscall*)(ISurface*);
		return (*(original_fn**)this)[66](this);
	}
}; // end namespace