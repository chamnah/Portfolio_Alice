#include "CGizmoScript.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "CTestMgr.h"

CGizmoScript::CGizmoScript()
	:m_eDirType(DIR_TYPE::END)
{
	m_vDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f,0.f,0.f);
	m_vDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_vDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);
}

CGizmoScript::~CGizmoScript()
{
}

int CGizmoScript::Update()
{
	if (KEYTAB(KEY_TYPE::KEY_LBTN))
	{
		CTestMgr::GetInst()->SetPicking(true);
	}

	if (KEYHOLD(KEY_TYPE::KEY_LBTN))
	{
		CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
		
		if (pMainCam == nullptr || (UINT)m_eDirType > (UINT)DIR_TYPE::FRONT)
			return 0;

		Vec3 vRot = pMainCam->TransForm()->GetRotation();
		Mtx matRotation = XMMatrixRotationX(vRot.x);
		matRotation *= XMMatrixRotationY(vRot.y);
		matRotation *= XMMatrixRotationZ(vRot.z);

		Vec3 vMove = XMVector3TransformNormal(m_vDir[(UINT)m_eDirType], matRotation);
		vMove.Normalize();
		m_vOldMousePos = m_vCurMousePos;
		m_vCurMousePos = CKeyMgr::GetInst()->GetDXMousePos();//vPos + (vDir * fDist);

		Vec3 vDiff = (m_vCurMousePos - m_vOldMousePos)* vMove;     /*vDiff.x = fabsf(m_vCurMousePos.x - m_vOldMousePos.x) * vMove.x; vDiff.y = fabsf(m_vCurMousePos.y - m_vOldMousePos.y) * vMove.y; vDiff.z = fabsf(m_vCurMousePos.z - m_vOldMousePos.z) * vMove.z;*/
		float fDiff = vDiff.x + vDiff.y + vDiff.z;

		if (m_eDirType == DIR_TYPE::FRONT)
			m_vDir[(UINT)m_eDirType] = -m_vDir[(UINT)m_eDirType];

		if (fDiff < -100.f || fDiff > 100.f)
			int i = 0;

		CGameObj* pRoot = GetParentOfOwner()->GetParent();
		if (pRoot == nullptr)
			pRoot = GetParentOfOwner();
		pRoot->GetTransForm()->SetPos(pRoot->GetTransForm()->GetPos() + (m_vDir[(UINT)m_eDirType] * fDiff));
	}
	else
	{
		m_vOldMousePos = CKeyMgr::GetInst()->GetDXMousePos();
		m_vCurMousePos = m_vOldMousePos;
	}

	if (KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		CTestMgr::GetInst()->SetPicking(false);
	}
	return 0;
}