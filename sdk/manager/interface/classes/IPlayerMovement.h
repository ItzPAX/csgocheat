#pragma once

enum ESoundLevel;
class PlayerMoveHelper {
public:
	virtual	const char* GetName(void* hEntity) const = 0;
	virtual void				SetHost(Player* pHost) = 0;
	virtual void				ResetTouchList() = 0;
	virtual bool				AddToTouched(const CGameTrace& trace, const Vec3D& vecImpactVelocity) = 0;
	virtual void				ProcessImpacts() = 0;
	virtual void				Con_NPrintf(int nIndex, char const* fmt, ...) = 0;
	virtual void				StartSound(const Vec3D& vecOrigin, int iChannel, char const* szSample, float flVolume, ESoundLevel soundlevel, int fFlags, int iPitch) = 0;
	virtual void				StartSound(const Vec3D& vecOrigin, const char* soundname) = 0;
	virtual void				PlaybackEventFull(int fFlags, int nClientIndex, unsigned short uEventIndex, float flDelay, Vec3D& vecOrigin, Vec3D& vecAngles, float flParam1, float flParam2, int iParam1, int iParam2, int bParam1, int bParam2) = 0;
	virtual bool				PlayerFallingDamage() = 0;
	virtual void				PlayerSetAnimation(int playerAnimation) = 0;
	virtual IPhysicsSurfaceProps* GetSurfaceProps() = 0;
	virtual bool				IsWorldEntity(const unsigned long& hEntity) = 0;
};

class PlayerMoveData {
public:
	bool    first_run_of_instructions : 1;
	bool    game_code_moved_player : 1;
	int     player_handle;
	int     impulse_command;
	Vec3D	view_angles;
	Vec3D	abs_view_angles;
	int     buttons;
	int     old_buttons;
	float   fw_move;
	float   sd_move;
	float   up_move;
	float   max_speed;
	float   client_max_speed;
	Vec3D	velocity;
	Vec3D	angles;
	Vec3D	old_angles;
	float   step_height;
	Vec3D	wish_velocity;
	Vec3D	jump_velocity;
	Vec3D	constraint_center;
	float   constraint_radius;
	float   constraint_width;
	float   constraint_speed_factor;
	float   u0[5];
	Vec3D	abs_origin;
};

class VirtualGameMovement {

public:
	virtual					~VirtualGameMovement(void) {}
	virtual void			ProcessMovement(Player* player, PlayerMoveData* move) = 0;
	virtual void			Reset(void) = 0;
	virtual void			StartTrackPredictionErrors(Player* player) = 0;
	virtual void			FinishTrackPredictionErrors(Player* player) = 0;
	virtual void			DiffPrint(char const* fmt, ...) = 0;
	virtual Vec3D const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vec3D const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vec3D const&	GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool			is_moving_player_stuck(void) const = 0;
	virtual Player*			GetMovingPlayer(void) const = 0;
	virtual void			UnblockPosher(Player* player, Player* pusher) = 0;
	virtual void			SetupMovementBounds(PlayerMoveData* move) = 0;
};

class PlayerGameMovement : public VirtualGameMovement {
public:
	virtual ~PlayerGameMovement(void) { }
};

class PlayerPrediction {
	public:
		std::byte		pad0[0x4];						// 0x0000
		std::uintptr_t	hLastGround;					// 0x0004
		bool			bInPrediction;					// 0x0008
		bool			bIsFirstTimePredicted;			// 0x0009
		bool			bEnginePaused;					// 0x000A
		bool			bOldCLPredictValue;				// 0x000B
		int				iPreviousStartFrame;			// 0x000C
		int				nIncomingPacketNumber;			// 0x0010
		float			flLastServerWorldTimeStamp;		// 0x0014

		struct Split_t
		{
			bool		bIsFirstTimePredicted;			// 0x0018
			std::byte	pad0[0x3];						// 0x0019
			int			nCommandsPredicted;				// 0x001C
			int			nServerCommandsAcknowledged;	// 0x0020
			int			iPreviousAckHadErrors;			// 0x0024
			float		flIdealPitch;					// 0x0028
			int			iLastCommandAcknowledged;		// 0x002C
			bool		bPreviousAckErrorTriggersFullLatchReset; // 0x0030
			CUtlVector<CBaseHandle> vecEntitiesWithPredictionErrorsInLastAck; // 0x0031
			bool		bPerformedTickShift;			// 0x0045
		};

		Split_t			Split[1];						// 0x0018
		// SavedGlobals 0x4C

	public:
		void Update(int iStartFrame, bool bValidFrame, int nIncomingAcknowledged, int nOutgoingCommand)
		{
			using original_fn = void(__thiscall*)(void*, int, bool, int, int);
			(*(original_fn**)this)[3](this, iStartFrame, bValidFrame, nIncomingAcknowledged, nOutgoingCommand);
		}

		void GetLocalViewAngles(Vec3D& angView)
		{
			using original_fn = void(__thiscall*)(void*, Vec3D&);
			(*(original_fn**)this)[12](this, std::ref(angView));
		}

		void SetLocalViewAngles(Vec3D& angView)
		{
			using original_fn = void(__thiscall*)(void*, Vec3D&);
			(*(original_fn**)this)[13](this, std::ref(angView));
		}

		void CheckMovingGround(Player * pEntity, double dbFrametime)
		{
			using original_fn = void(__thiscall*)(void*, Player*, double);
			(*(original_fn**)this)[18](this, pEntity, dbFrametime);
		}

		void SetupMove(Player* pEntity, CUserCmd * pCmd, PlayerMoveHelper * pHelper, PlayerMoveData * pMoveData)
		{
			using original_fn = void(__thiscall*)(void*, Player*, CUserCmd*, PlayerMoveHelper*, PlayerMoveData*);
			(*(original_fn**)this)[20](this, pEntity, pCmd, pHelper, pMoveData);
		}

		void FinishMove(Player * pEntity, CUserCmd * pCmd, PlayerMoveData* pMoveData)
		{
			using original_fn = void(__thiscall*)(void*, Player*, CUserCmd*, PlayerMoveData*);
			(*(original_fn**)this)[21](this, pEntity, pCmd, pMoveData);
		}
	};