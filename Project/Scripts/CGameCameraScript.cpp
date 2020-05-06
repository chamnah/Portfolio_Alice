#include "CGameCameraScript.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CPlayerMgr.h"
#include "CLayer.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CCollisionMgr.h"

CGameCameraScript::CGameCameraScript()
	:m_pPlayer(nullptr)
	, m_bPlayer(false)
	, m_bExit(false)
	, m_fDist(600.f)
	, m_fDifference(64.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CGAMECAMERASCRIPT);
	m_pPlayer = new CGameObj;
}

CGameCameraScript::~CGameCameraScript()
{
	SAFE_DELETE(m_pPlayer);
	m_pPlayer = nullptr;
}

int CGameCameraScript::Update()
{
	Vec3 vPos = TransForm()->GetPos();
	vPos.z = -m_fDist;
	TransForm()->SetPos(vPos);
	TransForm()->FinalUpdate();
	return 0;
}

int CGameCameraScript::LateUpdate()
{
	if (m_pPlayer == nullptr)
		return 0;

	CGameObj* pObj = CPlayerMgr::GetInst()->GetPlayer();
	if (pObj == nullptr || !CSceneMgr::GetInst()->GetPlay())
	{
		m_bPlayer = false;
		return 0;
	}

	if (m_bExit)
	{
		Vec3 vDir = Vec3(0.f, 0.f, -1.f);
		vDir.Normalize();

		TransForm()->SetPos(TransForm()->GetPos() + (vDir * 200 * DT));
		if (TransForm()->GetPos().z <= -600.f)
		{
			m_bExit = false;
			Vec3 vPos = TransForm()->GetPos();
			vPos.z = -600.f;
			TransForm()->SetPos(vPos);
		}
	}

	Vec3 vPos = pObj->GetTransForm()->GetPos();

	m_pPlayer->GetTransForm()->SetPos(vPos);

	if (m_bPlayer == false)
	{
		vPos = Vec3(0.f, 200.f, -600.f);
		GetOwner()->SetParent(m_pPlayer);
		TransForm()->SetPos(vPos);
		m_bPlayer = true;
	}

	Vec3 vMousePos = CKeyMgr::GetInst()->GetDXMousePos();
	Vec3 vDiff = vMousePos - m_vOldMousePos;

	m_vOldMousePos = vMousePos;

	if (KEYHOLD(KEY_TYPE::KEY_RBTN))
	{
		m_vRot.y += vDiff.x * DT * 0.8f;
		m_vRot.x -= vDiff.y * DT * 0.8f;
	}

	m_pPlayer->GetTransForm()->SetRotation(m_vRot);
	m_pPlayer->FinalUpdate();

	CPlayerMgr::GetInst()->SetAnimRot(m_vRot);

	vector<CCollider*>& vecColl = Camera()->GetAllColl();
	float fDist = m_fDist;
	float fTemp = 0.f;
	bool bRay = false;
	
	Camera()->CalcRay();
	for (size_t i = 0; i < vecColl.size(); i++)
	{
		if (vecColl[i]->IntersectsRay(Camera()->GetRay().vStart, TransForm()->GetWorldDir(DIR_TYPE::FRONT), fTemp))
		{
			if (fDist > fTemp)
			{
				fDist = fTemp;
				bRay = true;
			}
		}
	}

	if (bRay)
	{
		vPos = TransForm()->GetPos(); 
		vPos.z = -(m_fDist - (fDist + m_fDifference));
		TransForm()->SetPos(vPos);
	}

	return 0;
}

int CGameCameraScript::FinalUpdate()
{
	return 0;
}

void CGameCameraScript::CollisionEnter(CCollider * _pColl)
{
	/*m_bExit = false;
	Vec3 vDir = Vec3(0.f,0.f,1.f);
	vDir.Normalize();

	while(CCollisionMgr::GetInst()->IsCollision(_pColl, Collider()))
	{
		TransForm()->SetPos(TransForm()->GetPos() + (vDir * 100 * DT));
		TransForm()->FinalUpdate();
		Collider()->MatrixUpdate();
	}*/
}

void CGameCameraScript::Collision(CCollider * _pColl)
{
	//CollisionEnter(_pColl);
}

void CGameCameraScript::CollisionExit(CCollider * _pColl)
{
	//m_bExit = true;
}