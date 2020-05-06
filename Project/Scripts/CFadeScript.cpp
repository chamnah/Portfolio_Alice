#include "CFadeScript.h"
#include "CScriptMgr.h"
#include "TimeMgr.h"
#include "CMeshRender.h"

CFadeScript::CFadeScript()
	:m_fAccTime(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CFADESCRIPT);
}

CFadeScript::~CFadeScript()
{
}

int CFadeScript::Update()
{
	m_fAccTime += DT;

	Vec4 vColor = Vec4(0.f,0.f,0.f, m_fAccTime);
	if (m_fAccTime >= 1.f)
		m_fAccTime = 1.f;

	MeshRender()->SetParam(&vColor,SHADER_PARAM::MTRL);

	return 0;
}