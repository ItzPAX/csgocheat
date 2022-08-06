#include "../header/prediction.h"

Prediction g_Prediction;

void Prediction::StartCommand(Player* player, CUserCmd* cmd) {
	*player->GetCurrentCommand() = cmd;
	player->GetLastCommand() = *cmd;

	*iPredictionRandomSeed = cmd->random_seed;
	*pPredictionPlayer = player;
}

int Prediction::GetTickBase(Player* player, CUserCmd* cmd) {
	static int iTick = 0;

	if (cmd != nullptr)
	{
		static CUserCmd* pLastCmd = nullptr;

		// if command was not predicted - increment tickbase
		if (pLastCmd == nullptr || pLastCmd->hasbeenpredicted)
			iTick = player->iTickBase();
		else
			iTick++;

		pLastCmd = cmd;
	}

	return iTick;
}

void Prediction::Start(CUserCmd* ucmd, Player* player) {
	if (!player->bIsAlive() || g_Interface.pMoveHelper == nullptr)
		return;

	bool bOldFirstPredicted = g_Interface.pPrediction->bIsFirstTimePredicted;
	bool bOldInPrediction = g_Interface.pPrediction->bInPrediction;

	flOldCurtime = g_Interface.pGlobalVars->flCurTime;
	flOldFrameTime = g_Interface.pGlobalVars->flFrameTime;
	iOldTickCount = g_Interface.pGlobalVars->iTickCount;

	StartCommand(player, ucmd);

	// Set globals
	g_Interface.pGlobalVars->flCurTime = player->iTickBase() * TICK_INTERVAL;
	g_Interface.pGlobalVars->flFrameTime = g_Interface.pPrediction->bEnginePaused ? 0 : TICK_INTERVAL;
	g_Interface.pGlobalVars->iTickCount = GetTickBase(player, ucmd);

	g_Interface.pPrediction->bIsFirstTimePredicted = false;
	g_Interface.pPrediction->bInPrediction = true;

	ucmd->buttons |= player->GetForcedButtons();
	ucmd->buttons &= ~player->GetButtonDisabled(); // MAY WANT TO DO THIS LATER!!!

	g_Interface.pGameMovement->StartTrackPredictionErrors(player);

	// TODO
	// TODO:  Check for impulse predicted?

	// Get button states
	static auto UpdateButtonState = [](Player* Player, CUserCmd* cmd) {

		static int ButtonsChangedOffset = *(int*)(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x89\x96\x00\x00\x00\x00\x23\xC2"), XOR("xx????xx")) + 0x2);

		int ButtonsChanged = cmd->buttons ^ *(int*)((std::uintptr_t)Player + ButtonsChangedOffset);
		*(int*)((std::uintptr_t)Player + ButtonsChangedOffset - 0xC) = *(int*)((std::uintptr_t)Player + ButtonsChangedOffset);
		*(int*)((std::uintptr_t)Player + ButtonsChangedOffset) = cmd->buttons;
		*(int*)((std::uintptr_t)Player + ButtonsChangedOffset - 0x8) = cmd->buttons & ButtonsChanged;
		*(int*)((std::uintptr_t)Player + ButtonsChangedOffset - 0x4) = ButtonsChanged & ~cmd->buttons;
	};
	UpdateButtonState(player, ucmd);

	// TODO
	g_Interface.pPrediction->CheckMovingGround(player, (double)g_Interface.pGlobalVars->flFrameTime);
	// TODO

	g_Interface.pPrediction->SetLocalViewAngles(ucmd->viewangles);

	player->PreThink();
	player->Think();

	g_Interface.pMoveHelper->SetHost(player);
	g_Interface.pPrediction->SetupMove(player, ucmd, g_Interface.pMoveHelper, &data);
	g_Interface.pGameMovement->ProcessMovement(player, &data);

	g_Interface.pPrediction->FinishMove(player, ucmd, &data);
	g_Interface.pMoveHelper->ProcessImpacts();

	/*FIXMEFIXME*/
	player->PostThink();

	g_Interface.pPrediction->bIsFirstTimePredicted = bOldFirstPredicted;
	g_Interface.pPrediction->bInPrediction = bOldInPrediction;
}

void Prediction::End(CUserCmd* ucmd, Player* player) {
	if (!player->bIsAlive() || g_Interface.pMoveHelper == nullptr)
		return;

	g_Interface.pGlobalVars->flCurTime = flOldCurtime;
	g_Interface.pGlobalVars->flFrameTime = flOldFrameTime;
	g_Interface.pGlobalVars->iTickCount = iOldTickCount;

	g_Interface.pGameMovement->FinishTrackPredictionErrors(player);
	g_Interface.pMoveHelper->SetHost(NULL);

	*player->GetCurrentCommand() = NULL;
	*iPredictionRandomSeed = NULL;
	*pPredictionPlayer = NULL;

	g_Interface.pGameMovement->Reset();
}