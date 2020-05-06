#include "CCarScript.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CCollider.h"

CCarScript::CCarScript()
	:m_fAccSpeed(0.f)
	, m_fSpeed(10.f)
	,m_fFinalSpeed(0.f)
	,m_fFriction(50.f)
	,m_eDir(CAR_DIR::STOP)
	//,m_pTestOwner(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCARSCRIPT);
}

CCarScript::~CCarScript()
{
	//SAFE_DELETE(m_pTestOwner);
}

void CCarScript::Start()
{
	/*m_pTestOwner = new CGameObj;
	GetOwner()->SetParent(m_pTestOwner);*/
}

int CCarScript::Update()
{

	Vec3 vPos = TransForm()->GetPos();

	bool bStop = false;

	if (KEYHOLD(KEY_TYPE::KEY_UP))
	{
		m_fAccSpeed += DT * m_fSpeed;

		if (m_fAccSpeed > 0.f)
			m_eDir = CAR_DIR::FRONT;
	}
	else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
	{
		m_fAccSpeed += DT * -m_fSpeed;
		if (m_fAccSpeed < 0.f)
			m_eDir = CAR_DIR::BACK;
	}
	else
	{
		bStop = true;
		if(m_eDir == CAR_DIR::FRONT)
			m_fAccSpeed = m_fFinalSpeed;
		else
			m_fAccSpeed = -m_fFinalSpeed;

		m_fFinalSpeed = fabsf(m_fAccSpeed) - (DT * m_fSpeed);
	}

	if(!bStop)
		m_fFinalSpeed = fabsf(m_fAccSpeed) - (m_fFriction * DT);

	if (m_eDir == CAR_DIR::FRONT)
		m_vDir = TransForm()->GetLocalDir(DIR_TYPE::FRONT);
	else if (m_eDir == CAR_DIR::BACK)
		m_vDir = -TransForm()->GetLocalDir(DIR_TYPE::FRONT);

	if (m_fFinalSpeed < 0.f)
		m_fFinalSpeed = 0.f;
	
	if (KEYTAB(KEY_TYPE::KEY_LEFT))
	{	
		Vec3 vLeft = -(TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
		Vec3 vCarPos = TransForm()->GetRTMtx().Translation();
		Vec3 vOriPos = vCarPos;
		vCarPos += 200.f * vLeft;
		GetParentOfOwner()->GetTransForm()->SetPos(vCarPos); // 부모 위치 먼저 계산한다.

		GetParentOfOwner()->FinalUpdate();
		TransForm()->SetInvParentPos(vOriPos);
		float fRotY = GetParentOfOwner()->GetTransForm()->GetRotation().y;
		fRotY -= m_fFinalSpeed / 400.f;
		GetParentOfOwner()->GetTransForm()->SetRotation(Vec3(0.f, fRotY, 0.f));
	}	
	else if (KEYHOLD(KEY_TYPE::KEY_LEFT))
	{	
		float fRotY = GetParentOfOwner()->GetTransForm()->GetRotation().y;
		fRotY -= m_fFinalSpeed / 400.f;
		GetParentOfOwner()->GetTransForm()->SetRotation(Vec3(0.f, fRotY, 0.f));
	}	
	else if (KEYTAB(KEY_TYPE::KEY_RIGHT))
	{
		Vec3 vRight = (TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
		Vec3 vCarPos = TransForm()->GetRTMtx().Translation();
		Vec3 vOriPos = vCarPos;
		vCarPos += 200.f * vRight;
		GetParentOfOwner()->GetTransForm()->SetPos(vCarPos); // 부모 위치 먼저 계산한다.

		GetParentOfOwner()->FinalUpdate();
		TransForm()->SetInvParentPos(vOriPos);
		float fRotY = GetParentOfOwner()->GetTransForm()->GetRotation().y;
		fRotY += m_fFinalSpeed / 400.f;
		GetParentOfOwner()->GetTransForm()->SetRotation(Vec3(0.f, fRotY, 0.f));
	}
	else if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
	{
		float fRotY = GetParentOfOwner()->GetTransForm()->GetRotation().y;
		fRotY += m_fFinalSpeed / 400.f;
		GetParentOfOwner()->GetTransForm()->SetRotation(Vec3(0.f, fRotY, 0.f));
	}
	else
		vPos += m_vDir * m_fFinalSpeed;
	

	TransForm()->SetPos(vPos);
	return 0;
}

void CCarScript::CollisionEnter(CCollider * _pColl)
{
	Vec3 vDir = TransForm()->GetPos() - TransForm()->GetOldPos();
	vDir.Normalize();
}