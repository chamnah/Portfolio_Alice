#pragma once
#include "CRes.h"
class CTexture :
	public CRes
{
private:
	ID3D11ShaderResourceView*   m_pResView;
	ID3D11RenderTargetView*     m_pRTView;
	ID3D11UnorderedAccessView*  m_pUAView;
	ID3D11DepthStencilView*     m_pDSView;
	ID3D11Texture2D*            m_pTex2D;
	ScratchImage			    m_image;
	D3D11_TEXTURE2D_DESC	    m_Desc;

public:
	static CTexture* CreateTexture(const wstring& _wcsPath,UINT _iFlag = D3D11_BIND_SHADER_RESOURCE, UINT _iFormat = 0);
	static CTexture* CreateTexture(ID3D11ShaderResourceView* _pRV);
	static CTexture* CreateTexture(UINT _iWidth,UINT _iHeight,UINT _iFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage = D3D11_USAGE_DEFAULT);
	static CTexture* CreateTexture(ID3D11Texture2D* _pTex2D);
	static CTexture* CreateTexture(vector<CTexture*>& _vecTex, UINT _iFlag = D3D11_BIND_SHADER_RESOURCE);
	static CTexture* CreateTexture(CTexture* _pTex);
	void SetRegisterAll(int _iIndex);
	void SetRWTexRegister(int _iIndex);
	static void RegisterClear(int _iIndex);
	static void RWTexRegisterClear(int _iIndex);
	void Resize(int _iWidth, int _iHeight);

	UINT GetWidth() { return m_Desc.Width; }
	UINT GetHeight() { return m_Desc.Height; }
	DXGI_FORMAT GetFormat() { return m_Desc.Format; }

	ID3D11ShaderResourceView* GetShaderResourceView() { return m_pResView; }
	ID3D11RenderTargetView* GetRenderTargetView() { return m_pRTView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pDSView; }

	ID3D11Texture2D* GetTexture2D() { return m_pTex2D; }
	void* GetPixel();

	void Save(FILE* _pFile);
	static void Load(FILE* _pFile);

	void Clear(Vec4 vColor);
	void SetData(void* _pData,UINT _iSize);

public:
	CTexture();
	virtual ~CTexture();
	//void operator=(const CTexture& _other) { *this = _other; }

};