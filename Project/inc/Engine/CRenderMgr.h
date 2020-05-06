#pragma once
#include "global.h"

class CBlendState;
class CDepthStencilState;
class CRT;
class CMRT;
class CCamera;
class CLight;
class CGameObj;
class CTexture;
class CRenderMgr
{
	SINGLETON(CRenderMgr);

private:
	CBlendState*  m_arrBlend[(UINT)BLEND_TYPE::END];
	CDepthStencilState* m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::END];
	ID3D11RasterizerState* m_arrRasterize[(UINT)RASTERIZE_TYPE::END];
	CRT*          m_pRT[(UINT)RT_TYPE::END];
	CMRT*         m_pMRT[(UINT)MRT_TYPE::END];
	UINT          m_iWidth;
	UINT          m_iHeight;
	HWND		  m_hWnd;
	IDXGISwapChain* m_pSwap;
	vector<CCamera*> m_vecCam;
	vector<CLight*>   m_vecLight;
	CGameObj*     m_pMergeFilter;
	CGameObj*     m_pVisionFilter;
	map<wstring, CGameObj*> m_mapFilter;
	CCamera*      m_pMainCam;
	vector<wstring>       m_vecRSName;
	vector<wstring>       m_vecSPName;
	vector<CTexture*> m_vecRenderTex;
	bool          m_bCollView;
	ID2D1RenderTarget* m_pTextRT;
	ID2D1Factory* m_pFactory;
	ID3D11SamplerState* m_arrSampler[(UINT)SAMPLER_TYPE::END];
	bool m_bText;

public:
	void Init(UINT _iWidth, UINT _iHeight, HWND _hwnd);
	void FinalUpdate();
	void Render();

	void CreateSwap();
	void CreateMultiRenderTarget();
	void CreateBlend();
	void CreateDepthStencil();
	void CreateFilter();
	void CreateViewPort();
	void CreateRasterize();
	void CreateSampler();

	CBlendState* GetBlend(BLEND_TYPE _eType) { return m_arrBlend[(UINT)_eType]; }
	CBlendState* GetBlend(wstring& _wcsName);
	CDepthStencilState* GetDepthStencil(DEPTH_STENCIL_TYPE _eType) { return m_arrDepthStencil[(UINT)_eType]; }
	CDepthStencilState* GetDepthStencil(const wstring& _wcsName);
	wstring GetBlendName(CBlendState* _pState);
	wstring GetDepthStencilName(CDepthStencilState* _pState);
	IDXGISwapChain* GetSwapChain() { return m_pSwap; }
	ID3D11RasterizerState* GetRasterizeState(RASTERIZE_TYPE _eType) { return m_arrRasterize[(UINT)_eType]; }
	void RSSetState(RASTERIZE_TYPE _eType);
	void SPSetState(SAMPLER_TYPE _eType);

	CMRT* GetMRT(MRT_TYPE _eType) { return m_pMRT[(UINT)_eType]; }

	void Present() { m_pSwap->Present(0, 0); }
	void Clear();

	void RegisterCamera(CCamera* _pCam);
	void RegisterLight(CLight* _Light) { m_vecLight.push_back(_Light); };

	CGameObj* FindFilter(const wstring& _wcsKey);

	CGameObj* GetMergeFilter() { return m_pMergeFilter; }
	void SetMergeFilterParam(void* _pData, SHADER_PARAM _eParam);
	CCamera* GetMainCam() { return m_pMainCam; }

	wstring GetRSName(RASTERIZE_TYPE _eType) { return m_vecRSName[(UINT)_eType]; }
	RASTERIZE_TYPE GetRSState(wstring& _strName);

	wstring GetSPName(SAMPLER_TYPE _eType) { return m_vecSPName[(UINT)_eType]; }
	SAMPLER_TYPE GetSPState(wstring& _strName);

	HWND GetGameHwnd() { return m_hWnd; }
	void SetCollView(bool _bView) { m_bCollView = _bView; }

	CTexture* GetRenderTargetTexture(RT_TYPE _eType);
	HRESULT	D2DInit(ID2D1Factory **factory, ID2D1RenderTarget **Rt);
	ID2D1RenderTarget* GetTextRT() { return m_pTextRT; }
	void SetText(bool _bText) { m_bText = _bText; }
};