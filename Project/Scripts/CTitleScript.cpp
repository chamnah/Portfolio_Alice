#include "CTitleScript.h"
#include "KeyMgr.h"
#include "CEventMgr.h"
#include "CScriptMgr.h"
#include "CSaveLoadMgr.h"
#include "TimeMgr.h"
#include "CMeshRender.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGameObj.h"
#include "CResMgr.h"

CTitleScript::CTitleScript()
	:m_bEnter(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CTITLESCRIPT);
}


CTitleScript::~CTitleScript()
{
}

void CTitleScript::Start()
{
	CResMgr::GetInst()->SoundPlay(L"Credits", -1, false);
}

int CTitleScript::Update()
{
	if (!m_bEnter && KEYTAB(KEY_TYPE::KEY_ENTER))
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Default");
		vector<CGameObj*>& vecObj = pLayer->GetGameObj();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetText())
			{
				vecObj[i]->DelComponent(COMPONENT_TYPE::TEXT);
			}
		}
		m_bEnter = true;
	}

	if (m_bEnter)
	{
		m_fAccTime += DT * 0.5f;
		Vec4 vColor = Vec4(0.f,0.f,0.f, m_fAccTime);
		MeshRender()->SetParam(&vColor, SHADER_PARAM::MTRL);
		if (m_fAccTime >= 1.f)
		{
			CSaveLoadMgr::SetChangeScene(true);
			CSaveLoadMgr::SetPath(L"..\\content\\Scene\\SceneTwo.scene");
			CSound* pSound = CResMgr::GetInst()->Load<CSound>(L"Credits");
			pSound->Stop();
			CResMgr::GetInst()->SoundPlay(L"Ch4_Queen_Entrance", -1);
		}
	}
	return 0;
}