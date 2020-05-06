#include "CShrinkScript.h"
#include "CScriptMgr.h"
#include "CPlayerMgr.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "TimeMgr.h"

CShrinkScript::CShrinkScript()
	:m_fAccTime(0.f)
	, m_vColor{1.f,0.f,1.f,1.f}
{
	SetScriptType((UINT)SCRIPT_TYPE::CSHRINKSCRIPT);
}

CShrinkScript::~CShrinkScript()
{
}

int CShrinkScript::Update()
{
	if (CPlayerMgr::GetInst()->GetSmall())
	{
		m_fAccTime += DT * 0.5f;
		if (m_fAccTime >= 1.f)
			m_fAccTime = 1.f;
		Vec4 vColor = m_vColor * m_fAccTime;
		MeshRender()->GetMtrl()->SetParam(&vColor, SHADER_PARAM::MTRL);
	}
	else
	{
		m_fAccTime -= DT * 0.5f;
		if (m_fAccTime <= 0.f)
			m_fAccTime = 0.f;
		Vec4 vColor = m_vColor * m_fAccTime;
		MeshRender()->GetMtrl()->SetParam(&vColor, SHADER_PARAM::MTRL);
	}

	return 0;
}