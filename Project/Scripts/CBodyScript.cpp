#include "CBodyScript.h"
#include "CScriptMgr.h"
#include "CAnimator3D.h"
CBodyScript::CBodyScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CBODYSCRIPT);
}

CBodyScript::~CBodyScript()
{
}

int CBodyScript::Update()
{
	CGameObj* pParent = GetOwner()->GetParent();
	if (pParent == nullptr)
		return 0;

	tAnim3D* pAnim = pParent->GetAnimator3D()->GetCurAnim();
	Animator3D()->PlayAnim(pAnim->strKey, pParent->GetAnimator3D()->GetRepeat());

	return 0;
}