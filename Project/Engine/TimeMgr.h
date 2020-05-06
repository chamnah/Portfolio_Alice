#pragma once
#include "global.h"

class CTimeMgr
{
	SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER	m_llFrequency;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llOldCount;

	double			m_dt;	// 1프레임 시간
	UINT			m_iFPS;
	HDC				m_DC;
	class CGameObj* m_pFPSObj;
	class CGameObj* m_pInstObj;

public:
	float DeltaTime()
	{
		if (m_dt >= 0.1f)
			return 0.1f;
		return (float)m_dt;
	}

public:
	void init(HWND _hwnd);
	int update();
	void render();
	UINT GetFPS() { return m_iFPS; }
	void SetFPSObj(CGameObj* _pObj) { m_pFPSObj = _pObj; }
	void SetInstObj(CGameObj* _pObj) { m_pInstObj = _pObj; }
	CGameObj* GetInstObj() { return m_pInstObj; }
};