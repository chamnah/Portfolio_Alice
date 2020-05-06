#include "CItemScript.h"
#include "CPlayerMgr.h"
#include "CEventMgr.h"
#include "CCollider.h"
#include "CGameObj.h"
#include "CScriptMgr.h"

CItemScript::CItemScript()
	:m_eItemType(ITEM_TYPE::SCORE)
{
	SetScriptType((UINT)SCRIPT_TYPE::CITEMSCRIPT);
}

CItemScript::~CItemScript()
{
}

int CItemScript::Update()
{

	return 0;
}

void CItemScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() != L"Player")
		return;
	if (GetOwner()->GetDead())
		return;
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	if(m_eItemType == ITEM_TYPE::SCORE)
		CPlayerMgr::GetInst()->AddTooth(10);
}