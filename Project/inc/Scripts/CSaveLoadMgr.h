#pragma once
#include "global.h"
class CLayer;
class CGameObj;
class CRes;
class CScene;
class CSaveLoadMgr
{
private:
	static bool m_bLoad;
	static bool m_bLoadScene;
	static bool m_bChangeScene;
	static wstring m_wcsPath;
	static bool m_bClient;
	static CScene* m_pScene;

public:
	static void SaveScene(const wstring& _wcsPath);
	static void SaveLayer(CLayer* _pLayer, FILE* _pFile);
	static void SaveGameObject(CGameObj* _pObj, FILE* _pFile);
	static void SaveResource(FILE* _pFile);
	static void LoadScene(const wstring& _wcsPath);
	static CScene* LoadSceneNoChange(const wstring& _wcsPath);
	static void LoadScene();
	static void LoadLayer(CLayer * _pLayer, FILE* _pFile);
	static CGameObj* LoadGameObject(FILE* _pFile);
	static void LoadResource(FILE* _pFile);
	static void SetLoading(bool _bLoad) { m_bLoad = _bLoad; }
	static bool GetLoading() { return m_bLoad; }
	//static void LoadingReset(CGameObj*& _pObj);
	static void SavePrefab(const map<wstring, CRes*>& _mapPrefab, FILE * _pFile);
	static void LoadPrefab(int _iCount, FILE * _pFile);
	static void SavePrefabObject(CGameObj * _pPrefabObj, FILE * _pFile);
	static bool IsLoadScene() { return m_bLoadScene; }
	static void SetLoadScene(bool _bLoad) { m_bLoadScene = _bLoad; }
	static bool IsChangeScene() { return m_bChangeScene; }
	static void SetChangeScene(bool _bChange) { m_bChangeScene = _bChange; }
	static void SetPath(wstring _wcsPath) { m_wcsPath = _wcsPath; }
	static void SetClient(bool _bClient) { m_bClient = _bClient;}
	static void SetScene(CScene* _pScene) {	m_pScene = _pScene;	}
	static CScene* GetScene() { return m_pScene; }
};