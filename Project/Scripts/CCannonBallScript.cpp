#include "CCannonBallScript.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CMeshRender.h"
#include "CDistortionScript.h"
#include "CCollider.h"

CCannonBallScript::CCannonBallScript()
	:m_fSpeed(100.f)
	, m_bDeath(false)
	, m_fUpPower(220.f)
	, m_fGravity(120.f)
	, m_fAccTime(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCANNONBALLSCRIPT);
}

CCannonBallScript::~CCannonBallScript()
{
}

int CCannonBallScript::Update()
{
	if (m_bDeath)
		return 0;
	Vec3 vPos = TransForm()->GetPos();
	m_fUpPower -= m_fGravity * DT;
	vPos += m_vDir * m_fSpeed * DT;
	vPos.y += m_fUpPower * DT;

	TransForm()->SetPos(vPos);

	if (m_fAccTime > 5.f)
	{
		m_bDeath = true;
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	}

	return 0;
}

void CCannonBallScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() != L"Land")
		return;
	if (!m_bDeath)
	{
		CGameObj* pDistortion = CResMgr::GetInst()->Load<CPrefab>(L"Distortion")->CreateObjClone(L"Default");
		pDistortion->GetMeshRender()->SetParam(CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT),SHADER_PARAM::TEX_0);
		pDistortion->GetTransForm()->SetPos(TransForm()->GetPos());
		CDistortionScript* pScript = new CDistortionScript;
		pDistortion->AddComponent(pScript);
		m_bDeath = true;
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	}
}