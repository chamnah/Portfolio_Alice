#include "CButtonScript.h"
#include "CResMgr.h"
#include "CMeshRender.h"
#include "CScriptMgr.h"
#include "KeyMgr.h"
#include "CPlayerMgr.h"
#include "CText.h"

CButtonScript::CButtonScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CBUTTONSCRIPT);
}

CButtonScript::~CButtonScript()
{
}

void CButtonScript::CollisionEnter(CCollider * _pColl)
{
	m_pTex = MeshRender()->GetVecMtrl()[0]->GetArrTexture()[0];
	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(L"ButtonOn");
	if (pTex == nullptr)
	{
		pTex = CTexture::CreateTexture(L"..\\content\\Texture\\pc_button_on.tga");
		CResMgr::GetInst()->AddResource<CTexture>(L"ButtonOn", pTex);
	}
	MeshRender()->SetParam(pTex, SHADER_PARAM::TEX_0);
}

void CButtonScript::Collision(CCollider * _pColl)
{
	if (KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		if (CPlayerMgr::GetInst()->GetWeaponType() == WEAPON_TYPE::END)
			return;
		int iPrice = _wtoi(CPlayerMgr::GetInst()->GetPrice().c_str());
		int iTooth = CPlayerMgr::GetInst()->GetTooth();
		if (iPrice > iTooth)
		{
			vector<CGameObj*>& vecChild = GetOwner()->GetChild();
			if (!vecChild.empty())
				vecChild[0]->GetText()->SetText(L"돈이 모자랍니다.");
			return;
		}
		vector<CGameObj*>& vecChild = GetOwner()->GetChild();
		if (!vecChild.empty())
			vecChild[0]->GetText()->SetText(L"구매 감사합니다.");


		CPlayerMgr::GetInst()->AddWeaponUp(1, CPlayerMgr::GetInst()->GetWeaponType());
		CPlayerMgr::GetInst()->SetTooth(iTooth - iPrice);
	}
}

void CButtonScript::CollisionExit(CCollider * _pColl)
{
	MeshRender()->SetParam(m_pTex, SHADER_PARAM::TEX_0);
}

int CButtonScript::Update()
{
	return 0;
}