#pragma once
#include "CRes.h"
class CShader;
class CTexture;
class CBlendState;
class CDepthStencilState;
class CMaterial :
	public CRes
{
private:
	CShader*   m_pShader;
	CTexture*  m_arrTexture[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	CTexture*  m_arrRWTexture[(UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0];
	ShaderParam  m_Param;

	CBlendState*  m_pBlend;
	CDepthStencilState* m_pDepthStencil;
	UINT      m_iRef;
	RASTERIZE_TYPE m_eRSType;
	SAMPLER_TYPE m_eSPType;

	D3D11_PRIMITIVE_TOPOLOGY m_eTopol;
public:
	void UpdateData();
	void UpdateDataInstancing();
	void SetShader(CShader* _pShader) { m_pShader = _pShader; }
	CShader* GetShader() { return m_pShader; }
	void SetParam(void* _pData, SHADER_PARAM _eType);
	Vec4& GetDiff() { return m_Param.arrMtrl; }
	CMaterial* Clone();
	CTexture** GetArrTexture() { return m_arrTexture; }
	void SetBlend(CBlendState* _pBlend) { m_pBlend = _pBlend; }
	CBlendState* GetBlend() { return m_pBlend; }
	void SetDepthStencil(CDepthStencilState* _pDepthStencil) { m_pDepthStencil = _pDepthStencil; }
	CDepthStencilState* GetDepthStencil() { return m_pDepthStencil; }
	void SetRasterizeState(RASTERIZE_TYPE _eType) { m_eRSType = _eType; }
	RASTERIZE_TYPE GetRasterizeState() { return m_eRSType; }

	void SetSamplerState(SAMPLER_TYPE _eType) { m_eSPType = _eType; }
	SAMPLER_TYPE GetSamplerState() { return m_eSPType; }

	void Excute(UINT _iX, UINT _iY, UINT _iZ);

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopol) { m_eTopol = _eTopol; }
	void UAViewClear();

public:
	void SaveMeshData(FILE* _pFile);
	static CMaterial* LoadMeshData(FILE* _pFile);
public:
	CMaterial();
	~CMaterial();
};