#include "CSceneMgr.h"
#include "CScene.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CTransForm.h"
#include "CResMgr.h"
#include "CShaderMgr.h"
#include "CScript.h"
#include "CCamera.h"
#include "CPrefab.h"
#include "CMaterial.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CAnimator2D.h"
#include "CLayer.h"
#include "CStateMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CRenderMgr.h"

CGameObj* pMonster = nullptr;

CSceneMgr::CSceneMgr()
	:m_bDelObj(true)
	, m_bPlay(true)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::AddObject(const wstring& _wcsLayer, CGameObj * _pObj)
{
	m_pCurScene->AddGameObject(_wcsLayer, _pObj);
}

void CSceneMgr::AddObject(const UINT _iIdx, CGameObj * _pObj)
{
	m_pCurScene->AddGameObject(_iIdx, _pObj);
}

CLayer * CSceneMgr::GetLayer(int _iIdx)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_iIdx);
}

CLayer* CSceneMgr::GetLayer(const wstring & _wcsName)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_wcsName);
}
UINT CSceneMgr::GetLayerIndex(const wstring & _wcsName)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayerIndex(_wcsName);
}
CGameObj * CSceneMgr::FindObject(const wstring& _wcsName)
{
	return m_pCurScene->FindObject(_wcsName);
}
void CSceneMgr::AddLayer(const wstring & _wcsLayer)
{
	for (size_t i = 0; i < m_pCurScene->GetCurLayerCount(); i++)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);

		if (pLayer->GetName() == _wcsLayer)
		{
			MessageBox(CRenderMgr::GetInst()->GetGameHwnd(), L"같은 이름으로 된 레이어가 존재합니다.", L"오류", MB_OK);
			return;
		}
	}

	m_pCurScene->AddLayer(_wcsLayer);
}
const wstring & CSceneMgr::GetLayerName(UINT _iIdx)
{
	return m_pCurScene->GetLayerName(_iIdx);
}
void CSceneMgr::ChangeScene(CScene * pScene)
{
	//여기에서 기존에 있던 녀석을 삭제 시킨다.
	if (m_bDelObj)
	{
		SAFE_DELETE(m_pCurScene);
	}
	else
	{
		m_bDelObj = true;
		vector<CGameObj*>& vecPlayer = m_pCurScene->GetLayer(L"Player")->GetGameObj();
		vector<CGameObj*>& vecCamera = m_pCurScene->GetLayer(L"Camera")->GetGameObj();
		vector<CGameObj*>& vecUI = m_pCurScene->GetLayer(L"UI")->GetGameObj();
		vector<CGameObj*>& vecMenu = m_pCurScene->GetLayer(L"Menu")->GetGameObj();

		vector<CGameObj*>& vecNewPlayer = pScene->GetLayer(L"Player")->GetGameObj();
		vector<CGameObj*>& vecNewCamera = pScene->GetLayer(L"Camera")->GetGameObj();
		vector<CGameObj*>& vecNewUI = pScene->GetLayer(L"UI")->GetGameObj();
		vector<CGameObj*>& vecNewMenu = pScene->GetLayer(L"Menu")->GetGameObj();

		SafeArrayDelete(vecNewPlayer);
		SafeArrayDelete(vecNewCamera);
		SafeArrayDelete(vecNewUI);
		SafeArrayDelete(vecNewMenu);

		//vecCamera[0]->GetTransForm()->SetPos(Vec3{ 0.f,0.f,0.f });

		for (size_t i = 0; i < vecPlayer.size(); i++)
			vecNewPlayer.push_back(vecPlayer[i]);
		
		for (size_t i = 0; i < vecCamera.size(); i++)
			vecNewCamera.push_back(vecCamera[i]);
		
		for (size_t i = 0; i < vecUI.size(); i++)
			vecNewUI.push_back(vecUI[i]);

		for (size_t i = 0; i < vecMenu.size(); i++)
			vecNewMenu.push_back(vecMenu[i]);

		vecPlayer.clear();
		vecCamera.clear();
		vecUI.clear();
		vecMenu.clear();

		SAFE_DELETE(m_pCurScene);
	}

	m_pCurScene = pScene;
}

UINT CSceneMgr::GetCurLayerCount()
{
	return m_pCurScene->GetCurLayerCount();
}

void CSceneMgr::FrontSorting(CGameObj * _pObj)
{
	if (_pObj->GetParent() != nullptr)
		return;

	CLayer* pLayer = m_pCurScene->GetLayer(_pObj->GetID());

	if (pLayer == nullptr)
		return;

	vector<CGameObj*>& vecObj = pLayer->GetGameObj();
	vector<CGameObj*>::iterator iter = vecObj.begin();

	bool bErase = false;

	for (; iter != vecObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			vecObj.erase(iter);
			bErase = true;
			break;
		}
	}

	if (!bErase)
		return;
	
	vecObj.push_back(_pObj);

	for (size_t i = vecObj.size() - 1; i > 0 ; --i)
		vecObj[i] = vecObj[i - 1];

	vecObj[0] = _pObj;
}

void CSceneMgr::SetStop(bool _bStop)
{
	m_pCurScene->SetStop(_bStop);
}

bool CSceneMgr::GetStop()
{
	return m_pCurScene->GetStop();
}

bool CSceneMgr::GetObjOfLayer(wstring _wcsLayer, int _iIdx, CGameObj** _pObj)
{
	CLayer* pLayer = m_pCurScene->GetLayer(_wcsLayer);

	if (pLayer == nullptr)
		return false;

	if (pLayer->GetGameObj().empty())
		return false;

	*_pObj = pLayer->GetGameObj()[_iIdx];

	if (*_pObj == nullptr)
		return false;

	return true;
}

void CSceneMgr::RenderSort(UINT _iMask)
{
	while (!m_queueRender.empty())
	{
		CGameObj* pTop = m_queueRender.top();
		if (_iMask & (1 << pTop->GetID()))
		{
			if (pTop->GetLayerName() == L"Mirror")
				CStateMgr::GetInst()->DepthRender(L"Stencil");
			else if (pTop->GetLayerName() == L"MirrorObj")
				CStateMgr::GetInst()->DepthRender(L"StencilObj");
			else
				CStateMgr::GetInst()->DepthRender(L"Default");

			pTop->Render();
			m_queueRender.pop();
		}
		else
		{
			m_vecRender.push_back(m_queueRender.top());
			m_queueRender.pop();
		}
	}

	for (size_t i = 0; i < m_vecRender.size(); i++)
	{
		if (_iMask & (1 << m_vecRender[i]->GetID()))
		{
			if (m_vecRender[i]->GetLayerName() == L"Mirror")
				CStateMgr::GetInst()->DepthRender(L"Stencil");
			else if (m_vecRender[i]->GetLayerName() == L"MirrorObj")
				CStateMgr::GetInst()->DepthRender(L"StencilObj");
			else
				CStateMgr::GetInst()->DepthRender(L"Default");

			m_vecRender[i]->Render();
		}
	}

}

void CSceneMgr::RenderClear()
{
	/*for (size_t i = 0; i < m_queueRender.size(); i++)
		m_queueRender.pop();*/
	
	m_queueRender = priority_queue<CGameObj*, vector<CGameObj*>, cmp>();

	m_vecRender.clear();
}

void CSceneMgr::AllLayerAllObjClear()
{
	for (size_t i = 0; i < m_pCurScene->GetCurLayerCount(); i++)
	{
		m_pCurScene->GetLayer(i)->GetAllObj().clear();
	}
	
}

void CSceneMgr::RemoveLayer(const wstring & _strLayer)
{
	if (_strLayer == L"Default")
	{
		MessageBox(CRenderMgr::GetInst()->GetGameHwnd(),L"기본 레이어는 삭제할수 없습니다.",L"오류", MB_OK);
		return;
	}

	UINT iCount = m_pCurScene->GetCurLayerCount();
	CLayer* pDefault = m_pCurScene->GetLayer(L"Default");

	for (size_t i = 0; i < iCount; i++)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);
		if (pLayer->GetName() == _strLayer)
		{
			vector<CGameObj*>& vecObj = pLayer->GetGameObj();
			for (size_t i = 0; i < vecObj.size(); i++)
			{
				pDefault->AddGameObject(vecObj[i]);
			}
			vecObj.clear();

			break;
		}
	}
}

void CSceneMgr::ChangeName(const wstring & _strLayer, const wstring & _strName)
{
	UINT iIndex = GetLayerIndex(_strLayer);
	CLayer* pLayer = m_pCurScene->GetLayer(iIndex);
	pLayer->SetName(_strName);
	vector<CGameObj*>& vecObj = pLayer->GetGameObj();
	for (size_t i = 0; i < vecObj.size(); i++)
		vecObj[i]->SetLayerName(_strName);
}

void CSceneMgr::Init()
{
	m_pCurScene = new CScene;
	
	AddLayer(L"Default");
	AddLayer(L"Player");
	AddLayer(L"Effect");
	AddLayer(L"Light");
	AddLayer(L"UI");
	AddLayer(L"Camera");
	AddLayer(L"Land");
	AddLayer(L"Menu");
	AddLayer(L"Monster");

	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"Default");
	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"Land");
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Default");
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Land");
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Menu", L"Menu");
	CCollisionMgr::GetInst()->CollisionCheck(L"Camera", L"Default");

	CResMgr::GetInst()->SoundInit();

	m_pCurScene->Awake();
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();
	AllLayerAllObjClear();

	m_pCurScene->FinalUpdate();
	CCollisionMgr::GetInst()->Update();
	//m_pCurScene->FinalUpdate();
}

void CSceneMgr::Render()
{
	/*int iLightCount = (int)m_vecLight.size();

	if (iLightCount > 10)
		iLightCount = 10;

	g_Global.iCount = iLightCount;

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"Global");

	pBuffer->UpdateData(&g_Global, sizeof(tGlobal));
	pBuffer->UpdateRegisterAll();

	pBuffer = CDirect::GetInst()->FindConstBuffer(L"LightInfo");
	
	if(iLightCount != 0)
		pBuffer->UpdateData(&m_vecLight[0], sizeof(tLightInfo) * iLightCount);
	pBuffer->UpdateRegisterAll();

	m_pCurScene->Render();

	m_vecLight.clear();*/
}

bool cmp::operator()(CGameObj * t, CGameObj * u)
{
	return t->GetTransForm()->GetPos().z < u->GetTransForm()->GetPos().z;
}