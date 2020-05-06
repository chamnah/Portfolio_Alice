#include "CMonsterSpawnScript.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CCollider.h"
#include "CTransForm.h"

CMonsterSpawnScript::CMonsterSpawnScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CMONSTERSPAWNSCRIPT);
}

CMonsterSpawnScript::~CMonsterSpawnScript()
{
}

void CMonsterSpawnScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Player")
	{
		CGameObj* pObj = CResMgr::GetInst()->Load<CPrefab>(L"Grunt_Mini")->CreateObjClone(L"Default");
		pObj->GetTransForm()->SetPos(GetOwner()->GetTransForm()->GetPos());
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	}
}