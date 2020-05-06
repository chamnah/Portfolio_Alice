#include "CDoorScript.h"
#include "CPlayerMgr.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CSaveLoadMgr.h"
#include "KeyMgr.h"

CDoorScript::CDoorScript()
	:m_fAccTime(0.f)
	, m_bEvent(false)
	, m_bLeft(false)
	, m_bRot(false)

{
	SetScriptType((UINT)SCRIPT_TYPE::CDOORSCRIPT);

	BoolInfo Info = BoolInfo{ L"m_bLeft", };
	m_vecBool.push_back(Info);
	
	/*ValueInfo Info(&m_bLeft, typeid(m_bLeft).name(), strName);
	m_vecVal.push_back(Info);*/
}

CDoorScript::~CDoorScript()
{
}

int CDoorScript::Update()
{
	return 0;
}

int CDoorScript::LateUpdate()
{
	if (m_bRot)
		return 0;
	m_fAccTime += DT;
	if (m_fAccTime >= 1.f && m_fAccTime <= 2.f)
	{
		Vec3 vRot = TransForm()->GetRotation();
		if(m_vecBool[0].bVal == false)
			vRot.y += XM_PIDIV2 * DT;
		else
			vRot.y -= XM_PIDIV2 * DT;
		TransForm()->SetRotation(vRot);
	}
	else if (m_fAccTime >= 3.f)
	{
		CSceneMgr::GetInst()->SetPlay(true);
		m_fAccTime = 0.f;
		m_bRot = true;
	}

	return 0;
}

void CDoorScript::Save(FILE * _pFile)
{
	int iCount = m_vecBool.size();
	fwrite(&iCount,sizeof(int),1,_pFile);
	m_vecBool[0].strName = L"Left";
	for (size_t i = 0; i < m_vecBool.size(); i++)
	{
		fwrite(&m_vecBool[i].bVal, sizeof(bool), 1, _pFile);
		SaveWString(m_vecBool[i].strName,_pFile);
	}
}

void CDoorScript::Load(FILE * _pFile)
{
	m_vecBool.clear();
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, _pFile);
	for (size_t i = 0; i < iCount; i++)
	{
		BoolInfo Info;
		fread(&Info.bVal, sizeof(bool), 1, _pFile);
		Info.strName = LoadWString(_pFile);
		m_vecBool.push_back(Info);
	}
}