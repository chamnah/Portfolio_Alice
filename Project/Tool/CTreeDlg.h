#pragma once
class CTreeDlg :
	public CDialogEx
{
protected:
	CTreeCtrl m_ctrlTree;
	HTREEITEM m_Drag;
protected:
	void Init() {}
	HTREEITEM AddItem(CString _strText, DWORD_PTR _pItem, HTREEITEM _Parent = nullptr);
public:
	CTreeDlg();
	CTreeDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	~CTreeDlg();
	virtual void PostNcDestroy();
	HWND GetTreeDlgHwnd() { return m_ctrlTree.GetSafeHwnd(); }
};