#include "CPrefab.h"
#include "CGameObj.h"
#include "CEventMgr.h"
#include "CSceneMgr.h"

CPrefab::CPrefab(CGameObj* _Obj)
	:m_pObj(_Obj)
{
}

CPrefab::~CPrefab()
{
	SAFE_DELETE(m_pObj);
}

CGameObj* CPrefab::Instantiate()
{
	return m_pObj->Clone();
}

CGameObj* CPrefab::CreateObjClone(const wstring& _strLayer)
{
	CGameObj* pClone = m_pObj->Clone();
 	int iLayer = CSceneMgr::GetInst()->GetLayerIndex(_strLayer);
	CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::CREATE_OBJ,iLayer,(INT_PTR)pClone });
	return pClone;
}