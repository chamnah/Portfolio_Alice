#pragma once

#include "CDockView.h"
class CHierarchyDlg;
class CResDlg;
class CHierachyView : public CDockView
{
	DECLARE_DYNAMIC(CHierachyView)


protected:
	CHierarchyDlg*   m_pHierarchy;
	CResDlg*         m_pRes;

public:
	virtual void Init();
	virtual void Update();

public:
	CHierarchyDlg* GetHirerarchy() { return m_pHierarchy; }
	CResDlg* GetRes() { return m_pRes; }

public:
	CHierachyView();
	virtual ~CHierachyView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};