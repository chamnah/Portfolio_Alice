#pragma once
#include "CComponent.h"
class CMesh;
class CMaterial;
class CTransForm;
class CFilterCom :
	public CComponent
{
private:
	CMesh*    m_pVolumeMesh;
	CMesh*    m_pMesh;
	CMaterial* m_pMtrl;
	FILTER_TYPE m_eFilter;
	CMaterial* m_pVerBlur;
	CMaterial* m_pHorBlur;
	CTexture* m_pRWTex;
	CTexture* m_pBlurOut;
	CTexture* m_pIn;

public:
	CLONE(CFilterCom);

	virtual void Start();
	void Render();
	void BlurRender();

	void SetVolumMesh(CMesh* _pMesh) { m_pVolumeMesh = _pMesh; }
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }
	CMaterial* GetMaterial() { return m_pMtrl; }
	void SetTransForm(CTransForm* _pTransForm);
	void SetFilterType(FILTER_TYPE _eType) { m_eFilter = _eType; }
	void SetOutTexture(CTexture* _pTex) { m_pIn = _pTex; }
	CTexture* GetBlurOutTexture() { return m_pBlurOut; }
public:
	CFilterCom();
	~CFilterCom();
};