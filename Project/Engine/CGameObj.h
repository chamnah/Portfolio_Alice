#pragma once
#include "CParent.h"
#include "global.h"

template<typename T1>
struct ID
{
	typedef T1 type;
};

class CComponent;
class CMeshRender;
class CTransForm;
class CScript;
class CTexture;
class CCamera;
class CCollider;
class CAnimator2D;
class CGrid;
class CTile;
class CIncident;
class CLight;
class CFilterCom;
class CTerrain;
class CAnimator3D;
class CText;
class CGameObj :
	public CParent
{
private:
	CComponent* m_arrComponent[(UINT)COMPONENT_TYPE::END];
	vector<CScript*> m_vecScript;
	vector<CGameObj*> m_vecChild;
	int     m_iLayerID;
	CGameObj* m_pParent;
	bool    m_bDead;
	bool    m_bColl;
	wstring m_wcsLayerName;
	static int iValue;
	bool   m_bFrustum;
	bool   m_bShow;
	TEAM_TYPE m_eTeamType;

public:
	void AddComponent(CComponent* _pComponent);
	bool AddComponent(CComponent* _pComponent, COMPONENT_TYPE _eType);
	void SetComponent(CComponent * _pComponent, COMPONENT_TYPE _eType);
	void DelComponent(COMPONENT_TYPE _eType);
	void AddChild(CGameObj* _pObj);
	void DeleteAllChild();
	void DisconnectParent();
	void ChangeLayer(UINT _iID);
	bool IsFamily(CGameObj* _pObj);
	void LayerEscape();
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);
	CGameObj* GetAncestor();

	CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrComponent[(UINT)_eType];}
	CMeshRender* GetMeshRender() { return (CMeshRender*)m_arrComponent[(UINT)COMPONENT_TYPE::MESH_RENDER]; }
	CTransForm* GetTransForm() { return (CTransForm*)m_arrComponent[(UINT)COMPONENT_TYPE::TRANSFORM]; }
	CCamera* GetCamera() { return (CCamera*)m_arrComponent[(UINT)COMPONENT_TYPE::CAMERA]; }
	CCollider* GetCollider() { return (CCollider*)m_arrComponent[(UINT)COMPONENT_TYPE::COLLIDER]; }
	CGrid* GetGrid() { return (CGrid*)m_arrComponent[(UINT)COMPONENT_TYPE::GRID]; }
	CIncident* GetIncident() { return (CIncident*)m_arrComponent[(UINT)COMPONENT_TYPE::INCIDENT]; }
	CAnimator2D* GetAnimator2D() { return (CAnimator2D*)m_arrComponent[(UINT)COMPONENT_TYPE::ANIMATION_2D]; }
	CTile* GetTile() { return (CTile*)m_arrComponent[(UINT)COMPONENT_TYPE::TILE];}
	CLight* GetLight() { return (CLight*)m_arrComponent[(UINT)COMPONENT_TYPE::LIGHT]; }
	CFilterCom* GetFilter() { return (CFilterCom*)m_arrComponent[(UINT)COMPONENT_TYPE::FILTER]; }
	CTerrain* GetTerrain() { return (CTerrain*)m_arrComponent[(UINT)COMPONENT_TYPE::TERRAIN];}
	CAnimator3D* GetAnimator3D() { return (CAnimator3D*)m_arrComponent[(UINT)COMPONENT_TYPE::ANIMATION_3D]; }
	CText* GetText() { return (CText*)m_arrComponent[(UINT)COMPONENT_TYPE::TEXT]; }

	void SetValue(int _iValue) { iValue = _iValue; }
	int*  GetValue() { return &iValue; }

	void SetComponent(COMPONENT_TYPE _eType, CComponent* _pCom) { m_arrComponent[(UINT)_eType] = _pCom; }

	vector<CScript*>& GetScript() { return m_vecScript; }
	void SetParent(CGameObj* _pObj) { m_pParent = _pObj; }
	CGameObj* GetParent() { return m_pParent; }
	vector<CGameObj*>& GetChild() {	return m_vecChild;}
	CGameObj* GetChild(wstring _wcsName);
	void SetShow(bool _bShow) {	m_bShow = _bShow;}
	bool GetShow() { return m_bShow; }
public:
	void SetID(int _iID) { m_iLayerID = _iID; }
	int  GetID() { return m_iLayerID; }
	void SetLayerName(wstring _wcsName) { m_wcsLayerName = _wcsName; }
	wstring  GetLayerName() { return m_wcsLayerName; }
	void SetFrustum(bool _bFrustum) { m_bFrustum = _bFrustum; }
	bool GetFrustum() { return m_bFrustum; }
	void SetTeamType(TEAM_TYPE _eType) { m_eTeamType = _eType; }
	TEAM_TYPE GetTeamType() { return m_eTeamType; }
public:
	CLONE(CGameObj)

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	void IsDead();
	bool GetDead() { return m_bDead; }
	void SetColl(bool _bColl);
	bool GetColl() { return m_bColl; }

public:
	CGameObj();
	CGameObj(const CGameObj& _pObj);
	virtual ~CGameObj();
};