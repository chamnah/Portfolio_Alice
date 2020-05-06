#include "CLayer.h"
#include "CGameObj.h"
#include "CDirect.h"
#include "CSceneMgr.h"

void CLayer::Awake()
{
	for (auto& pObj : m_vecObj)
	{
		pObj->Awake();
	}
}

void CLayer::Update()
{
	for (auto& pObj : m_vecObj)
	{
		pObj->Update();
	}
}

void CLayer::LateUpdate()
{
	for (auto& pObj : m_vecObj)
	{
		pObj->LateUpdate();
	}
}

void CLayer::FinalUpdate()
{
	for (auto& pObj : m_vecObj)
	{
		pObj->FinalUpdate();
	}
}

void CLayer::Render()
{
	for (auto& pObj : m_vecAllObj)
	{
		pObj->Render();
	}

	m_vecAllObj.clear();
}

void CLayer::AddGameObject(CGameObj * _pObj)
{
	_pObj->SetID(m_iLayerID);
	_pObj->SetLayerName(GetName());
	m_vecObj.push_back(_pObj);
}

void CLayer::PopGameObject(CGameObj * _pObj)
{
	vector<CGameObj*>::iterator iter = m_vecObj.begin();

	for (; iter != m_vecObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			m_vecObj.erase(iter);
			break;
		}
	}

	iter = m_vecAllObj.begin();

	for (; iter != m_vecAllObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			m_vecAllObj.erase(iter);
			break;
		}
	}
}

CLayer::CLayer()
	:m_iLayerID(-1)
{
}

CLayer::~CLayer()
{
	SafeArrayDelete(m_vecObj);
}