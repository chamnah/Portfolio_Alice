#pragma once
class CGameObj;
class CDockView : public CDockablePane
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;

protected:
	CGameObj*		 m_pTargetObj;

public:
	CDockView();
	~CDockView();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void SetTargetObj(CGameObj* _pObj) 
	{ 
		m_pTargetObj = _pObj; 
	}

};