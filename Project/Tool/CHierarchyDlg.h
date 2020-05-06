#pragma once

#include "CTreeDlg.h"
// CHierarchyDlg 대화 상자
class CHierarchyDlg : public CTreeDlg
{
	DECLARE_DYNAMIC(CHierarchyDlg)

public:
	CHierarchyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHierarchyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHY };
#endif

private:
	HTREEITEM m_Target;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void ClickedAddObject();
	afx_msg void OnObjectDelete();

public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

public:
	void Init();
	void Reset();
	void AddGameObject(CString _strText, DWORD_PTR _pItem, HTREEITEM _Parent = nullptr);
	void MoveItem(HTREEITEM _Drag, HTREEITEM _Target);
	void GetChildItem(vector<HTREEITEM>& _vecChild, HTREEITEM _item);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnObjectPrefab();
	afx_msg void OnObjCopy();
};