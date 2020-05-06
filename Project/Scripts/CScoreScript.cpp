#include "CScoreScript.h"
#include "CPlayerMgr.h"
#include "CText.h"
#include "CScriptMgr.h"
CScoreScript::CScoreScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CSCORESCRIPT);
}

CScoreScript::~CScoreScript()
{
}

void CScoreScript::Start()
{
}

int CScoreScript::Update()
{
	if (Text() == nullptr)
		return 0;
	wchar_t szTooth[10];
	_itow_s(CPlayerMgr::GetInst()->GetTooth(), szTooth, 10);
	Text()->SetText(szTooth);

	return 0;
}