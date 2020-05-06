#pragma once
#include "CParent.h"
class CGameObj;
class CLayer;
class CCamera;
class CScene :
	public CParent
{
private:
	CLayer*     m_arrLayer[MAX_LAYER_COUNT];
	UINT		m_iCurLayerCount;
	//vector<CGameObj*> m_vecTileObj;
	CGameObj* pParent;
	bool      m_bStop;

public:
	void Awake();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	//void CreateTile(UINT _iCountX, UINT _iCountY, bool _bIsoTile = false);

public:
	void AddLayer(wstring _wcsName);
	void AddLayer(CLayer* _pLayer);
	void AddGameObject(wstring _wcsLayer, CGameObj* _pObj);
	void AddGameObject(UINT _iIdx, CGameObj* _pObj);

public:
	CLayer* GetLayer(int _iIdx);
	CLayer* GetLayer(const wstring& _wcsName);
	CGameObj* FindObject(const wstring& _wcsName);
	UINT  GetCurLayerCount() { return m_iCurLayerCount; }
	UINT GetLayerIndex(const wstring& _wcsName);
	const wstring& GetLayerName(UINT _iIdx);
	bool GetStop() { return m_bStop; }

	void SetStop(bool _bStop) { m_bStop = _bStop; }
	//vector<CCamera*>& GetCamera() { return m_vecCam; };

public:
	CScene();
	~CScene();
};