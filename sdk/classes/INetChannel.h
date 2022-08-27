#pragma once

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

class INetMessage;

class INetChannelInfo
{
public:

	enum NETMsg{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	// get channel name
	virtual const char* GetAddress(void) const = 0; // get channel IP address as string
	virtual float		GetTime(void) const = 0;	// current net time
	virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
	virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut(void) const = 0;	// true if timing out
	virtual bool		IsPlayback(void) const = 0;	// true if demo playback

	virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};

class INetChannel
{
public:
	std::byte	pad0[0x14];				//0x0000
	bool		bProcessingMessages;	//0x0014
	bool		bShouldDelete;			//0x0015
	bool		bStopProcessing;		//0x0016
	std::byte	pad1[0x1];				//0x0017
	int			iOutSequenceNr;			//0x0018 last send outgoing sequence number
	int			iInSequenceNr;			//0x001C last received incoming sequence number
	int			iOutSequenceNrAck;		//0x0020 last received acknowledge outgoing sequence number
	int			iOutReliableState;		//0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int			iInReliableState;		//0x0028 state of incoming reliable data
	int			iChokedPackets;			//0x002C number of choked packets
	std::byte	pad2[0x414];			//0x0030

	bool SendNetMsg(INetMessage& msg, bool rel = false, bool audio = false) 
	{
		using original_fn = void(__thiscall*)(INetChannel*, INetMessage&, bool, bool);
		(*(original_fn**)this)[40](this, msg, rel, audio);
	}

	int	SendDatagram(bf_write* pDatagram)
	{
		using original_fn = void(__thiscall*)(INetChannel*, bf_write*);
		(*(original_fn**)this)[46](this, pDatagram);
	}

	bool Transmit(bool bOnlyReliable = false)
	{
		using original_fn = void(__thiscall*)(INetChannel*, bool);
		(*(original_fn**)this)[46](this, bOnlyReliable);
	}
}; // Size: 0x0444

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/inetmessage.h
class INetMessage
{
public:
	virtual					~INetMessage() { }
	virtual void			SetNetChannel(INetChannel* pNetChannel) = 0;
	virtual void			SetReliable(bool bState) = 0;
	virtual bool			Process() = 0;
	virtual	bool			ReadFromBuffer(bf_read& buffer) = 0;
	virtual	bool			WriteToBuffer(bf_write& buffer) = 0;
	virtual bool			IsReliable() const = 0;
	virtual int				GetType() const = 0;
	virtual int				GetGroup() const = 0;
	virtual const char*		GetName() const = 0;
	virtual INetChannel*	GetNetChannel() const = 0;
	virtual const char*		ToString() const = 0;
	virtual size_t			GetSize() const = 0;
};

struct CCLCMsg_ClientInfo
{
	char pad[0xC];
	uint32_t send_table_crc;
	uint32_t server_count;
	bool is_hltv;
	bool is_replay;
	uint32_t friends_id;
	const char* friends_name;
	uint32_t custom_files;
};

struct CCLCMsg_Move
{
	char pad[0xC];
	int backup_commands;
	int new_commands;
};

struct CCLCMsg_FileCRCCheck
{
	char pad[0xC];
	int code_path;
	//NOT COMPLETE
};

struct CCLCMsg_LoadingProgress
{
	char pad[0xC];
	int progress = 1;
};

struct CCLCMsg_RespondCvarValue
{
	char pad[0xC];
	int cookie;										// QueryCvarCookie_t
	int status_code;								// EQueryCvarValueStatus
	const char* name;
	const char* value;
};

enum VoiceDataFormat_t
{
	VOICEDATA_FORMAT_STEAM = 0,						//steam uses SILK
	VOICEDATA_FORMAT_ENGINE = 1						// was speex, switching to celt
};

struct CCLCMsg_VoiceData
{
	char pad[0xC];
	const char* data;
	uint64_t xuid;
	VoiceDataFormat_t format;
	int sequence_bytes;								// This is a TCP-style sequence number, so it includes the current packet length.  So it's actually the offset within the compressed data stream of the next packet to follow (if any).
	uint32_t section_number;
	uint32_t uncompressed_sample_offset;
};

struct CNETMsg_StringCmd
{
	char pad[0xC];
	const char* command;
};

struct CNETMsg_Tick
{
	char pad[0xC];
	uint32_t tick;									// current tick count
	uint32_t host_computationtime;					// Host frame computation time in usec (1/1,000,000th sec) - will be say 4 ms when server is running at 25% CPU load for 64-tick server
	uint32_t host_computationtime_std_deviation;    // Host frame computation time stddev in usec (1/1,000,000th sec)
	uint32_t host_framestarttime_std_deviation;		// Host frame start time stddev in usec (1/1,000,000th sec) - measures how precisely we can wake up from sleep when meeting server framerate
	uint32_t hltv_replay_flags;						// 0 or absent by default, 1 when hltv replay is in progress - used to fix client state in case of server crashes of full frame updates
};