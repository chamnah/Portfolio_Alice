#include "CIncident.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGameObj.h"
#include "CGrid.h"
#include "CTile.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "CAnimator2D.h"
#include "CCore.h"
#include "TimeMgr.h"
#include "CPlayerMgr.h"
#include "CCollider.h"
#include "CRenderMgr.h"
#include "CCamera.h"

int CIncident::Update()
{
	if (CPlayerMgr::GetInst()->GetDoor())
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Monster");
		vector<CGameObj*>& vecObj = pLayer->GetGameObj();
		if (vecObj.empty())
		{
			CPlayerMgr::GetInst()->SetEvent(false);
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
			if(m_strScene != L"")
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CHANGE_SCENE,(INT_PTR)m_strScene.c_str(),NULL });
		}
	}

	if (m_bDelete)
	{
		m_fDeathTime += DT;
		if (!m_bHide && m_fDeathTime > 5.f)
		{
			m_bHide = true;
			m_pResult->GetAnimator2D()->PlayAnim(L"ResultOut", false);
			CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pScore,0});
			m_pScore = nullptr;
			CCore::GetInst()->ClearText();
		}

		if (m_bHide && m_pResult->GetAnimator2D()->GetAnimFinish())
		{
			m_bDelete = false;
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 });
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pResult,0 });
			m_pResult = nullptr;
		}

	}

	if (!m_bIncident)
		return 0;

	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Monster");
	vector<CGameObj*>& vecObj = pLayer->GetGameObj();

	if (!vecObj.empty())
		return 0;

	m_bDelete = true;

	m_pResult = CResMgr::GetInst()->PrefabToCreateObj(L"UI", L"ResultIn");
	m_pResult->GetAnimator2D()->PlayAnim(L"ResultIn", false);
	m_pResult->GetTransForm()->SetPos(Vec3(0.f,0.f,100.f));

	m_pScore = CResMgr::GetInst()->PrefabToCreateObj(L"UI", L"SuppressorIcon");
	m_pScore->GetTransForm()->SetPos(Vec3(-840,0,0));

	if (m_iMonsterCount != 0)
	{
		wchar_t szCount[10];
		_itow_s(m_iMonsterCount, szCount, 10);

		wstring wcsCount = L"x ";
		wcsCount += szCount;
		CCore::GetInst()->AddText(TextInfo{ wcsCount,Vec2{200,500},Vec2{300,600} });
	}

	m_bIncident = false;
	return 0;
}

void CIncident::AddObj(CGameObj * _pObj)
{
	m_vecObj.push_back(EventObj(_pObj->GetTransForm()->GetPos(), _pObj->GetTransForm()->GetRotation(),_pObj->GetName()));
}

CIncident::CIncident()
	:m_bIncident(false)
	, m_iMonsterCount(0)
	, m_bDelete(false)
	, m_fDeathTime(0.f)
	, m_pResult(nullptr)
	, m_pScore(nullptr)
	, m_bHide(false)
	, m_strScene(L"")
{
	m_eType = COMPONENT_TYPE::INCIDENT;
}

CIncident::~CIncident()
{
}

void CIncident::Save(FILE * _pFile)
{
	UINT iSize = (UINT)m_vecObj.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);
	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		SaveWString(m_vecObj[i].wcsName, _pFile);
		fwrite(&m_vecObj[i].vPos,sizeof(Vec3),1,_pFile);
		fwrite(&m_vecObj[i].vRot,sizeof(Vec3),1,_pFile);
	}
	SaveWString(m_strScene, _pFile);
}

void CIncident::Load(FILE * _pFile)
{
	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	EventObj eObj;
	for (size_t i = 0; i < iSize; i++)
	{
		eObj.wcsName = LoadWString(_pFile);
		fread(&eObj.vPos, sizeof(Vec3), 1, _pFile);
		fread(&eObj.vRot, sizeof(Vec3), 1, _pFile);
		m_vecObj.push_back(eObj);
	}
		m_strScene = LoadWString(_pFile);
}

void CIncident::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetID() != m_iPlayerID)
		return;
	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		CGameObj* pMonster = CResMgr::GetInst()->Load<CPrefab>(m_vecObj[i].wcsName)->CreateObjClone(L"Monster");
		pMonster->GetTransForm()->SetPos(m_vecObj[i].vPos);
		pMonster->GetTransForm()->SetRotation(m_vecObj[i].vRot);
	}
	CPlayerMgr::GetInst()->SetEvent(true);
	GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER);
}

void CIncident::Collision(CCollider * _pColl)
{
}

void CIncident::CollisionExit(CCollider* _pColl)
{
}