#pragma once

#include "global.h"
class CGameObj;
class CEventMgr
{
	SINGLETON(CEventMgr);
private:
	vector<tEvent>    m_vecEvent;
	vector<CGameObj*> m_vecDel;
	bool              m_bReset;
	bool              m_bChangeScene;
	wstring           m_strScene;

public:
	void Init();
	int Update();

public:
	void AddEvent(const tEvent& _event);
	void Event(tEvent& _event);
	bool IsReset() { return m_bReset; }
	void SetReset(bool _bReset) { m_bReset = _bReset; }
	bool GetChangeScene() { return m_bChangeScene; }
	void OffChangeScene() { m_bChangeScene = false; m_strScene = L""; }
	wstring& GetSceneName() { return m_strScene; }
};