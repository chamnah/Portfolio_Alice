#include "CMouseScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"

CMouseScript::CMouseScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CMOUSESCRIPT);
}

CMouseScript::~CMouseScript()
{
}

int CMouseScript::Update()
{
	Vec3 vPos = CKeyMgr::GetInst()->GetDXMousePos();
	TransForm()->SetAddPos(vPos);
	return 0;
}

void CMouseScript::CollisionEnter(CCollider * _pColl)
{
}

void CMouseScript::Collision(CCollider * _pColl)
{
}

void CMouseScript::CollisionExit(CCollider * _pColl)
{
}
