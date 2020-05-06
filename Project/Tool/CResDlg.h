#pragma once

#include "CTreeDlg.h"

class CResDlg : public CTreeDlg
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CResDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESDLG };
#endif

private:
	bool     m_bDrag;

public:
	void Init();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void DataLoad(RESOURCE_TYPE _eType, wstring& _Path);
	void CreateNewObject();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDBClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBeginDragTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void Reset();
	afx_msg void OnResourceDelete();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCreateObject();
};
