// Spectator Movement modes
enum
{
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

/* settings for m_takedamage */
enum EDamageType : int
{
	DAMAGE_NO = 0,
	DAMAGE_EVENTS_ONLY,
	DAMAGE_YES,
	DAMAGE_AIM
};

enum EPropertyDataInteractions : int
{
	PROPINTER_PHYSGUN_WORLD_STICK,		// "onworldimpact"		"stick"
	PROPINTER_PHYSGUN_FIRST_BREAK,		// "onfirstimpact"		"break"
	PROPINTER_PHYSGUN_FIRST_PAINT,		// "onfirstimpact"		"paintsplat"
	PROPINTER_PHYSGUN_FIRST_IMPALE,		// "onfirstimpact"		"impale"
	PROPINTER_PHYSGUN_LAUNCH_SPIN_NONE,	// "onlaunch"			"spin_none"
	PROPINTER_PHYSGUN_LAUNCH_SPIN_Z,	// "onlaunch"			"spin_zaxis"
	PROPINTER_PHYSGUN_BREAK_EXPLODE,	// "onbreak"			"explode_fire"
	PROPINTER_PHYSGUN_BREAK_EXPLODE_ICE,// "onbreak"			"explode_ice"
	PROPINTER_PHYSGUN_DAMAGE_NONE,		// "damage"				"none"
	PROPINTER_FIRE_FLAMMABLE,			// "flammable"			"yes"
	PROPINTER_FIRE_EXPLOSIVE_RESIST,	// "explosive_resist"	"yes"
	PROPINTER_FIRE_IGNITE_HALFHEALTH,	// "ignite"				"halfhealth"
	PROPINTER_PHYSGUN_CREATE_FLARE,		// "onpickup"			"create_flare"
	PROPINTER_PHYSGUN_ALLOW_OVERHEAD,	// "allow_overhead"		"yes"
	PROPINTER_WORLD_BLOODSPLAT,			// "onworldimpact",		"bloodsplat"
	PROPINTER_PHYSGUN_NOTIFY_CHILDREN,	// "onfirstimpact" - cause attached flechettes to explode
	PROPINTER_MELEE_IMMUNE,				// "melee_immune"		"yes"
	PROPINTER_NUM_INTERACTIONS			// if we get more than 32 of these, we'll need a different system
};

enum EMultiplayerBreak : int
{
	MULTIPLAYER_BREAK_DEFAULT,
	MULTIPLAYER_BREAK_SERVERSIDE,
	MULTIPLAYER_BREAK_CLIENTSIDE,
	MULTIPLAYER_BREAK_BOTH
};

enum EMultiplayerPhysicsMode : int
{
	PHYSICS_MULTIPLAYER_AUTODETECT = 0,	// use multiplayer physics mode as defined in model prop data
	PHYSICS_MULTIPLAYER_SOLID = 1,	// solid, pushes player away
	PHYSICS_MULTIPLAYER_NON_SOLID = 2,	// nonsolid, but pushed by player
	PHYSICS_MULTIPLAYER_CLIENTSIDE = 3	// clientside only, nonsolid
};

class IMultiplayerPhysics
{
public:
	virtual int		GetMultiplayerPhysicsMode() = 0;
	virtual float	GetMass() = 0;
	virtual bool	IsAsleep() = 0;
};

struct string_t
{
public:
	bool operator!() const { return (szValue == nullptr); }
	bool operator==(const string_t& rhs) const { return (szValue == rhs.szValue); }
	bool operator!=(const string_t& rhs) const { return (szValue != rhs.szValue); }
	bool operator<(const string_t& rhs) const { return (reinterpret_cast<void*>(const_cast<char*>(szValue)) < reinterpret_cast<void*>(const_cast<char*>(rhs.szValue))); }

	const char* c_str() const { return (szValue) ? szValue : ""; }
protected:
	const char* szValue;
};

enum EItemDefinitionIndices : int {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

class IBreakableWithPropData
{
public:
	// Damage modifiers
	virtual void		SetDmgModBullet(float flDmgMod) = 0;
	virtual void		SetDmgModClub(float flDmgMod) = 0;
	virtual void		SetDmgModExplosive(float flDmgMod) = 0;
	virtual float		GetDmgModBullet() = 0;
	virtual float		GetDmgModClub() = 0;
	virtual float		GetDmgModExplosive() = 0;
	virtual float		GetDmgModFire() = 0;

	// Explosive
	virtual void		SetExplosiveRadius(float flRadius) = 0;
	virtual void		SetExplosiveDamage(float flDamage) = 0;
	virtual float		GetExplosiveRadius() = 0;
	virtual float		GetExplosiveDamage() = 0;

	// Physics damage tables
	virtual void		SetPhysicsDamageTable(string_t iszTableName) = 0;
	virtual string_t	GetPhysicsDamageTable() = 0;

	// Breakable chunks
	virtual void		SetBreakableModel(string_t iszModel) = 0;
	virtual string_t 	GetBreakableModel() = 0;
	virtual void		SetBreakableSkin(int iSkin) = 0;
	virtual int			GetBreakableSkin() = 0;
	virtual void		SetBreakableCount(int iCount) = 0;
	virtual int			GetBreakableCount() = 0;
	virtual void		SetMaxBreakableSize(int iSize) = 0;
	virtual int			GetMaxBreakableSize() = 0;

	// LOS blocking
	virtual void		SetPropDataBlocksLOS(bool bBlocksLOS) = 0;
	virtual void		SetPropDataIsAIWalkable(bool bBlocksLOS) = 0;

	// Interactions
	virtual void		SetInteraction(EPropertyDataInteractions Interaction) = 0;
	virtual bool		HasInteraction(EPropertyDataInteractions Interaction) = 0;

	// Multiplayer physics mode
	virtual void		SetPhysicsMode(int iMode) = 0;
	virtual int			GetPhysicsMode() = 0;

	// Multiplayer breakable spawn behavior
	virtual void		SetMultiplayerBreakMode(EMultiplayerBreak mode) = 0;
	virtual EMultiplayerBreak GetMultiplayerBreakMode() const = 0;

	// Used for debugging
	virtual void		SetBasePropData(string_t iszBase) = 0;
	virtual string_t	GetBasePropData() = 0;
};