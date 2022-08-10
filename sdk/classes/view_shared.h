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
	CViewSetup()
	{
		m_flAspectRatio = 0.0f;

		// These match mat_dof convars
		m_flNearBlurDepth = 20.0;
		m_flNearFocusDepth = 100.0;
		m_flFarFocusDepth = 250.0;
		m_flFarBlurDepth = 1000.0;
		m_flNearBlurRadius = 10.0;
		m_flFarBlurRadius = 5.0;
		m_nDoFQuality = 0;

		m_bRenderToSubrectOfLargerScreen = false;
		m_bDoBloomAndToneMapping = true;
		m_nMotionBlurMode = MOTION_BLUR_GAME;
		m_bDoDepthOfField = false;
		m_bHDRTarget = false;
		m_bOffCenter = false;
		m_bCacheFullSceneState = false;
		m_bDrawWorldNormal = false;
		m_bCullFrontFaces = false;
		m_bCustomViewMatrix = false;
		//		m_bUseExplicitViewVector = false;
		m_bCustomProjMatrix = false;
		m_bCSMView = false;
		m_pCSMVolumeCuller = 0;
	}

	float ComputeViewMatrices(void* pWorldToView, void* pViewToProjection, void* pWorldToProjection) const;

	// shared by 2D & 3D views

		// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	int			m_nUnscaledHeight;

	// X360 csm resolve offsets
#if defined(_X360)
	int			xCsmDstOffset;
	int			yCsmDstOffset;
#endif

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// Note: This really should be called "m_bCustomCameraMatrix", because m_matCustomViewMatrix is interpreted as world->camera, not world->view.
	bool		m_bCustomViewMatrix;
	Matrix		m_matCustomViewMatrix;

	bool		m_bCustomProjMatrix;
	Matrix		m_matCustomProjMatrix;

	const void* m_pCSMVolumeCuller;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vec3D		origin;

	// heading of camera (pitch, yaw, roll)
	Vec3D		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Camera settings to control depth of field
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;

	// Camera settings to control motion blur
	MotionBlurMode_t	m_nMotionBlurMode;
	float	m_flShutterTime;				// In seconds
	Vec3D	m_vShutterOpenPosition;			// Start of frame or "shutter open"
	Vec3D	m_shutterOpenAngles;			//
	Vec3D	m_vShutterClosePosition;		// End of frame or "shutter close"
	Vec3D	m_shutterCloseAngles;			// 

	// Controls for off-center projection (needed for poster rendering)
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bOffCenter : 1;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen : 1;

	// Controls that the SFM needs to tell the engine when to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping : 1;
	bool		m_bDoDepthOfField : 1;
	bool		m_bHDRTarget : 1;
	bool		m_bDrawWorldNormal : 1;
	bool		m_bCullFrontFaces : 1;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState : 1;

	// True if this is a CSM depth view. The CSM world->view matrix doesn't have an XY translation (that's moved into the CSM ortho view->projection 
	// matrix to address continuity issues), so the usual assumptions made about camera/view space do not necessarily apply.
	bool		m_bCSMView : 1;
};
