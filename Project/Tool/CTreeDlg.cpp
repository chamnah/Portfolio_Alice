#include "stdafx.h"
#include "CTreeDlg.h"


CTreeDlg::CTreeDlg()
{
}

CTreeDlg::CTreeDlg(UINT nIDTemplate, CWnd * pParent)
	:CDialogEx(nIDTemplate,pParent)
	, m_Drag(nullptr)
{
}


CTreeDlg::~CTreeDlg()
{
}


void CTreeDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}

HTREEITEM CTreeDlg::AddItem(CString _strText, DWORD_PTR _pItem, HTREEITEM _Parent)
{
	TVINSERTSTRUCT st{};

	st.hInsertAfter = TVI_LAST;
	st.hParent = _Parent;
	st.item.pszText = _strText.GetBuffer();
	st.item.cchTextMax = (int)wcslen(st.item.pszText);
	st.item.mask = TVIF_TEXT;

	HTREEITEM item = m_ctrlTree.InsertItem(&st);
	m_ctrlTree.SetItemData(item, _pItem);

	return item;
}