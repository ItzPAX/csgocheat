#pragma once

class IGameResources
{
public:
	virtual	~IGameResources() {};

	// Team data access 
	virtual const char* GetTeamName(int index) = 0;
	virtual int				GetTeamScore(int index) = 0;
	virtual const Color& GetTeamColor(int index) = 0;

	// Player data access
	virtual bool	IsConnected(int index) = 0;
	virtual bool	IsAlive(int index) = 0;
	virtual bool	IsFakePlayer(int index) = 0;
	virtual bool	IsLocalPlayer(int index) = 0;

	virtual const char* GetPlayerName(int index) = 0;
	virtual int		GetKills(int index) = 0;
	virtual int		GetPing(int index) = 0;
	//	virtual int		GetPacketloss( int index ) = 0;
	virtual int		GetDeaths(int index) = 0;
	virtual int		GetFrags(int index) = 0;
	virtual int		GetTeam(int index) = 0;
	virtual int		GetHealth(int index) = 0;
};