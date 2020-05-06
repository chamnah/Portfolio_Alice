#pragma once
#include "CComponent.h"
	
union uInstID
{	
	struct
	{
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	LONGLONG llID;
};	
	
struct tInstObj
{	
	CGameObj* pObj;
	UINT      iMtrlIdx;
};	
	
struct cmpView {
	bool operator()(tInstObj& t, tInstObj& u);
};	
	
class CLayer;
class CFrustum;
class CCamera :
	public CComponent
{	
private:
	Mtx    m_mtxView;
	Mtx    m_mtxProj;
	Mtx    m_mtxInvView;
	bool   m_bPerspective;
	float  m_fFOV;
	float  m_fNear;
	float  m_fFar;
	float  m_fScale;
	UINT m_iLayerMask;
	CFrustum* m_pFrustum;
	vector<CGameObj*> m_vecDeferred;
	vector<CGameObj*> m_vecFilter;
	vector<CGameObj*> m_vecForward;
	vector<CGameObj*> m_vecShadow;
	vector<CCollider*> m_vecColl;
	vector<CGameObj*> m_vecText;
	bool   m_bRegister;
	bool   m_bMainCam;
	tRay   m_Ray;
	bool   m_bRender;
	bool bInst;
	map<LONGLONG, vector<tInstObj>> m_mapInstGroupD;
	map<LONGLONG, vector<tInstObj>> m_mapInstGroupF;
	map<LONGLONG, vector<tInstObj>> m_mapInstGroupP;
	map<LONGLONG, vector<tInstObj>> m_mapInstGroupS;
	map<wstring, vector<tInstObj>> m_mapInstSingle;

public:
	void Init();
	int Update();
	virtual int LateUpdate();
	int FinalUpdate();
	void Render(CLayer** _pLayer);
	void DeferredRender();
	void ForwardRender();
	void PostEffectRender();
	void ShadowRender();
	void PointShadowRender();
	void CollRender();
	void TextRender();
	void UpdateRender();

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	void SetPerspective(bool _bPers) { m_bPerspective = _bPers; }
	float GetScale() { return m_fScale; }

	bool IsPerspective() { return m_bPerspective; }

	void CheckLayerMask(UINT _iMask);
	void LayerMaskClear() { m_iLayerMask = 0; }
	void AllCheckLayerMask() { m_iLayerMask = 0xffff; }
	UINT GetLayerMask() { return m_iLayerMask; }

	float GetFar() {return m_fFar;}
	float GetFOV() { return m_fFOV; }
	void SetFar(float _fFar) { m_fFar = _fFar; }
	void SetFOV(float _fFOV) { m_fFOV = _fFOV; }
	void SetScale(float _fScale) { m_fScale = _fScale; }

	Mtx GetMtxView() { return m_mtxView; }
	Mtx GetMtxProj() { return m_mtxProj; }
	Mtx GetMtxInvView() { return m_mtxInvView; }
	void SetMainCam(bool _bMainCam) { m_bMainCam = _bMainCam; }
	bool GetMainCam() { return m_bMainCam; }

	tRay& GetRay() { return m_Ray; }
	
	vector<CCollider*>& GetAllColl() { return m_vecColl; }

	void SetRender(bool _bRender) { m_bRender = _bRender; }

	void SortGameObject();
	void SortShadowObject();

	void SetRegister(bool _bRegister) { m_bRegister = _bRegister; }

	void CalcView();
	void CalcRay();

	CLONE(CCamera);

	void RenderClear();

public:
	CCamera();
	~CCamera();
};	