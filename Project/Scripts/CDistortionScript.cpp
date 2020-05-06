#include "CDistortionScript.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"

CDistortionScript::CDistortionScript()
	:m_fAccTime(0.f)
	, m_vChangeScale(150.f, 150.f, 150.f)
	, m_bDeath(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CDISTORTIONSCRIPT);
}

CDistortionScript::~CDistortionScript()
{
}

int CDistortionScript::Update()
{
	if (m_bDeath)
		return 0;
	Vec3 vScale = TransForm()->GetScale();
	vScale += m_vChangeScale * DT;
	TransForm()->SetScale(vScale);
	m_fAccTime += DT;

	/*Vec3 vDir = CRenderMgr::GetInst()->GetMainCam()->TransForm()->GetPos() - TransForm()->GetPos();
	vDir.Normalize();
	TransForm()->SetLook(vDir);*/

	Vec3 vDir = TransForm()->GetPos() - CRenderMgr::GetInst()->GetMainCam()->TransForm()->GetPos();
	vDir.Normalize();
	TransForm()->SetLook(vDir);

	Vec3 vUp = TransForm()->GetLocalDir(DIR_TYPE::UP);
	Mtx matRot = XMMatrixRotationAxis(vUp, -XM_PI / 2.f);
	TransForm()->RotationMatrix(matRot);

	/*Vec3 vCamDir = Transform()->GetWorldPos() - m_pMainCam->Transform()->GetWorldPos();
	vCamDir.Normalize();
	Transform()->SetLookAt(vCamDir);

	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Matrix matRot = XMMatrixRotationAxis(vUp, -XM_PI / 2.f);

	Transform()->RotationMatrix(matRot);*/

	/*Mtx matRot = XMMatrixRotationAxis(vUp, -XM_PI / 2.f);

	Transform()->RotationMatrix(matRot);*/

	if (m_fAccTime > 2.f)
	{
		m_bDeath = true;
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
	}

	return 0;
}