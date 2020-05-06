#include "CEffectScript.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CScriptMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "CMesh.h"

CEffectScript::CEffectScript()
	:m_fDeathTime(0.f)
	, m_bDeath(false)
	, m_fAccTime(0.f)
	, m_eType(EFFECT_TYPE::BILLBOARD)
{
	SetScriptType((UINT)SCRIPT_TYPE::CEFFECTSCRIPT);
	//SetFirework(6, 6);
}

CEffectScript::~CEffectScript()
{
}

int CEffectScript::Update()
{
	m_fAccTime -= DT;

	if (m_eType == EFFECT_TYPE::FADEOUT)
	{
		static Vec4 vDiff = MeshRender()->GetMtrl()->GetDiff();
		Vec4 vColor = vDiff * (m_fAccTime / m_fDeathTime);
		MeshRender()->GetMtrl()->SetParam(&vColor, SHADER_PARAM::MTRL);

		if (!m_bDeath && m_fAccTime < 0)
		{
			m_bDeath = true;
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
		}
	}
	else if (m_eType == EFFECT_TYPE::BILLBOARD)
	{
		CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
		Vec4 vPos = Vec4(pMainCam->TransForm()->GetPos(),1.f);
		Vec4 vDir = Vec4(pMainCam->TransForm()->GetWorldDir(DIR_TYPE::UP),0.f);
		MeshRender()->SetParam(&vDir, SHADER_PARAM::VEC4_0);
		MeshRender()->SetParam(&vPos,SHADER_PARAM::VEC4_1);
	}
	else if (m_eType == EFFECT_TYPE::TIMEOUT)
	{
		if (!m_bDeath && m_fAccTime < 0)
		{
			m_bDeath = true;
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),NULL });
		}
	}
	return 0;
}

void CEffectScript::SetFirework(int _iHoriz, int _iVertical)
{
	m_vecDir.clear();
	Vec3 vDir = {};
	for (size_t i = 1; i < _iHoriz - 1; i++)
	{
		vDir.y = cosf(XM_PI * (i / _iHoriz));
		for (size_t j = 0;  j < _iVertical;  j++)
		{
			vDir.x = cosf(XM_2PI * (j / _iVertical));
			vDir.z = sinf(XM_2PI * (j / _iVertical));
			m_vecDir.push_back(vDir);
		}
	}

	MeshRender()->GetMesh();
}