#pragma once
#include "global.h"

struct tKey
{
	KEY_TYPE	eType;
	KEY_STATE	eState;
};

class CKeyMgr
{
	SINGLETON(CKeyMgr);
private:
	vector<tKey>	m_vecKey;
	Vec3            m_vMousePos;
	Vec3			m_vWinMousePos;
	HWND			m_hWnd;
	Mtx             m_mtxMouse;

public:
	bool GetKeyState(KEY_TYPE _eType, KEY_STATE _eState)
	{
		if (_eState == m_vecKey[(UINT)_eType].eState)
			return true;
		else
			return false;
	};
	const Vec3& GetDXMousePos();
	void SetHandle(HWND _hWnd) { m_hWnd = _hWnd; }
	POINT TilePicking(){}
	Mtx& GetMtxMouse() 
	{ 
		return m_mtxMouse; 
	}
	Vec3& GetWindowMousePos() { return m_vWinMousePos; }

public:
	void init();
	void update();
};
