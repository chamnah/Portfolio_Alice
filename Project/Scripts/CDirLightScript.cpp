#include "CDirLightScript.h"
#include "CScriptMgr.h"
#include "CPlayerMgr.h"
#include "CTransForm.h"

CDirLightScript::CDirLightScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CDIRLIGHTSCRIPT);
}

CDirLightScript::~CDirLightScript()
{
}

int CDirLightScript::Update()
{
	CGameObj* pPlayer = CPlayerMgr::GetInst()->GetPlayer();
	if (pPlayer)
	{
		TransForm()->SetAddPos(pPlayer->GetTransForm()->GetPos());
	}

	return 0;
}