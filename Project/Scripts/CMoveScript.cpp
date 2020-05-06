#include "CMoveScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"

int CMoveScript::Update()
{
	Vec3 vPos = TransForm()->GetPos();
	if (KEYHOLD(KEY_TYPE::KEY_W))
		vPos.y += m_fSpeed * DT;
	if (KEYHOLD(KEY_TYPE::KEY_S))
		vPos.y -= m_fSpeed * DT;
	if (KEYHOLD(KEY_TYPE::KEY_A))
		vPos.x -= m_fSpeed * DT;
	if (KEYHOLD(KEY_TYPE::KEY_D))
		vPos.x += m_fSpeed * DT;

	TransForm()->SetPos(vPos);

	return 0;
}

CMoveScript::CMoveScript()
	:m_fSpeed(500.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CMOVESCRIPT);
}

CMoveScript::~CMoveScript()
{
}