#pragma once

#include "CDockView.h"
// CAnimView
class CAnimationDlg;
class CGameObj;
class CAnimView : public CDockView
{
	DECLARE_DYNAMIC(CAnimView)

private:
	CAnimationDlg* m_Anim;

public:
	void Init();
	void Update();

public:
	CAnimView();
	virtual ~CAnimView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};