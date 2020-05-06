#pragma once

#include "CComponentDlg.h"
// CTerrainDlg 대화 상자

class CTerrainDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTerrainDlg)

public:
	CTerrainDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTerrainDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TerrainDlg };
#endif
public:
	virtual void Update(CGameObj* _pObj);

public:
	void StringSelCut(CString& _strSel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl  m_listBrush;
	CImageList m_imglist;
	int        m_iChangeIndex;
	CString    m_strName;
	int        m_iOldIndex;
	CString    m_strOldName;
	bool       m_bFirst;
	CString    m_strSel;
	bool       m_bIng;
	bool       m_TexLoad;
	CComboBox m_cbBrush;
	class CWBrushDlg* m_pWBrush;
	class CHBrushDlg* m_pHBrush;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchange();
	CEdit m_editBrushSize;
	afx_msg void OnEnChangeBrushSize();
private:
	CTabCtrl m_tabTerrain;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editMin;
	CEdit m_editMax;
	afx_msg void OnEnChangeRange(UINT _iId);
};