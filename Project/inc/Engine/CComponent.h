#pragma once
#include "CParent.h"
#include "CGameObj.h"

class CComponent :
	public CParent
{
private:
	CGameObj* m_pOwnerObj;

protected:
	COMPONENT_TYPE m_eType;

public:
	void SetParentObj(CGameObj* pObj) { m_pOwnerObj = pObj; }
	virtual int Update() { return 0; }
	virtual int LateUpdate() { return 0; }
	virtual int FinalUpdate() { return 0; }
	virtual void Render() {}
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile) { fread(&m_eType, sizeof(UINT), 1, _pFile); }

	virtual COMPONENT_TYPE GetType();

public:
	virtual CComponent* Clone() = 0;

public:
	CTransForm* TransForm() { return m_pOwnerObj->GetTransForm(); }
	CMeshRender* MeshRender() { return m_pOwnerObj->GetMeshRender(); }
	CAnimator2D* Animator2D() { return m_pOwnerObj->GetAnimator2D(); }
	CAnimator3D* Animator3D() { return m_pOwnerObj->GetAnimator3D(); }
	CCollider* Collider() { return m_pOwnerObj->GetCollider(); }
	CCamera* Camera() { return m_pOwnerObj->GetCamera(); }
	CTile* Tile() { return m_pOwnerObj->GetTile(); }
	CGrid* Grid() { return m_pOwnerObj->GetGrid(); }
	CIncident* Incident() { return m_pOwnerObj->GetIncident(); }
	CFilterCom* Filter() { return m_pOwnerObj->GetFilter(); }
	CText* Text() { return m_pOwnerObj->GetText(); }
	const vector<CScript*>& Script() { return m_pOwnerObj->GetScript(); }
	int GetLayerID() { return m_pOwnerObj->GetID(); }
	CGameObj* GetOwner() { return m_pOwnerObj; }
	CGameObj* GetParentOfOwner() { return m_pOwnerObj->GetParent(); }

public:
	CComponent();
	CComponent(const CComponent& _Com);
	virtual ~CComponent();

	friend class CGameObj;
};