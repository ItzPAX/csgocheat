#pragma once
#include "pch.h"
#include "includes.h"

void cHudUpdate() {
	// sort players by distance in a vector
	g_Visuals.SortPlayers();

	// update the player rects
	g_Visuals.UpdatePlayerRects();
}