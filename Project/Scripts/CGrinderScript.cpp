#include "CGrinderScript.h"
#include "CAnimator3D.h"
#include "TimeMgr.h"
#include "CResMgr.h"
#include "CScriptMgr.h"
#include "CBulletScript.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CEventMgr.h"

CGrinderScript::CGrinderScript()
	:m_fAccTime(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CGRINDERSCRIPT);
}

CGrinderScript::~CGrinderScript()
{
}

void CGrinderScript::Start()
{
}

int CGrinderScript::Update()
{
	if (Animator3D() == nullptr)
		return 0;

	if (Animator3D()->GetCurAnimKey(L"fire"))
	{
		m_fAccTime += DT;
		if (m_fAccTime > 1.f)
		{
			CResMgr::GetInst()->SoundPlay(L"sfx_pepperg_fire01",1);
			m_fAccTime = 0.f;
			CGameObj* pBullet = CResMgr::GetInst()->Load<CPrefab>(L"Bullet")->CreateObjClone(L"Default");
			pBullet->GetTransForm()->SetPos(TransForm()->GetWorldPos());
			pBullet->SetTeamType(TEAM_TYPE::PLAYER);
			//pBullet->GetTransForm()->SetMtxOwner(GetOwner()->GetTransForm()->GetWorldMtx());
			pBullet->FinalUpdate();
			CBulletScript* pScript = new CBulletScript;
			pBullet->AddComponent(pScript);
			pScript->SetSpeed(300.f);
			
			CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

			vector<CCollider*>& vecColl = pMainCam->GetAllColl();
			Vec3 vEnd;
			bool bRay = false;
			float fComDist = 0.f;
			float fDist = 0.f;
			for (size_t i = 0; i < vecColl.size(); i++)
			{
				if (vecColl[i]->GetOwner()->GetLayerName() == L"Player")
					continue;
				if (vecColl[i]->IntersectsRay(pMainCam->GetRay().vStart, pMainCam->TransForm()->GetWorldDir(DIR_TYPE::FRONT), fDist))
				{
					if (!bRay)
					{
						bRay = true;
						vEnd = (pMainCam->GetRay().vStart + (pMainCam->TransForm()->GetWorldDir(DIR_TYPE::FRONT) * fDist));
						fComDist = fDist;
					}
					else if (fDist < fComDist)
					{
						vEnd = (pMainCam->GetRay().vStart + (pMainCam->TransForm()->GetWorldDir(DIR_TYPE::FRONT) * fDist));
						fComDist = fDist;
					}
				}
			}
			
			if (bRay)
			{
				Vec3 vDir = vEnd - pBullet->GetTransForm()->GetWorldPos();
				vDir.Normalize();
				pScript->SetDir(vDir);
			}
			else
			{
				Vec3 vStart = pMainCam->GetRay().vStart;
				Vec3 vDir = pMainCam->TransForm()->GetWorldDir(DIR_TYPE::FRONT);
				vEnd = (pMainCam->GetRay().vStart + (vDir * 10000.f));
				vDir = vEnd - pBullet->GetTransForm()->GetPos();
				vDir.Normalize();
				pScript->SetDir(vDir);
			}
		}
	}
	else
		m_fAccTime = 0.f;
	return 0;
}