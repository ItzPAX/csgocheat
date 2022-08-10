#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/datacache/imdlcache.h
using MDLHandle_t = std::uint16_t;

#pragma region mdlcache_enumerations
enum
{
	MDLHANDLE_INVALID = static_cast<MDLHandle_t>(~0)
};

enum MDLCacheDataType_t : int
{
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

enum MDLCacheFlush_t : unsigned int
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,
	MDLCACHE_FLUSH_COMBINED_DATA = 0x80,
	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

enum ECombinedAsset
{
	COMBINED_ASSET_MATERIAL = 0,
	COMBINED_ASSET_TEXTURE,

	COMBINED_ASSET_MAX
};
#pragma endregion

class IVTFTexture;
class KeyValues;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

//-----------------------------------------------------------------------------
// Combiner
//-----------------------------------------------------------------------------
#define COMBINER_MAX_MODELS						15
#define COMBINER_MAX_LODS						8
#define COMBINER_MAX_TEXTURES_PER_MATERIAL		5
#define COMBINER_MAX_ATLAS_GROUPS				5
#define COMBINER_MAX_MATERIALS_PER_INPUT_MODEL	4

typedef struct SCombinedResults
{
	// individual model info
	int							m_nNumVerts[COMBINER_MAX_MODELS][COMBINER_MAX_LODS];
	int							m_nNumIndexes[COMBINER_MAX_MODELS][COMBINER_MAX_LODS];
	int							m_nBatches[COMBINER_MAX_MODELS][COMBINER_MAX_LODS];
	int							m_nNumLODs[COMBINER_MAX_MODELS];

	// cumulative combined info
	int							m_nCombinedNumVerts[COMBINER_MAX_LODS];
	int							m_nCombinedNumIndexes[COMBINER_MAX_LODS];
	int							m_nCombinedBatches[COMBINER_MAX_LODS];
	int							m_nCombinedNumLODs;
	int							m_nNumTexturePackIterations;
	unsigned int				m_nCombinedResults;
	char						m_szErrorMessage[256];
	char						m_szErrorDetails[4096];
	unsigned int				m_nDetailedError;
	float						m_flModelLoadDuration;
	float						m_flModelCombineDuration;
	float						m_flTextureLoadDuration;
	float						m_flTextureCombineDuration;
	float						m_flEngineProcessingDuration;
} TCombinedResults;


struct STextureSubstitute_t
{
	STextureSubstitute_t()
		: m_iszMaterialParam()
		, m_pVTFTexture(NULL)
	{
	}

	string_t	m_iszMaterialParam;
	IVTFTexture* m_pVTFTexture;
};

struct SCombinerModelInput_t
{
	SCombinerModelInput_t()
		: m_iszModelName()
		, m_nSkinFamily(0)
		, m_nBodyGroupSubModelSelection(-1)
	{
	}

	SCombinerModelInput_t(string_t iszModelName, int nSkinFamily = 0, int nBodyGroupSubModelSelection = -1)
		: m_iszModelName(iszModelName)
		, m_nSkinFamily(nSkinFamily)
		, m_nBodyGroupSubModelSelection(nBodyGroupSubModelSelection)
	{
	}

	string_t				m_iszModelName;
	int						m_nSkinFamily;
	int						m_nBodyGroupSubModelSelection; // -1 means no selection, include all body part sub models
	STextureSubstitute_t	m_textureSubstitutes[COMBINER_MAX_MATERIALS_PER_INPUT_MODEL][COMBINER_MAX_TEXTURES_PER_MATERIAL];	// entries can have NULL contents, it means don't substitute (use the one the mdl says)
};

class IMDLCacheNotify
{
public:
	virtual void OnDataLoaded(MDLCacheDataType_t nType, MDLHandle_t hModel) = 0;
	virtual void OnCombinerPreCache(MDLHandle_t hOldHandle, MDLHandle_t hNewHandle) = 0;
	virtual void OnDataUnloaded(MDLCacheDataType_t nType, MDLHandle_t hModel) = 0;
	virtual bool ShouldSupressLoadWarning(MDLHandle_t hModel) = 0;
};

// Callback for the combining process
// pUserData: the pointer you optionally specified in the FinishedCombinedModel() call
// OldHandle: either the temporary model model returned by CreateCombinedModel() or MDLHANDLE_INVALID if reusing an existing handle
// NewHandle: the combined model handle, which may be your reused model handle specified in CreateCombinedModel()
// CombinedResults: various statistical and timing information about the model and combining process
typedef void (*CombinedModelLoadedCallback)(void* pUserData, MDLHandle_t OldHandle, MDLHandle_t NewHandle, TCombinedResults& CombinedResults);

class IMDLCache : public IAppSystem
{
public:
	// Used to install callbacks for when data is loaded + unloaded
	// Returns the prior notify
	virtual void SetCacheNotify(IMDLCacheNotify* pNotify) = 0;

	// NOTE: This assumes the "GAME" path if you don't use
	// the UNC method of specifying files. This will also increment
	// the reference count of the MDL
	virtual MDLHandle_t FindMDL(const char* pMDLRelativePath) = 0;

	// Reference counting
	virtual int AddRef(MDLHandle_t handle) = 0;
	virtual int Release(MDLHandle_t handle) = 0;
	virtual int GetRef(MDLHandle_t handle) = 0;

	// Gets at the various data associated with a MDL
	virtual StudioHDR* GetStudioHdr(MDLHandle_t handle) = 0;
	virtual StudioHWData* GetHardwareData(MDLHandle_t handle) = 0;
	virtual vcollide_t* GetVCollide(MDLHandle_t handle) = 0;
	virtual unsigned char* GetAnimBlock(MDLHandle_t handle, int nBlock, bool preloadIfMissing) = 0;
	virtual bool HasAnimBlockBeenPreloaded(MDLHandle_t handle, int nBlock) = 0;
	virtual virtualmodel_t* GetVirtualModel(MDLHandle_t handle) = 0;
	virtual int GetAutoplayList(MDLHandle_t handle, unsigned short** pOut) = 0;
	virtual vertexFileHeader_t* GetVertexData(MDLHandle_t handle) = 0;

	// Brings all data associated with an MDL into memory
	virtual void TouchAllData(MDLHandle_t handle) = 0;

	// Gets/sets user data associated with the MDL
	virtual void SetUserData(MDLHandle_t handle, void* pData) = 0;
	virtual void* GetUserData(MDLHandle_t handle) = 0;

	// Is this MDL using the error model?
	virtual bool IsErrorModel(MDLHandle_t handle) = 0;
	virtual bool IsOverBudget(MDLHandle_t handle) = 0;

	// Flushes the cache, force a full discard
	virtual void Flush(MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Flushes a particular model out of memory
	virtual void Flush(MDLHandle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Returns the name of the model (its relative path)
	virtual const char* GetModelName(MDLHandle_t handle) = 0;

	virtual void* GetCacheSection(MDLCacheDataType_t type) = 0;

	// faster access when you already have the studiohdr
	virtual virtualmodel_t* GetVirtualModelFast(const StudioHDR* pStudioHdr, MDLHandle_t handle) = 0;

	// all cache entries that subsequently allocated or successfully checked 
	// are considered "locked" and will not be freed when additional memory is needed
	virtual void BeginLock() = 0;

	// reset all protected blocks to normal
	virtual void EndLock() = 0;

	// returns a pointer to a counter that is incremented every time the cache has been out of the locked state (EVIL)
	virtual int* GetFrameUnlockCounterPtrOLD() = 0;

	// Finish all pending async operations
	virtual void FinishPendingLoads() = 0;

	virtual vcollide_t* GetVCollideEx(MDLHandle_t handle, bool synchronousLoad = true) = 0;
	virtual bool GetVCollideSize(MDLHandle_t handle, int* pVCollideSize) = 0;

	virtual bool GetAsyncLoad(MDLCacheDataType_t type) = 0;
	virtual bool SetAsyncLoad(MDLCacheDataType_t type, bool bAsync) = 0;

	virtual void BeginMapLoad() = 0;
	virtual void EndMapLoad() = 0;
	virtual void MarkAsLoaded(MDLHandle_t handle) = 0;

	virtual void InitPreloadData(bool rebuild) = 0;
	virtual void ShutdownPreloadData() = 0;

	virtual bool IsDataLoaded(MDLHandle_t handle, MDLCacheDataType_t type) = 0;

	virtual int* GetFrameUnlockCounterPtr(MDLCacheDataType_t type) = 0;

	virtual StudioHDR* LockStudioHdr(MDLHandle_t handle) = 0;
	virtual void UnlockStudioHdr(MDLHandle_t handle) = 0;

	virtual bool PreloadModel(MDLHandle_t handle) = 0;

	// Hammer uses this. If a model has an error loading in GetStudioHdr, then it is flagged
	// as an error model and any further attempts to load it will just get the error model.
	// That is, until you call this function. Then it will load the correct model.
	virtual void ResetErrorModelStatus(MDLHandle_t handle) = 0;

	virtual void MarkFrame() = 0;

	// Locking for things that we can lock over longer intervals than
	// resources locked by BeginLock/EndLock
	virtual void BeginCoarseLock() = 0;
	virtual void EndCoarseLock() = 0;

	virtual void ReloadVCollide(MDLHandle_t handle) = 0;

	virtual bool ReleaseAnimBlockAllocator() = 0;

	virtual bool RestoreHardwareData(MDLHandle_t handle, void* pAsyncVTXControl, void* pAsyncVVDControl) = 0;

	virtual void DisableVCollideLoad() = 0;
	virtual void EnableVCollideLoad() = 0;

	virtual void DisableFileNotFoundWarnings() = 0;
	virtual void EnableFileNotFoundWarnings() = 0;

	virtual bool ProcessPendingHardwareRestore() = 0;

	virtual void UnloadQueuedHardwareData() = 0;

	virtual void DumpDictionaryState() = 0;

	// ========================
	// combined models

	// This will create a combined model by the name specified in pszModelName and return back a temporary model handle.
	// May return MDLHANDLE_INVALID if the name is a duplicate of an existing model
	virtual MDLHandle_t	CreateCombinedModel(const char* pszModelName) = 0;

	// This will create a combined model in replace of an existing model handle.  The handle should be valid to use during the entire process.
	// Returns true if the handle is acceptable to use for the combining process.
	virtual bool		CreateCombinedModel(MDLHandle_t handle) = 0;

	// Sets the models for the combining process
	virtual bool		SetCombineModels(MDLHandle_t handle, const CUtlVector< SCombinerModelInput_t >& vecModelsToCombine) = 0;

	// Indicates that you are done with the setup of the combining process.  You must specify a callback function and optionally
	// a user pointer that will be supplied to you in the callback
	virtual bool		FinishCombinedModel(MDLHandle_t handle, CombinedModelLoadedCallback pFunc, void* pUserData = NULL) = 0;

	// Lets you query a model handle to see if it is a placeholder returned by CreateCombinedModel()
	virtual bool		IsCombinedPlaceholder(MDLHandle_t handle) = 0;

	// Lets you query a model handle to see if it is a combined model
	virtual bool		IsCombinedModel(MDLHandle_t handle) = 0;

	// Returns the number of models contained in the combined model.  Includes primary and all secondary models
	virtual int			GetNumCombinedSubModels(MDLHandle_t handle) = 0;

	// Gets the file name for a sub model
	virtual void		GetCombinedSubModelFilename(MDLHandle_t handle, int nSubModelIndex, char* pszResult, int nResultSize) = 0;

	// Returns the KV of the material set for the model.  You are responsible for calling deleteThis() on the result.
	virtual KeyValues* GetCombinedMaterialKV(MDLHandle_t handle, int nAtlasGroup = 0) = 0;

	// Internal engine use function to help drive the combining process.
	virtual void		UpdateCombiner() = 0;	// should be called from main thread after rendering has completed

	// Internal engine function to help get access to specific combined assets
	virtual void* GetCombinedInternalAsset(ECombinedAsset AssetType, const char* pszAssetID = NULL, int* nSize = NULL) = 0;

	// Set Combiner Flags
	virtual void		SetCombinerFlags(unsigned nFlags) = 0;

	// Clear Combiner Flags
	virtual void		ClearCombinerFlags(unsigned nFlags) = 0;

	// Dump out resident combiner info
	virtual void		DebugCombinerInfo() = 0;
};

class CMDLCacheCriticalSection
{
public:
	CMDLCacheCriticalSection(IMDLCache* pCache)
		: pCache(pCache)
	{
		this->pCache->BeginLock();
	}

	~CMDLCacheCriticalSection()
	{
		this->pCache->EndLock();
	}
private:
	IMDLCache* pCache;
};