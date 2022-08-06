#pragma once
#include "pch.h"
#include "includes.h"

class CCSGOPlayerAnimState
{
public:
	void Create(Entity* pEntity)
	{
		using CreateAnimationStateFn = void(__thiscall*)(void*, Entity*);
		static auto oCreateAnimationState = reinterpret_cast<CreateAnimationStateFn>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x56\x8B\xF1\xB9\x00\x00\x00\x00\xC7\x46"), XOR("xxxxxxx????xx"))); // @xref: "ggprogressive_player_levelup"

		if (oCreateAnimationState == nullptr)
			return;

		oCreateAnimationState(this, pEntity);
	}

	void Update(Vec3D angView)
	{
		using UpdateAnimationStateFn = void(__vectorcall*)(void*, void*, float, float, float, void*);
		static auto oUpdateAnimationState = reinterpret_cast<UpdateAnimationStateFn>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x18\x56\x57\x8B\xF9\xF3\x0F\x11\x54\x24"), XOR("xxxxxxxxxxxxxxxxxxx"))); // @xref: "%s_aim"

		if (oUpdateAnimationState == nullptr)
			return;

		oUpdateAnimationState(this, nullptr, 0.0f, angView.y, angView.x, nullptr);
	}

	void Reset()
	{
		using ResetAnimationStateFn = void(__thiscall*)(void*);
		static auto oResetAnimationState = reinterpret_cast<ResetAnimationStateFn>(g_Tools.SignatureScan(XOR("client.dll"), XOR("\x56\x6A\x01\x68\x00\x00\x00\x00\x8B\xF1"), XOR("xxxx????xx"))); // @xref: "player_spawn"

		if (oResetAnimationState == nullptr)
			return;

		oResetAnimationState(this);
	}

	std::byte	pad0[0x60]; // 0x00
	Entity* pEntity; // 0x60
	Entity* pActiveWeapon; // 0x64
	Entity* pLastActiveWeapon; // 0x68
	float		flLastUpdateTime; // 0x6C
	int			iLastUpdateFrame; // 0x70
	float		flLastUpdateIncrement; // 0x74
	float		flEyeYaw; // 0x78
	float		flEyePitch; // 0x7C
	float		flGoalFeetYaw; // 0x80
	float		flLastFeetYaw; // 0x84
	float		flMoveYaw; // 0x88
	float		flLastMoveYaw; // 0x8C // changes when moving/jumping/hitting ground
	float		flLeanAmount; // 0x90
	float		flLowerBodyYawAlignTime; // 0x94
	float		flFeetCycle; // 0x98 0 to 1
	float		flMoveWeight; // 0x9C 0 to 1
	float		flMoveWeightSmoothed; // 0xA0
	float		flDuckAmount; // 0xA4
	float		flHitGroundCycle; // 0xA8
	float		flRecrouchWeight; // 0xAC
	Vec3D		vecOrigin; // 0xB0
	Vec3D		vecLastOrigin;// 0xBC
	Vec3D		vecVelocity; // 0xC8
	Vec3D		vecVelocityNormalized; // 0xD4
	Vec3D		vecVelocityNormalizedNonZero; // 0xE0
	float		flVelocityLenght2D; // 0xEC
	float		flJumpFallVelocity; // 0xF0
	float		flSpeedNormalized; // 0xF4 // clamped velocity from 0 to 1 
	float		flRunningSpeed; // 0xF8
	float		flDuckingSpeed; // 0xFC
	float		flDurationMoving; // 0x100
	float		flDurationStill; // 0x104
	bool		bOnGround; // 0x108
	bool		bHitGroundAnimation; // 0x109
	std::byte	pad2[0x2]; // 0x10A
	float		flNextLowerBodyYawUpdateTime; // 0x10C
	float		flDurationInAir; // 0x110
	float		flLeftGroundHeight; // 0x114
	float		flHitGroundWeight; // 0x118 // from 0 to 1, is 1 when standing
	float		flWalkToRunTransition; // 0x11C // from 0 to 1, doesnt change when walking or crouching, only running
	std::byte	pad3[0x4]; // 0x120
	float		flAffectedFraction; // 0x124 // affected while jumping and running, or when just jumping, 0 to 1
	std::byte	pad4[0x208]; // 0x128
	float		flMinBodyYaw; // 0x330
	float		flMaxBodyYaw; // 0x334
	float		flMinPitch; //0x338
	float		flMaxPitch; // 0x33C
	int			iAnimsetVersion; // 0x340
}; // Size: 0x344