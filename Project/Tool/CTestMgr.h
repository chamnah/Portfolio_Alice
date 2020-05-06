#pragma once

class CGameObj;
class CTestMgr
{
private:
	CGameObj* m_pMouse;
	CGameObj* m_pTarget;
	COLL_TYPE m_eCollType;
	CGameObj* m_pGizmo;
	CGameObj* m_pGizmoXYZ[3];
	BoundingBox m_Box;
	bool      m_bPicking;
	CGameObj* m_pTargetGizmo;
	bool m_bStop;
public:
	void Init();
	void Update();
	void Render();
	void UpdateData();
	void SetTargetObj(CGameObj* _pTarget);
	CGameObj* GetTargetObj() { return m_pTarget; }
	CGameObj* GetMouseObj() { return m_pMouse; }
	void SetPicking(bool _bPick) { m_bPicking = _bPick; }
	void SetStop(bool _bStop) { m_bStop = _bStop; }
SINGLETON(CTestMgr)
};