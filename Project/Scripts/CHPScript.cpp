#include "CHPScript.h"
#include "CScriptMgr.h"
#include "CMeshRender.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CPlayerMgr.h"

CHPScript::CHPScript()
	:m_iCurHP(0)
	, m_iMaxHP(64)
{
	SetScriptType((UINT)SCRIPT_TYPE::CHPSCRIPT);
	CPlayerMgr::GetInst()->SetHP_UI(this);


	CResMgr::GetInst()->Load<CPrefab>(L"Upgrade");
}

CHPScript::~CHPScript()
{
}

void CHPScript::Start()
{
	CPlayerMgr::GetInst()->UIPos(GetOwner());
//m_vecHP.resize(8);
//CTexture* pTex = CResMgr::GetInst()->LoadAndAddTexture(L"HP7",L"HP7.tga");
//
//for (int i = 0; i < 8; i++)
//{
//	m_vecHP[i] = new CGameObj;
//	m_vecHP[i]->AddComponent(new CMeshRender);
//	m_vecHP[i]->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	m_vecHP[i]->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred")->Clone());
//	m_vecHP[i]->GetMeshRender()->SetParam(pTex, SHADER_PARAM::TEX_0);
//	m_vecHP[i]->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));
//	m_vecHP[i]->GetTransForm()->SetScale(Vec3(30.f, 30.f, 1.f));
//	int iZ = -(i + 1);
//	m_vecHP[i]->GetTransForm()->SetPos(Vec3(i * 40, 0, iZ));
//	GetOwner()->AddChild(m_vecHP[i]);
//}
}

int CHPScript::Update()
{
	bool bReset = CPlayerMgr::GetInst()->GetHP_UI_Reset();
	if (bReset)
	{
		Reset();
		CPlayerMgr::GetInst()->SetHP_UI_Reset(false);
	}



	return 0;
}

void CHPScript::AddHP(int _iAdd)
{
}

void CHPScript::SubHP()
{
}

void CHPScript::Reset()
{
	vector<CGameObj*>& vecChild = GetOwner()->GetChild();
	m_iCurHP = CPlayerMgr::GetInst()->GetHP();
	int iIdx = m_iCurHP - 1;
	if (iIdx <= -1) // HP가 0일때 죽었을때
	{
		for (size_t i = 0; i < vecChild.size(); i++)
			vecChild[i]->SetShow(false);
		return;
	}

	int iCount = (m_iCurHP - 1) / vecChild.size();
	for (size_t i = 0; i < iCount; i++)
	{
		CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(L"HP7.tga");
		vecChild[i]->GetMeshRender()->SetParam(pTex, SHADER_PARAM::TEX_0);
		vecChild[i]->SetShow(true);
	}
	
	int iBar = (m_iCurHP - 1) % 8;

	wstring strHP = L"HP";
	wchar_t szNum[3] = L"";
	_itow_s(iBar, szNum, 10);
	strHP += szNum;

	CTexture* pTex = CResMgr::GetInst()->LoadAndAddTexture(strHP, strHP + L".tga");
	vecChild[iCount]->GetMeshRender()->SetParam(pTex, SHADER_PARAM::TEX_0);
	vecChild[iCount]->SetShow(true);

	for (size_t i = iCount + 1; i < 8; i++)
		vecChild[i]->SetShow(false);
}