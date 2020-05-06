#include "CUIScript.h"
#include "CScriptMgr.h"
#include "CPlayerMgr.h"

CUIScript::CUIScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CUISCRIPT);
}

CUIScript::~CUIScript()
{
}

void CUIScript::Start()
{
	CPlayerMgr::GetInst()->UIPos(GetOwner());
}