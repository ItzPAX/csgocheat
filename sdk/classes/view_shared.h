#pragma once
#include "utilities/structures/matrix/matrix.h"

class CEventInfo
{
public:
	short				nClassID;		// 0x00 // 0 implies not in use
	float				flFireDelay;	// 0x02 // if non-zero, the delay time when the event should be fired ( fixed up on the client )
	const void* pSendTable;		// 0x06
	const ClientClass* pClientClass;	// 0x0A // clienclass pointer
	void* pData;			// 0x0E // raw event data
	std::intptr_t		iPackedBits;	// 0x12
	int					iFlags;			// 0x16
	std::byte			pad0[0x16];		// 0x1A
}; // Size: 0x30

class INetChannel;
class IClientState
{
public:
	std::byte		pad0[0x9C];				// 0x0000
	INetChannel* pNetChannel;			// 0x009C
	int				iChallengeNr;			// 0x00A0
	std::byte		pad1[0x64];				// 0x00A4
	int				iSignonState;			// 0x0108
	std::byte		pad2[0x8];				// 0x010C
	float			flNextCmdTime;			// 0x0114
	int				nServerCount;			// 0x0118
	int				iCurrentSequence;		// 0x011C
	std::byte		pad3[0x54];				// 0x0120
	int				iDeltaTick;				// 0x0174
	bool			bPaused;				// 0x0178
	std::byte		pad4[0x7];				// 0x0179
	int				iViewEntity;			// 0x0180
	int				iPlayerSlot;			// 0x0184
	char			szLevelName[MAX_PATH];	// 0x0188
	char			szLevelNameShort[80];	// 0x028C
	char			szMapGroupName[80];		// 0x02DC
	char			szLastLevelNameShort[80]; // 0x032C
	std::byte		pad5[0xC];				// 0x037C
	int				nMaxClients;			// 0x0388 
	std::byte		pad6[0x498C];			// 0x038C
	float			flLastServerTickTime;	// 0x4D18
	bool			bInSimulation;			// 0x4D1C
	std::byte		pad7[0x3];				// 0x4D1D
	int				iOldTickcount;			// 0x4D20
	float			flTickRemainder;		// 0x4D24
	float			flFrameTime;			// 0x4D28
	int				iLastOutgoingCommand;	// 0x4D2C
	int				nChokedCommands;		// 0x4D30
	int				iLastCommandAck;		// 0x4D34
	int				iCommandAck;			// 0x4D38
	int				iSoundSequence;			// 0x4D3C
	std::byte		pad8[0x50];				// 0x4D40
	Vec3D			angViewPoint;			// 0x4D90
	std::byte		pad9[0xD0];				// 0x4D9C
	CEventInfo* pEvents;				// 0x4E6C
}; // Size: 0x4E70

//-----------------------------------------------------------------------------
// Flags passed in with view setup
//-----------------------------------------------------------------------------
enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};


enum MotionBlurMode_t
{
	MOTION_BLUR_DISABLE = 1,
	MOTION_BLUR_GAME = 2,			// Game uses real-time inter-frame data
	MOTION_BLUR_SFM = 3				// Use SFM data passed in CViewSetup structure
};

class CViewSetup
{
public:
	int			iX;
	int			iUnscaledX;
	int			iY;
	int			iUnscaledY;
	int			iWidth;
	int			iUnscaledWidth;
	int			iHeight;
	int			iUnscaledHeight;
	bool		bOrtho;
	std::byte	pad0[0x8F];
	float		flFOV;
	float		flViewModelFOV;
	Vec3D		vecOrigin;
	Vec3D		angView;
	float		flNearZ;
	float		flFarZ;
	float		flNearViewmodelZ;
	float		flFarViewmodelZ;
	float		flAspectRatio;
	float		flNearBlurDepth;
	float		flNearFocusDepth;
	float		flFarFocusDepth;
	float		flFarBlurDepth;
	float		flNearBlurRadius;
	float		flFarBlurRadius;
	float		flDoFQuality;
	int			nMotionBlurMode;
	float		flShutterTime;
	Vec3D		vecShutterOpenPosition;
	Vec3D		vecShutterOpenAngles;
	Vec3D		vecShutterClosePosition;
	Vec3D		vecShutterCloseAngles;
	float		flOffCenterTop;
	float		flOffCenterBottom;
	float		flOffCenterLeft;
	float		flOffCenterRight;
	bool		bOffCenter;
	bool		bRenderToSubrectOfLargerScreen;
	bool		bDoBloomAndToneMapping;
	bool		bDoDepthOfField;
	bool		bHDRTarget;
	bool		bDrawWorldNormal;
	bool		bCullFontFaces;
	bool		bCacheFullSceneState;
	bool		bCSMView;
};