#pragma once

class CUserCmd {
public:
	enum Buttons {
		IN_ATTACK =		(1 << 0),
		IN_JUMP =		(1 << 1),
		IN_DUCK =		(1 << 2),
		IN_FORWARD =	(1 << 3),
		IN_BACK =		(1 << 4),
		IN_USE =		(1 << 5),
		IN_CANCEL =		(1 << 6),
		IN_LEFT =		(1 << 7),
		IN_RIGHT =		(1 << 8),
		IN_MOVELEFT =	(1 << 9),
		IN_MOVERIGHT =	(1 << 10),
		IN_ATTACK2 =	(1 << 11),
		IN_RUN =		(1 << 12),
		IN_RELOAD =		(1 << 13),
		IN_ALT1 =		(1 << 14),
		IN_ALT2 =		(1 << 15),
		IN_SCORE =		(1 << 16),  // Used by client.dll for when scoreboard is held down
		IN_SPEED =		(1 << 17),	// Player is holding the speed key
		IN_WALK =		(1 << 18),	// Player holding walk key
		IN_ZOOM =		(1 << 19),	// Zoom key for HUD zoom
		IN_WEAPON1 =	(1 << 20),	// weapon defines these bits
		IN_WEAPON2 =	(1 << 21),	// weapon defines these bits
		IN_BULLRUSH =	(1 << 22),
		IN_GRENADE1 =	(1 << 23),	// grenade 1
		IN_GRENADE2 =	(1 << 24),	// grenade 2
		IN_ATTACK3 =	(1 << 25),
	};


public:
	// pad
	int		_pad;

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	Vec3D	viewangles;

	Vec3D aimdirection;

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