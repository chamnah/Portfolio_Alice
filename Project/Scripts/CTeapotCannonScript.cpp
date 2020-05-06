#include "CTeapotCannonScript.h"
#include "CScriptMgr.h"
#include "CAnimator3D.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CCannonBallScript.h"

CTeapotCannonScript::CTeapotCannonScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CTEAPOTCANNONSCRIPT);
}

CTeapotCannonScript::~CTeapotCannonScript()
{
}

int CTeapotCannonScript::Update()
{
	if (Animator3D() == nullptr)
		return 0;

	int iFrm = Animator3D()->GetCurFrm();
	if (Animator3D()->GetCurAnimKey(L"fire"))
	{
		if (iFrm >= 15 && iFrm >= 20)
		{
			CResMgr::GetInst()->SoundPlay(L"sfx_teacannon_fire01", 1);
			Animator3D()->PlayAnim(L"Idle");
			CGameObj* pBullet = CResMgr::GetInst()->Load<CPrefab>(L"CannonBallLv01")->CreateObjClone(L"Default");
			pBullet->GetTransForm()->SetPos(TransForm()->GetWorldPos());
			CCannonBallScript* pScript = new CCannonBallScript;
			pBullet->AddComponent(pScript);
			//pBullet->GetTransForm()->SetMtxOwner(GetOwner()->GetTransForm()->GetWorldMtx());
			pScript->SetDir(TransForm()->GetWorldDir(DIR_TYPE::UP));
		}
	}

	return 0;
}