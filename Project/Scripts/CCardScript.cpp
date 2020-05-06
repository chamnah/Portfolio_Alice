#include "CCardScript.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CEventMgr.h"
#include "CSceneMgr.h"
#include "KeyMgr.h"
#include "CPlayerMgr.h"
#include "CText.h"

CCardScript::CCardScript()
	:m_pRedLine{}
, m_strPrice{L"10",L"20",L"30"}
, m_pPriceText(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCARDSCRIPT);
	IntInfo Info = IntInfo{ 0,L"m_iType" };
	m_vecInt.push_back(Info);
}

CCardScript::~CCardScript()
{
}

void CCardScript::Start()
{
	if (GetOwner()->GetChild().empty())
	{
		m_pPriceText = new CGameObj;
		m_pPriceText->AddComponent(new CText);
		GetOwner()->AddChild(m_pPriceText);
	}
	else
		m_pPriceText = GetOwner()->GetChild()[0];
}

int CCardScript::Update()
{
	if (!CPlayerMgr::GetInst()->GetMenu())
		return 0;
	if (!m_bColl)
	{
		if (m_vecInt[0].iVal != (UINT)CPlayerMgr::GetInst()->GetWeaponType()
			&& m_pRedLine[0] != nullptr)
		{
			for (size_t i = 0; i < 4; i++)
			{
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pRedLine[i],NULL });
				m_pRedLine[i] = nullptr;
			}
		}
	}
	return 0;
}

void CCardScript::CollisionEnter(CCollider * _pColl)
{
	m_bColl = true;
	if (m_pRedLine[0] != nullptr)
		return;

	for (size_t i = 0; i < 4; i++)
	{
		m_pRedLine[i] = CResMgr::GetInst()->Load<CPrefab>(L"RedLine")->Instantiate();
		GetOwner()->AddChild(m_pRedLine[i]);
		m_pRedLine[i]->SetLayerName(L"Menu");
		m_pRedLine[i]->SetID(CSceneMgr::GetInst()->GetLayerIndex(L"Menu"));
	}
	
	m_pRedLine[0]->GetTransForm()->SetPos(Vec3(-70.f,-10.f,0.f));
	m_pRedLine[1]->GetTransForm()->SetPos(Vec3(70.f, -10.f, 0.f));
	m_pRedLine[2]->GetTransForm()->SetPos(Vec3(6.f, 110.f, 0.f));
	m_pRedLine[2]->GetTransForm()->SetScale(Vec3(16.f, 140.f, 1.f));
	m_pRedLine[2]->GetTransForm()->SetRotation(Vec3(0.f, 0.f, XM_PIDIV2));

	m_pRedLine[3]->GetTransForm()->SetPos(Vec3(6.f, -110.f, 0.f));
	m_pRedLine[3]->GetTransForm()->SetScale(Vec3(16.f, 140.f, 1.f));
	m_pRedLine[3]->GetTransForm()->SetRotation(Vec3(0.f, 0.f, XM_PIDIV2));

	for (size_t i = 0; i < 4; i++)
	{
		CPlayerMgr::GetInst()->UIPos(m_pRedLine[i]);
	}
	
}

void CCardScript::Collision(CCollider * _pColl)
{
	if (KEYTAB(KEY_TYPE::KEY_LBTN))
	{
		CPlayerMgr::GetInst()->SetWeaponType((WEAPON_TYPE)m_vecInt[0].iVal);
		UINT iIdx = CPlayerMgr::GetInst()->GetWeaponUp((WEAPON_TYPE)m_vecInt[0].iVal);
		CPlayerMgr::GetInst()->SetPrice(m_strPrice[iIdx]);
	}
}

void CCardScript::CollisionExit(CCollider * _pColl)
{
	m_bColl = false;
	if (m_vecInt[0].iVal == (UINT)CPlayerMgr::GetInst()->GetWeaponType())
		return;
	for (size_t i = 0; i < 4; i++)
	{
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pRedLine[i],NULL });
		m_pRedLine[i] = nullptr;
	}
}

void CCardScript::Save(FILE * _pFile)
{
	int iCount = m_vecInt.size();
	fwrite(&iCount, sizeof(int), 1, _pFile);
	for (size_t i = 0; i < m_vecInt.size(); i++)
	{
		fwrite(&m_vecInt[i].iVal,sizeof(int),1,_pFile );
		SaveWString(m_vecInt[i].strName,_pFile);
	}
}
void CCardScript::Load(FILE * _pFile)
{
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, _pFile);
	for (size_t i = 0; i < iCount; i++)
	{
		fread(&m_vecInt[i].iVal, sizeof(int), 1, _pFile);
		m_vecInt[i].strName = LoadWString(_pFile);
	}
}