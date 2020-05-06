#include "CBuyScript.h"
#include "CPlayerMgr.h"
#include "CText.h"
#include "CScriptMgr.h"

CBuyScript::CBuyScript()
	:m_bColor(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CBUYSCRIPT);
}

CBuyScript::~CBuyScript()
{
}

void CBuyScript::Start()
{
	CText* pText = Text();
	if (pText == nullptr)
		return;
	int iTooth = CPlayerMgr::GetInst()->GetTooth();
	int iScore = _wtoi(pText->GetTextName().c_str());

	if (iTooth < iScore)
		pText->ChangeColor(Vec4(1.f, 0.f, 0.f, 1.f));
	else
		pText->ChangeColor(Vec4(1.f, 1.f, 1.f, 1.f));
}

int CBuyScript::Update()
{
	CText* pText = Text();
	if (pText == nullptr)
		return 0;
	int iTooth = CPlayerMgr::GetInst()->GetTooth();
	int iScore = _wtoi(pText->GetTextName().c_str());

	if (iTooth < iScore)
	{
		if (!m_bColor)
		{
			m_bColor = true;
			pText->ChangeColor(Vec4(1.f, 0.f, 0.f, 1.f));
		}
	}
	else
	{
		if (m_bColor)
		{
			m_bColor = false;
			pText->ChangeColor(Vec4(1.f, 1.f, 1.f, 1.f));
		}
	}
	return 0;
}