#pragma once

enum CmdButton {
	InAttack = (1 << 0),
	InJump = (1 << 1),
	InDuck = (1 << 2),
	InForward = (1 << 3),
	InBack = (1 << 4),
	InUse = (1 << 5),
	InCancel = (1 << 6),
	InLeft = (1 << 7),
	InRight = (1 << 8),
	InMoveleft = (1 << 9),
	InMoveright = (1 << 10),
	InAttack2 = (1 << 11),
	InRun = (1 << 12),
	InReload = (1 << 13),
	InAlt1 = (1 << 14),
	InAlt2 = (1 << 15),
	InScore = (1 << 16),
	InSpeed = (1 << 17),
	InWalk = (1 << 18),
	InZoom = (1 << 19),
	InWeapon1 = (1 << 20),
	InWeapon2 = (1 << 21),
	InBullrush = (1 << 22),
	InGrenade1 = (1 << 23),
	InBrenade2 = (1 << 24)
};

class CUserCmd {
public:
	// pad
	int		_pad;

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	Vec3D	viewangles;
	// Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	BYTE    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;
};