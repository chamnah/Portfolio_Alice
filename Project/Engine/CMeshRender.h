#pragma once
#include "CComponent.h"

class CMesh;
class CMaterial;
class CMeshRender :
	public CComponent
{
public:
	CMesh* m_pMesh;
	CMaterial* m_pMtrl;
	vector<CMaterial*> m_vecMtrl;
	CMaterial* m_pSubMtrl;
	bool m_bView;
	RENDER_TYPE  m_eRenderType;
	bool m_bParticle;
	int  m_iParticleCount;
	bool   m_bShadow;
	Vec2 m_vUV;

public:
	void SetMesh(CMesh* _pMesh);// { m_pMesh = _pMesh; }
	//void SetMtrl(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }
	void SetMtrl(CMaterial* _pMtrl, int _iSubset = 0);

	void SetSubMtrl(CMaterial* _pMtrl);
	void SetMtrlClone(CMaterial* _pMtrl, int _iSubset);
	CMaterial* GetMtrl(int _iSubset = 0) { return m_vecMtrl[_iSubset]; }
	vector<CMaterial*>& GetVecMtrl() { return m_vecMtrl; }
	CMesh* GetMesh() { return m_pMesh; }
	void SetTexture(CTexture* pTex, SHADER_PARAM _Param = SHADER_PARAM::TEX_0, int _iSubset = 0);
	void SetParam(void* pValue, SHADER_PARAM _Param, int _iSubset = 0);
	void SetView(bool _bView) { m_bView = _bView; }
	void SetRenderType(RENDER_TYPE _eType) { m_eRenderType = _eType; }
	RENDER_TYPE GetRenderType() { return m_eRenderType; }
	void SetIsParticle(bool _bParticle) { m_bParticle = _bParticle; }
	bool GetIsParticle() { return m_bParticle; }
	void SetParticleCount(int _iParticleCount) { m_iParticleCount = _iParticleCount; }
	int GetParticleCount() { return m_iParticleCount; }
	void SetShadow(bool _bShadow) { m_bShadow = _bShadow; }
	bool GetShadow() { return m_bShadow; }
	LONGLONG GetInstID(int _iMtrlIdx);
	void SetUV(const Vec2& _vUV) { m_vUV = _vUV; }
	Vec2& GetUV() { return m_vUV; }
	CLONE(CMeshRender)

public:
	void Render();
	void Render(int _iIndex);
	void ShadowRender();
	void PointShadowRender();
	void ParticleRender();
public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CMeshRender();
	CMeshRender(const CMeshRender& _Other);
	virtual ~CMeshRender();
};