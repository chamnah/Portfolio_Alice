#include "CPlayerScript.h"
#include "KeyMgr.h"
#include "CSceneMgr.h"
#include "CRenderMgr.h"
#include "CLayer.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CScriptMgr.h"

CPlayerScript::CPlayerScript()
	:m_bVision(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CPLAYERSCRIPT);
}

CPlayerScript::~CPlayerScript()
{
}

int CPlayerScript::Update()
{
	return 0;
	if (KEYTAB(KEY_TYPE::KEY_SPACE))
	{
		m_bVision = !m_bVision;
		if (m_bVision)
		{
			CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Default");
			vector<CGameObj*>& pObj= pLayer->GetAllObj();

			for (size_t i = 0; i < pObj.size(); i++)
			{
				if(pObj[i]->GetMeshRender() != nullptr)
				pObj[i]->GetMeshRender()->GetMtrl()->SetDepthStencil(CRenderMgr::GetInst()->GetDepthStencil(DEPTH_STENCIL_TYPE::LESS_EQUAL));
			}

			pLayer = CSceneMgr::GetInst()->GetLayer(L"UI");
			pObj = pLayer->GetAllObj();

			for (size_t i = 0; i < pObj.size(); i++)
			{
				/*if (pObj[i]->GetMeshRender() != nullptr)
					pObj[i]->GetMeshRender()->SetDeferred(false);*/
			}

			int iVision = 0;
			CRenderMgr::GetInst()->SetMergeFilterParam(&iVision, SHADER_PARAM::INT_0);
		}
		else
		{
			CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Default");
			vector<CGameObj*>& pObj = pLayer->GetAllObj();

			for (size_t i = 0; i < pObj.size(); i++)
			{
				if (pObj[i]->GetMeshRender() != nullptr)
				pObj[i]->GetMeshRender()->GetMtrl()->SetDepthStencil(nullptr);
			}

			pLayer = CSceneMgr::GetInst()->GetLayer(L"UI");
			pObj = pLayer->GetAllObj();

			for (size_t i = 0; i < pObj.size(); i++)
			{
				/*if (pObj[i]->GetMeshRender() != nullptr)
					pObj[i]->GetMeshRender()->SetDeferred(true);*/
			}

			int iVision = 1;
			CRenderMgr::GetInst()->SetMergeFilterParam(&iVision, SHADER_PARAM::INT_0);
		}
	}
	return 0;
}