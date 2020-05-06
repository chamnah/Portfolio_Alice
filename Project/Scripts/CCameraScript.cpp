#include "CCameraScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"

CCameraScript::CCameraScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CCAMERASCRIPT);

//	vOldMousePos = Vec3(0.f,0.f,0.f);

	//vPos = new Vec3;
}

CCameraScript::~CCameraScript()
{
	//delete vPos;
}

int CCameraScript::FinalUpdate()
{
	Vec3& vRight = TransForm()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3& vFront = TransForm()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 vPos = TransForm()->GetPos();
	Vec3 vRot = TransForm()->GetRotation();

	if (KEYHOLD(KEY_TYPE::KEY_UP))
	{
		vPos += vFront * 500.f * DT;
	}
	if (KEYHOLD(KEY_TYPE::KEY_DOWN))
	{
		vPos -= vFront * 500.f * DT;
	}
	if (KEYHOLD(KEY_TYPE::KEY_LEFT))
	{
		vPos -= vRight * 500.f * DT;
	}
	if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
	{
		vPos += vRight * 500.f * DT;
	}

	Vec3 vMousePos = CKeyMgr::GetInst()->GetDXMousePos();
	Vec3 vDiff = vMousePos - m_vOldMousePos;

	if (KEYHOLD(KEY_TYPE::KEY_RBTN))
	{
		vRot.y += vDiff.x * DT * 0.8f;
		vRot.x -= vDiff.y * DT * 0.8f;
	}

	TransForm()->SetPos(vPos);
	TransForm()->SetRotation(vRot);

	m_vOldMousePos = vMousePos;

	return 0;
}