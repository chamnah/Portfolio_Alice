#include "CUpgradeTextScript.h"
#include "CScriptMgr.h"
#include "CCardScript.h"
#include "CPlayerMgr.h"
#include "CText.h"

CUpgradeTextScript::CUpgradeTextScript()
	:m_iUp(0)
	, m_eWType(WEAPON_TYPE::END)
	, bFirst(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CUPGRADETEXTSCRIPT);
}

CUpgradeTextScript::~CUpgradeTextScript()
{
}

int CUpgradeTextScript::Update()
{
	if (!bFirst)
	{
		if (GetParentOfOwner() != nullptr)
		{
			m_eWType = (WEAPON_TYPE)(GetParentOfOwner()->GetScript()[0]->GetVecInt()[0].iVal);
			bFirst = true;
		}
	}

	if (m_eWType != WEAPON_TYPE::END)
	{
		if(m_iUp != CPlayerMgr::GetInst()->GetWeaponUp(m_eWType))
		{
			m_iUp = CPlayerMgr::GetInst()->GetWeaponUp(m_eWType);
			wchar_t szInt[10];
			_itow_s(m_iUp, szInt,10);
			Text()->SetText(szInt);
		}
	}

	return 0;
}

void CUpgradeTextScript::Start()
{
	Text()->SetText(L"0");
}