#pragma once
#include "pch.h"
#include "includes.h"

void cFrameStageNotify(ClientFrameStage_t curStage) {
	switch (curStage) {
	case FRAME_NET_UPDATE_START:
		g_Backtrack.RecordData();

	case FRAME_RENDER_START:
		// thirdperson
		g_Visuals.ThirdPerson();
		break;
	}
}