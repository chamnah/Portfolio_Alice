#include "CPlayerMgr.h"
#include "CSceneMgr.h"
#include "KeyMgr.h"
#include "CCore.h"
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CGameObj.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CLayer.h"
#include "CText.h"
#include "CCollider.h"

CPlayerMgr::CPlayerMgr()
	:m_pPlayer(nullptr)
	,m_pPlayerScript(nullptr)
	,m_pHP_UI(nullptr)
	,m_iHP(64)
	, m_bUIReset(false)
	, m_bMenu(false)
	, m_pMenu(nullptr)
	, m_bEvent(false)
	, m_bDoor(false)
	, m_eWeapon(WEAPON_TYPE::END)
	, m_bTooth(true)
	, m_iWeaponUp{}
	, m_bSmall(false)
	, m_bJumpPad(false)
{
	m_vDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_vDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_vDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);
	m_vDir[(UINT)DIR_TYPE::FRONT_LEFT] = Vec3(-1.f, 0.f, 1.f);
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::Init()
{
	m_vOriSize.x;
}

void CPlayerMgr::Update()
{
	m_pPlayer = nullptr;
	if (CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &m_pPlayer))
	{
		for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(m_vDir[i], XMMatrixRotationY(m_vAnimRot.y));
			m_vWorldDir[i].Normalize();
		}
	}

	/*TextInfo Info = {};
	Info.vLeftTop.x = 70;
	Info.vLeftTop.y = 170;
	Info.vRightBottom.x = 150;
	Info.vRightBottom.y = 200;
	_itow_s(m_iTooth, m_strTooth, 10);
	Info.wcsText = m_strTooth;
	CCore::GetInst()->AddText(Info);*/

	if (KEYTAB(KEY_TYPE::KEY_F3))
	{
		if (!m_bMenu)
		{
			CGameObj* pMouse = nullptr;
			if (CSceneMgr::GetInst()->GetObjOfLayer(L"Menu", 0, &pMouse))
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)pMouse,NULL });
			CSceneMgr::GetInst()->SetStop(true);
			m_pMenu = CResMgr::GetInst()->Load<CPrefab>(L"Upgrade")->CreateObjClone(L"Menu");

			if (pMouse != nullptr)
			{
				int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Menu");
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,iLayer,(INT_PTR)pMouse->Clone() });
			}
			m_bMenu = true;
			UIPos(m_pMenu);
		}
		else
		{
			m_eWeapon = WEAPON_TYPE::END;
			m_bMenu = false;
			CSceneMgr::GetInst()->SetStop(false);
			m_pMenu->SetShow(false);
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pMenu,NULL });
		}
	}
	else if (KEYTAB(KEY_TYPE::KEY_F4))
	{
		CGameObj* pBlur = CRenderMgr::GetInst()->FindFilter(L"Blur")->Clone();
		pBlur->SetName(L"Blur");
		int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pBlur });
	}
	m_bDoor = m_bEvent;

	if (KEYTAB(KEY_TYPE::KEY_NUM9))
		m_bSmall = !m_bSmall;
}

void CPlayerMgr::UIPos(CGameObj * _pChild)
{
	float fOffset = 0.f;
	if (_pChild->GetText() != nullptr)
	{
		float fSize = _pChild->GetText()->GetSize() / ORI_SIZEX;
		_pChild->GetText()->SetSize(fSize * g_Global.fWidth);
		_pChild->GetText()->ChangeFontAndSize();
		fOffset = -5.f;
	}

	Vec3 vPos = _pChild->GetTransForm()->GetPos() / Vec3(ORI_SIZEX, ORI_SIZEY, 1.f);
	Vec3 vScale = _pChild->GetTransForm()->GetScale() / Vec3(ORI_SIZEX, ORI_SIZEY, 1.f);
	_pChild->GetTransForm()->SetPos(Vec3(vPos.x * g_Global.fWidth, (vPos.y * g_Global.fHeight) + fOffset, _pChild->GetTransForm()->GetPos().z));
	_pChild->GetTransForm()->SetScale(Vec3(vScale.x * g_Global.fWidth, vScale.y * g_Global.fHeight, _pChild->GetTransForm()->GetScale().z));
	
	if (_pChild->GetCollider() != nullptr)
	{
		Vec3 vCollScale = _pChild->GetCollider()->GetScale() / Vec3(ORI_SIZEX, ORI_SIZEY, 1.f);
		_pChild->GetCollider()->SetScale(Vec3(vCollScale.x * g_Global.fWidth, vCollScale.y * g_Global.fHeight, _pChild->GetCollider()->GetScale().z));
	}
	if (_pChild->GetChild().empty())
		return;

	for (size_t i = 0; i < _pChild->GetChild().size(); i++)
	{
		UIPos(_pChild->GetChild()[i]);
	}
}