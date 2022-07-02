#pragma once
#include "pch.h"
#include "includes.h"

void cFrameStageNotify(IBaseClientDLL::ClientFrameStage_t curStage) {
	// run thirdperson always
	g_Visuals.ThirdPerson();

	switch (curStage) {
	case IBaseClientDLL::FRAME_NET_UPDATE_START:
		g_Backtrack.RecordData();
		break;
	}
}