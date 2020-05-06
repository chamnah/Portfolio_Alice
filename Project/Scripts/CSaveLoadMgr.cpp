#include "stdafx.h"
#include "CSaveLoadMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObj.h"
#include "CScript.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "CCollider.h"

bool CSaveLoadMgr::m_bLoad = false;
bool CSaveLoadMgr::m_bLoadScene = false;
bool CSaveLoadMgr::m_bChangeScene = false;
wstring CSaveLoadMgr::m_wcsPath = L"";
bool CSaveLoadMgr::m_bClient = false;
CScene* CSaveLoadMgr::m_pScene = nullptr;

void CSaveLoadMgr::SaveScene(const wstring& _wcsPath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _wcsPath.c_str(), L"wb");
	assert(pFile);

	SaveResource(pFile);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	assert(pCurScene);

	UINT iCount = pCurScene->GetCurLayerCount();
	fwrite(&iCount,sizeof(UINT),1,pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		SaveLayer(pCurScene->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void CSaveLoadMgr::SaveLayer(CLayer * _pLayer, FILE* _pFile)
{
	SaveWString(_pLayer->GetName(),_pFile);

	vector<CGameObj*>& vecObj = _pLayer->GetGameObj();

	UINT iObjCount = (UINT)vecObj.size();
	fwrite(&iObjCount,sizeof(UINT),1, _pFile);

	for (UINT i = 0; i < iObjCount; ++i)
	{
		SaveGameObject(vecObj[i],_pFile);
	}
}

void CSaveLoadMgr::SaveGameObject(CGameObj * _pObj, FILE * _pFile)
{
	_pObj->Save(_pFile);

	const vector<CScript*>& vecScript = _pObj->GetScript();
	UINT iSize = (UINT)vecScript.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		SaveWString(CScriptMgr::GetScriptName(vecScript[i]),_pFile);
		vecScript[i]->CComponent::Save(_pFile);
		vecScript[i]->Save(_pFile);
	}

	vector<CGameObj*>& vecChild = _pObj->GetChild();
	iSize = (UINT)vecChild.size();

	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		SaveGameObject(vecChild[i], _pFile);
	}
}

void CSaveLoadMgr::LoadScene(const wstring & _wcsPath)
{
	m_bLoad = true;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _wcsPath.c_str(), L"rb");
	assert(pFile);

	CScene* pScene = new CScene;

	LoadResource(pFile);

	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		CLayer* pLayer = new CLayer;
		pScene->AddLayer(pLayer);
		LoadLayer(pLayer,pFile);
	}
	fclose(pFile);
	
	CSceneMgr::GetInst()->ChangeScene(pScene);
}

CScene* CSaveLoadMgr::LoadSceneNoChange(const wstring & _wcsPath)
{
	m_bLoad = true;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _wcsPath.c_str(), L"rb");
	assert(pFile);

	CScene* pScene = new CScene;

	LoadResource(pFile);

	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		CLayer* pLayer = new CLayer;
		pScene->AddLayer(pLayer);
		LoadLayer(pLayer, pFile);
	}
	fclose(pFile);

	return pScene;
}

void CSaveLoadMgr::LoadScene() // 씬이동용
{
	m_bLoad = true;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, m_wcsPath.c_str(), L"rb");
	assert(pFile);

	CScene* pScene = new CScene;


	LoadResource(pFile);

	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		CLayer* pLayer = new CLayer;
		pScene->AddLayer(pLayer);
		LoadLayer(pLayer, pFile);
	}
	fclose(pFile);

	CSceneMgr::GetInst()->ChangeScene(pScene);
	m_wcsPath = L"";
}

void CSaveLoadMgr::LoadLayer(CLayer * _pLayer,FILE * _pFile)
{
	wstring wcsName = LoadWString(_pFile);
	_pLayer->SetName(wcsName);

	UINT iObjCount = 0;
	fread(&iObjCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iObjCount; ++i)
	{
		CGameObj* pObj = LoadGameObject(_pFile);
		_pLayer->AddGameObject(pObj);
	}
}

CGameObj* CSaveLoadMgr::LoadGameObject(FILE * _pFile)
{
	CGameObj* pObj = new CGameObj;
	pObj->Load(_pFile);

	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		wstring wcsScript = LoadWString(_pFile);
		
		CScript* pScript = CScriptMgr::GetScript(wcsScript);
		pScript->CComponent::Load(_pFile);
		pScript->Load(_pFile);
		pObj->AddComponent(pScript);
	}

	iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		CGameObj* pChild = LoadGameObject(_pFile);
		pObj->AddChild(pChild);
	}

	pObj->Start();

	return pObj;
}

//void CSaveLoadMgr::LoadingReset(CGameObj*& _pObj)
//{
//	if (m_bLoad)
//	{
//		if(_pObj == nullptr)
//			m_bLoad = false;
//		_pObj = nullptr;
//	}
//}

void CSaveLoadMgr::SavePrefab(const map<wstring, CRes*>& _mapPrefab, FILE * _pFile)
{
	if (nullptr == _pFile)
		assert(&_pFile);

	UINT iSize = (UINT)_mapPrefab.size();

	if (nullptr != _pFile)
	{
		fwrite(&iSize, sizeof(UINT), 1, _pFile);
		for (auto& pair : _mapPrefab)
		{
			SaveWString(pair.second->GetName(), _pFile);
			SaveGameObject(((CPrefab*)pair.second)->GetOriginalObj(), _pFile);
		}
	}
}

void CSaveLoadMgr::LoadPrefab(int _iCount, FILE * _pFile)
{
	if (nullptr == _pFile)
		return;

	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		wstring strKey = LoadWString(_pFile);
		CGameObj* pObj = LoadGameObject(_pFile);

		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(strKey);
		if (nullptr == pPrefab)
			CResMgr::GetInst()->AddResource<CPrefab>(strKey, new CPrefab(pObj));
		else
		{
			delete pObj;
			pObj = nullptr;
		}
	}
}

void CSaveLoadMgr::SaveResource(FILE * _pFile)
{
	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResource((RESOURCE_TYPE)i);

		// Resource 타입
		fwrite(&i, sizeof(UINT), 1, _pFile);

		// Resource 개수
		UINT iCount = (UINT)mapRes.size();
		fwrite(&iCount, sizeof(UINT), 1, _pFile);

		if ((UINT)RESOURCE_TYPE::PREFAB == i)
			SavePrefab(mapRes, _pFile);
		else
		{
			for (auto& pair : mapRes)
				pair.second->Save(_pFile);
		}
	}

	// Resource 타입
	UINT i = (UINT)RESOURCE_TYPE::END;
	fwrite(&i, sizeof(UINT), 1, _pFile);
}

void CSaveLoadMgr::LoadResource(FILE * _pFile)
{
	while (true)
	{
		RESOURCE_TYPE eResType = RESOURCE_TYPE::END;
		fread(&eResType, sizeof(UINT), 1, _pFile);

		if (eResType == RESOURCE_TYPE::END)
			break;

		int iCount = 0;
		fread(&iCount, sizeof(int), 1, _pFile);

		switch (eResType)
		{
		case RESOURCE_TYPE::MESH:
			break;
		case RESOURCE_TYPE::TEXTURE:
			for (int i = 0; i < iCount; ++i)
				CTexture::Load(_pFile);
			break;
		case RESOURCE_TYPE::MATERIAL:
			break;
		case RESOURCE_TYPE::PREFAB:
			LoadPrefab(iCount, _pFile);
			break;
		case RESOURCE_TYPE::SOUND:
			for (int i = 0; i < iCount; ++i)
				CSound::Load(_pFile);
			break;

		case RESOURCE_TYPE::MESHDATA:
			for (int i = 0; i < iCount; ++i)
				CMeshData::Load(_pFile);
			break;
		default:
			break;
		}
	}
}

void CSaveLoadMgr::SavePrefabObject(CGameObj * _pPrefabObj, FILE * _pFile)
{
	//_pPrefabObj->Save(_pFile);

	//// GameObject의 Script 정보 저장
	//vector<CScript*>& vecScript = _pPrefabObj->Script();
	//UINT iScriptCount = vecScript.size();
	//fwrite(&iScriptCount, sizeof(UINT), 1, _pFile);

	//for (UINT i = 0; i < vecScript.size(); ++i)
	//{
	//	// 스크립트 이름 저장
	//	SaveWString(CScriptMgr::GetScriptName(vecScript[i]), _pFile);

	//	// 스크립트 데이터 저장
	//	vecScript[i]->::CComponent::Save(_pFile);
	//	vecScript[i]->Save(_pFile);
	//}
	//// 자식들 저장
	//vector<CGameObject*>& vecChild = _pPrefabObj->GetChild();

	//UINT iChildCount = vecChild.size();
	//fwrite(&iChildCount, sizeof(UINT), 1, _pFile);

	//for (UINT i = 0; i < iChildCount; ++i)
	//{
	//	SaveGameObject(vecChild[i], _pFile);
	//}
}