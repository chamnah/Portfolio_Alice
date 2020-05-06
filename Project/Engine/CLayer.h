#pragma once
#include "CParent.h"

class CGameObj;
class CLayer :
	public CParent
{
private:
	vector<CGameObj*> m_vecObj;
	vector<CGameObj*> m_vecAllObj;
	int     m_iLayerID;

public:
	void Awake();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();
	void AddGameObject(CGameObj* _pObj);
	void PopGameObject(CGameObj* _pObj);
public:
	void SetID(int _iID) { m_iLayerID = _iID;}
	int  GetID() { return m_iLayerID; }
	vector<CGameObj*>& GetGameObj() { return m_vecObj; }
	void SetAllObj(CGameObj* _pObj) { m_vecAllObj.push_back(_pObj); }
	vector<CGameObj*>& GetAllObj() { return m_vecAllObj; }

public:
	CLayer();
	~CLayer();
};