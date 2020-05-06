#include "CJumpPadScript.h"
#include "CScriptMgr.h"
#include "CAnimator3D.h"
#include "CCollider.h"
#include "CPlayerMgr.h"

CJumpPadScript::CJumpPadScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CJUMPPADSCRIPT);
}

CJumpPadScript::~CJumpPadScript()
{
}

void CJumpPadScript::Start()
{
}

int CJumpPadScript::Update()
{
	if (Animator3D() == nullptr)
		return 0;
	if (Animator3D()->GetCurAnimKey(L"ready") && Animator3D()->GetFinish())
		Animator3D()->PlayAnim(L"launch", false);
	return 0;
}

void CJumpPadScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Player")
	{
		Animator3D()->PlayAnim(L"ready",false);
		CPlayerMgr::GetInst()->SetJumpPad(true);
		//_pColl->GetOwner()->GetTransForm();
	}
}