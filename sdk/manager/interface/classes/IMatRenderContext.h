#pragma once

//-----------------------------------------------------------------------------
// important enumeration
//-----------------------------------------------------------------------------
enum ShaderParamType_t
{
	SHADER_PARAM_TYPE_TEXTURE,
	SHADER_PARAM_TYPE_INTEGER,
	SHADER_PARAM_TYPE_COLOR,
	SHADER_PARAM_TYPE_VEC2,
	SHADER_PARAM_TYPE_VEC3,
	SHADER_PARAM_TYPE_VEC4,
	SHADER_PARAM_TYPE_ENVMAP,	// obsolete
	SHADER_PARAM_TYPE_FLOAT,
	SHADER_PARAM_TYPE_BOOL,
	SHADER_PARAM_TYPE_FOURCC,
	SHADER_PARAM_TYPE_MATRIX,
	SHADER_PARAM_TYPE_MATERIAL,
	SHADER_PARAM_TYPE_STRING,
};

enum MaterialMatrixMode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,

	MATERIAL_MATRIX_UNUSED0,
	MATERIAL_MATRIX_UNUSED1,
	MATERIAL_MATRIX_UNUSED2,
	MATERIAL_MATRIX_UNUSED3,
	MATERIAL_MATRIX_UNUSED4,
	MATERIAL_MATRIX_UNUSED5,
	MATERIAL_MATRIX_UNUSED6,
	MATERIAL_MATRIX_UNUSED7,

	MATERIAL_MODEL,

	// Total number of matrices
	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,
};

// FIXME: How do I specify the actual number of matrix modes?
const int NUM_MODEL_TRANSFORMS = 53;
const int MATERIAL_MODEL_MAX = MATERIAL_MODEL + NUM_MODEL_TRANSFORMS;

//-----------------------------------------------------------------------------
// allowed stencil operations. These match the d3d operations
//-----------------------------------------------------------------------------
enum ShaderStencilOp_t
{
#if !defined( _X360 )
	SHADER_STENCILOP_KEEP = 1,
	SHADER_STENCILOP_ZERO = 2,
	SHADER_STENCILOP_SET_TO_REFERENCE = 3,
	SHADER_STENCILOP_INCREMENT_CLAMP = 4,
	SHADER_STENCILOP_DECREMENT_CLAMP = 5,
	SHADER_STENCILOP_INVERT = 6,
	SHADER_STENCILOP_INCREMENT_WRAP = 7,
	SHADER_STENCILOP_DECREMENT_WRAP = 8,
#else
	SHADER_STENCILOP_KEEP = D3DSTENCILOP_KEEP,
	SHADER_STENCILOP_ZERO = D3DSTENCILOP_ZERO,
	SHADER_STENCILOP_SET_TO_REFERENCE = D3DSTENCILOP_REPLACE,
	SHADER_STENCILOP_INCREMENT_CLAMP = D3DSTENCILOP_INCRSAT,
	SHADER_STENCILOP_DECREMENT_CLAMP = D3DSTENCILOP_DECRSAT,
	SHADER_STENCILOP_INVERT = D3DSTENCILOP_INVERT,
	SHADER_STENCILOP_INCREMENT_WRAP = D3DSTENCILOP_INCR,
	SHADER_STENCILOP_DECREMENT_WRAP = D3DSTENCILOP_DECR,
#endif
	SHADER_STENCILOP_FORCE_DWORD = 0x7fffffff
};


enum ShaderStencilFunc_t
{
#if !defined( _X360 )
	SHADER_STENCILFUNC_NEVER = 1,
	SHADER_STENCILFUNC_LESS = 2,
	SHADER_STENCILFUNC_EQUAL = 3,
	SHADER_STENCILFUNC_LEQUAL = 4,
	SHADER_STENCILFUNC_GREATER = 5,
	SHADER_STENCILFUNC_NOTEQUAL = 6,
	SHADER_STENCILFUNC_GEQUAL = 7,
	SHADER_STENCILFUNC_ALWAYS = 8,
#else
	SHADER_STENCILFUNC_NEVER = D3DCMP_NEVER,
	SHADER_STENCILFUNC_LESS = D3DCMP_LESS,
	SHADER_STENCILFUNC_EQUAL = D3DCMP_EQUAL,
	SHADER_STENCILFUNC_LEQUAL = D3DCMP_LESSEQUAL,
	SHADER_STENCILFUNC_GREATER = D3DCMP_GREATER,
	SHADER_STENCILFUNC_NOTEQUAL = D3DCMP_NOTEQUAL,
	SHADER_STENCILFUNC_GEQUAL = D3DCMP_GREATEREQUAL,
	SHADER_STENCILFUNC_ALWAYS = D3DCMP_ALWAYS,
#endif

	SHADER_STENCILFUNC_FORCE_DWORD = 0x7fffffff
};


enum MaterialPrimitiveType_t
{
	MATERIAL_POINTS = 0x0,
	MATERIAL_LINES,
	MATERIAL_TRIANGLES,
	MATERIAL_TRIANGLE_STRIP,
	MATERIAL_LINE_STRIP,
	MATERIAL_LINE_LOOP,	// a single line loop
	MATERIAL_POLYGON,	// this is a *single* polygon
	MATERIAL_QUADS,
	MATERIAL_SUBD_QUADS_EXTRA, // Extraordinary sub-d quads
	MATERIAL_SUBD_QUADS_REG,   // Regular sub-d quads
	MATERIAL_INSTANCED_QUADS, // (X360) like MATERIAL_QUADS, but uses vertex instancing

	// This is used for static meshes that contain multiple types of
	// primitive types.	When calling draw, you'll need to specify
	// a primitive type.
	MATERIAL_HETEROGENOUS
};

enum TessellationMode_t
{
	TESSELLATION_MODE_DISABLED = 0,
	TESSELLATION_MODE_ACC_PATCHES_EXTRA,
	TESSELLATION_MODE_ACC_PATCHES_REG
};

// acceptable property values for MATERIAL_PROPERTY_OPACITY
enum MaterialPropertyOpacityTypes_t
{
	MATERIAL_ALPHATEST = 0,
	MATERIAL_OPAQUE,
	MATERIAL_TRANSLUCENT
};

enum MaterialBufferTypes_t
{
	MATERIAL_FRONT = 0,
	MATERIAL_BACK
};

enum MaterialCullMode_t
{
	MATERIAL_CULLMODE_CCW,	// this culls polygons with counterclockwise winding
	MATERIAL_CULLMODE_CW,	// this culls polygons with clockwise winding
	MATERIAL_CULLMODE_NONE	// cull nothing
};

enum MaterialIndexFormat_t
{
	MATERIAL_INDEX_FORMAT_UNKNOWN = -1,
	MATERIAL_INDEX_FORMAT_16BIT = 0,
	MATERIAL_INDEX_FORMAT_32BIT,
};

enum MaterialFogMode_t
{
	MATERIAL_FOG_NONE,
	MATERIAL_FOG_LINEAR,
	MATERIAL_FOG_LINEAR_BELOW_FOG_Z,
};

enum MaterialHeightClipMode_t
{
	MATERIAL_HEIGHTCLIPMODE_DISABLE,
	MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT,
	MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
};

enum MaterialNonInteractiveMode_t
{
	MATERIAL_NON_INTERACTIVE_MODE_NONE = -1,
	MATERIAL_NON_INTERACTIVE_MODE_STARTUP = 0,
	MATERIAL_NON_INTERACTIVE_MODE_LEVEL_LOAD,

	MATERIAL_NON_INTERACTIVE_MODE_COUNT,
};

struct UberlightState_t
{
	UberlightState_t()
	{
		m_fNearEdge = 2.0f;
		m_fFarEdge = 100.0f;
		m_fCutOn = 10.0f;
		m_fCutOff = 650.0f;
		m_fShearx = 0.0f;
		m_fSheary = 0.0f;
		m_fWidth = 0.3f;
		m_fWedge = 0.05f;
		m_fHeight = 0.3f;
		m_fHedge = 0.05f;
		m_fRoundness = 0.8f;
	}

	float m_fNearEdge;
	float m_fFarEdge;
	float m_fCutOn;
	float m_fCutOff;
	float m_fShearx;
	float m_fSheary;
	float m_fWidth;
	float m_fWedge;
	float m_fHeight;
	float m_fHedge;
	float m_fRoundness;
};

//-----------------------------------------------------------------------------
// Stencil state
//-----------------------------------------------------------------------------
struct ShaderStencilState_t
{
	bool m_bEnable;
	ShaderStencilOp_t m_FailOp;
	ShaderStencilOp_t m_ZFailOp;
	ShaderStencilOp_t m_PassOp;
	ShaderStencilFunc_t m_CompareFunc;
	int m_nReferenceValue;
	unsigned int m_nTestMask;
	unsigned int m_nWriteMask;

#if defined( _X360 )
	bool m_bHiStencilEnable;
	bool m_bHiStencilWriteEnable;
	ShaderHiStencilFunc_t m_HiStencilCompareFunc;
	int m_nHiStencilReferenceValue;
#endif

	ShaderStencilState_t()
	{
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = SHADER_STENCILOP_KEEP;
		m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;

#if defined( _X360 )
		m_bHiStencilEnable = false;
		m_bHiStencilWriteEnable = false;
		m_HiStencilCompareFunc = SHADER_HI_STENCILFUNC_EQUAL;
		m_nHiStencilReferenceValue = 0;
#endif
	}
};

class ITexture;

// fixme: should move this into something else.
struct FlashlightState_t
{
	FlashlightState_t()
	{
		m_bEnableShadows = false;						// Provide reasonable defaults for shadow depth mapping parameters
		m_bDrawShadowFrustum = false;
		m_flShadowMapResolution = 1024.0f;
		m_flShadowFilterSize = 3.0f;
		m_flShadowSlopeScaleDepthBias = 16.0f;
		m_flShadowDepthBias = 0.0005f;
		m_flShadowJitterSeed = 0.0f;
		m_flShadowAtten = 0.0f;
		m_flAmbientOcclusion = 0.0f;
		m_nShadowQuality = 0;
		m_bShadowHighRes = false;

		m_bScissor = false;
		m_nLeft = -1;
		m_nTop = -1;
		m_nRight = -1;
		m_nBottom = -1;

		m_bUberlight = false;

		m_bVolumetric = false;
		m_flNoiseStrength = 0.8f;
		m_flFlashlightTime = 0.0f;
		m_nNumPlanes = 64;
		m_flPlaneOffset = 0.0f;
		m_flVolumetricIntensity = 1.0f;

		m_bOrtho = false;
		m_fOrthoLeft = -1.0f;
		m_fOrthoRight = 1.0f;
		m_fOrthoTop = -1.0f;
		m_fOrthoBottom = 1.0f;

		m_fBrightnessScale = 1.0f;
		m_pSpotlightTexture = NULL;
		m_pProjectedMaterial = NULL;
		m_bShareBetweenSplitscreenPlayers = false;
	}

	Vec3D m_vecLightOrigin;
	Quaternion m_quatOrientation;
	float m_NearZ;
	float m_FarZ;
	float m_fHorizontalFOVDegrees;
	float m_fVerticalFOVDegrees;
	bool  m_bOrtho;
	float m_fOrthoLeft;
	float m_fOrthoRight;
	float m_fOrthoTop;
	float m_fOrthoBottom;
	float m_fQuadraticAtten;
	float m_fLinearAtten;
	float m_fConstantAtten;
	float m_FarZAtten;
	float m_Color[4];
	float m_fBrightnessScale;
	ITexture* m_pSpotlightTexture;
	IMaterial* m_pProjectedMaterial;
	int m_nSpotlightTextureFrame;

	// Shadow depth mapping parameters
	bool  m_bEnableShadows;
	bool  m_bDrawShadowFrustum;
	float m_flShadowMapResolution;
	float m_flShadowFilterSize;
	float m_flShadowSlopeScaleDepthBias;
	float m_flShadowDepthBias;
	float m_flShadowJitterSeed;
	float m_flShadowAtten;
	float m_flAmbientOcclusion;
	int   m_nShadowQuality;
	bool  m_bShadowHighRes;

	// simple projection
	float m_flProjectionSize;
	float m_flProjectionRotation;

	// Uberlight parameters
	bool m_bUberlight;
	UberlightState_t m_uberlightState;

	bool m_bVolumetric;
	float m_flNoiseStrength;
	float m_flFlashlightTime;
	int m_nNumPlanes;
	float m_flPlaneOffset;
	float m_flVolumetricIntensity;
	bool m_bShareBetweenSplitscreenPlayers;	// When true, this flashlight will render for all splitscreen players

	// Getters for scissor members
	bool DoScissor() const { return m_bScissor; }
	int GetLeft()	 const { return m_nLeft; }
	int GetTop()	 const { return m_nTop; }
	int GetRight()	 const { return m_nRight; }
	int GetBottom()	 const { return m_nBottom; }

private:

	friend class CShadowMgr;

	bool m_bScissor;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
};

class VMatrix;
class IVertexBuffer;
class IIndexBuffer;
class IMorph;
class ICallQueue;
class MorphWeight_t;

class IRefCounted {
private:
	volatile long refCount;

public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

class IMatRenderContext : public IRefCounted
{
public:
	void BeginRender()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[2](this);
	}

	void EndRender()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[3](this);
	}

	void BindLocalCubemap(ITexture* pTexture)
	{
		using original_fn = void(__thiscall*)(void*, ITexture*);
		(*(original_fn**)this)[5](this, pTexture);
	}

	void SetIntRenderingParameter(int a1, int a2)
	{
		using original_fn = void(__thiscall*)(void*, int, int);
		(*(original_fn**)this)[126](this, a1, a2);
	}

	void SetRenderTarget(ITexture* pTexture)
	{
		using original_fn = void(__thiscall*)(void*, ITexture*);
		(*(original_fn**)this)[6](this, pTexture);
	}

	ITexture* GetRenderTarget()
	{
		using original_fn = ITexture*(__thiscall*)(void*);
		return (*(original_fn**)this)[7](this);
	}

	void DepthRange(float zNear, float zFar) {
		using original_fn = void* (__thiscall*)(void*, float, float);
		(*(original_fn**)this)[11](this, zNear, zFar);
	}

	void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false)
	{
		using original_fn = void(__thiscall*)(void*, bool, bool, bool);
		(*(original_fn**)this)[12](this, bClearColor, bClearDepth, bClearStencil);
	}

	void SetLights(int nCount, const LightDesc_t* pLights)
	{
		using original_fn = void(__thiscall*)(void*, int, const LightDesc_t*);
		(*(original_fn**)this)[17](this, nCount, pLights);
	}

	void SetAmbientLightCube(Vec4D vecCube[6])
	{
		using original_fn = void(__thiscall*)(void*, Vec4D[6]);
		(*(original_fn**)this)[18](this, vecCube);
	}

	void Viewport(int x, int y, int iWidth, int iHeight)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int);
		(*(original_fn**)this)[40](this, x, y, iWidth, iHeight);
	}

	void GetViewport(int* x, int* y, int* iWidth, int* iHeight)
	{
		using original_fn = void(__thiscall*)(void*, int*, int*, int*, int*);
		(*(original_fn**)this)[41](this, x, y, iWidth, iHeight);
	}

	void ClearColor3ub(unsigned char r, unsigned char g, unsigned char b)
	{
		using original_fn = void(__thiscall*)(void*, unsigned char, unsigned char, unsigned char);
		(*(original_fn**)this)[78](this, r, g, b);
	}

	void ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		using original_fn = void(__thiscall*)(void*, unsigned char, unsigned char, unsigned char, unsigned char);
		(*(original_fn**)this)[79](this, r, g, b, a);
	}

	void DrawScreenSpaceRectangle(IMaterial* pMaterial, int iDestX, int iDestY, int iWidth, int iHeight, float flTextureX0, float flTextureY0, float flTextureX1, float flTextureY1, int iTextureWidth, int iTextureHeight, void* pClientRenderable = nullptr, int nXDice = 1, int nYDice = 1)
	{
		using original_fn = void(__thiscall*)(void*, IMaterial*, int, int, int, int, float, float, float, float, int, int, void*, int, int);
		(*(original_fn**)this)[114](this, pMaterial, iDestX, iDestY, iWidth, iHeight, flTextureX0, flTextureY0, flTextureX1, flTextureY1, iTextureWidth, iTextureHeight, pClientRenderable, nXDice, nYDice);
	}

	void PushRenderTargetAndViewport()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[119](this);
	}

	void PopRenderTargetAndViewport()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[120](this);
	}

	void SetStencilState(const ShaderStencilState_t& state) {
		using original_fn = void(__thiscall*)(void*, const ShaderStencilState_t&);
		(*(original_fn**)this)[128](this, state);
	}

	void SetLightingOrigin(/*Vector vecLightingOrigin*/float x, float y, float z)
	{
		using original_fn = void(__thiscall*)(void*, float, float, float);
		(*(original_fn**)this)[158](this, x, y, z);
	}

	void SetScissorRect(const int nLeft, const int nTop, const int nRight, const int nBottom, const bool bEnableScissor = true)
	{
		using original_fn = void(__thiscall*)(void*, int, int, int, int, const bool);
		(*(original_fn**)this)[159](this, nLeft, nTop, nRight, nBottom, bEnableScissor);
	}

	void PopScissorRect()
	{
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[160](this);
	}
};

template <class T>
class CBaseAutoPtr
{
public:
	CBaseAutoPtr() : m_pObject(0) {}
	CBaseAutoPtr(T* pFrom) : m_pObject(pFrom) {}

	operator const void* () const { return m_pObject; }
	operator void* () { return m_pObject; }

	operator const T* () const { return m_pObject; }
	operator const T* () { return m_pObject; }
	operator T* () { return m_pObject; }

	int			operator=(int i) { m_pObject = 0; return 0; }
	T* operator=(T* p) { m_pObject = p; return p; }

	bool        operator !() const { return (!m_pObject); }
	bool        operator!=(int i) const { return (m_pObject != NULL); }
	bool		operator==(const void* p) const { return (m_pObject == p); }
	bool		operator!=(const void* p) const { return (m_pObject != p); }
	bool		operator==(T* p) const { return operator==((void*)p); }
	bool		operator!=(T* p) const { return operator!=((void*)p); }
	bool		operator==(const CBaseAutoPtr<T>& p) const { return operator==((const void*)p); }
	bool		operator!=(const CBaseAutoPtr<T>& p) const { return operator!=((const void*)p); }

	T* operator->() { return m_pObject; }
	T& operator *() { return *m_pObject; }
	T** operator &() { return &m_pObject; }

	const T* operator->() const { return m_pObject; }
	const T& operator *() const { return *m_pObject; }
	T* const* operator &() const { return &m_pObject; }

protected:
	CBaseAutoPtr(const CBaseAutoPtr<T>& from) : m_pObject(from.m_pObject) {}
	void operator=(const CBaseAutoPtr<T>& from) { m_pObject = from.m_pObject; }

	T* m_pObject;
};

//---------------------------------------------------------

template <class T>
class CRefPtr : public CBaseAutoPtr<T>
{
	typedef CBaseAutoPtr<T> BaseClass;
public:
	CRefPtr() {}
	CRefPtr(T* pInit) : BaseClass(pInit) {}
	CRefPtr(const CRefPtr<T>& from) : BaseClass(from) {}
	~CRefPtr() { if (BaseClass::m_pObject) BaseClass::m_pObject->Release(); }

	void operator=(const CRefPtr<T>& from) { BaseClass::operator=(from); }

	int operator=(int i) { return BaseClass::operator=(i); }
	T* operator=(T* p) { return BaseClass::operator=(p); }

	operator bool() const { return !BaseClass::operator!(); }
	operator bool() { return !BaseClass::operator!(); }

	void SafeRelease() { if (BaseClass::m_pObject) BaseClass::m_pObject->Release(); BaseClass::m_pObject = 0; }
	void AssignAddRef(T* pFrom) { SafeRelease(); if (pFrom) pFrom->AddRef(); BaseClass::m_pObject = pFrom; }
	void AddRefAssignTo(T*& pTo) { ::SafeRelease(pTo); if (BaseClass::m_pObject) BaseClass::m_pObject->AddRef(); pTo = BaseClass::m_pObject; }
};


class CMatRenderContextPtr : public CRefPtr<IMatRenderContext>
{
	typedef CRefPtr<IMatRenderContext> CBaseClass;
public:
	CMatRenderContextPtr() = default;

	CMatRenderContextPtr(IMatRenderContext* pInit) : CBaseClass(pInit)
	{
		if (CBaseClass::m_pObject != nullptr)
			CBaseClass::m_pObject->BeginRender();
	}

	CMatRenderContextPtr(IMaterialSystem* pFrom) : CBaseClass(pFrom->GetRenderContext())
	{
		if (CBaseClass::m_pObject != nullptr)
			CBaseClass::m_pObject->BeginRender();
	}

	~CMatRenderContextPtr()
	{
		if (CBaseClass::m_pObject != nullptr)
			CBaseClass::m_pObject->EndRender();
	}

	IMatRenderContext* operator=(IMatRenderContext* pSecondContext)
	{
		if (pSecondContext != nullptr)
			pSecondContext->BeginRender();

		return CBaseClass::operator=(pSecondContext);
	}

	void SafeRelease()
	{
		if (CBaseClass::m_pObject != nullptr)
			CBaseClass::m_pObject->EndRender();

		CBaseClass::SafeRelease();
	}

	void AssignAddReference(IMatRenderContext* pFrom)
	{
		if (CBaseClass::m_pObject)
			CBaseClass::m_pObject->EndRender();

		CBaseClass::AssignAddRef(pFrom);
		CBaseClass::m_pObject->BeginRender();
	}

	void GetFrom(IMaterialSystem* pFrom)
	{
		AssignAddReference(pFrom->GetRenderContext());
	}

private:
	CMatRenderContextPtr(const CMatRenderContextPtr& pRefPtr);
	void operator=(const CMatRenderContextPtr& pSecondRefPtr);
};

class Texture_t {
public:
	char pad[0xC];
	LPDIRECT3DTEXTURE9 texture_ptr;
};

class ITexture {
private:
	template <typename T, typename ... args_t>
	constexpr T CallVFunc(void* thisptr, std::size_t nIndex, args_t... argList)
	{
		using VirtualFn = T(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<VirtualFn**>(thisptr))[nIndex](thisptr, argList...);
	}
private:
	std::byte	pad0[0x50];		 // 0x0000
public:
	Texture_t** pTextureHandles; // 0x0050

	int GetActualWidth()
	{
		return CallVFunc<int>(this, 3);
	}

	int GetActualHeight()
	{
		return CallVFunc<int>(this, 4);
	}

	void IncrementReferenceCount()
	{
		CallVFunc<void>(this, 10);
	}

	void DecrementReferenceCount()
	{
		CallVFunc<void>(this, 11);
	}
};