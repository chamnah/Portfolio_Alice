#pragma once


// CDlgList 대화 상자

class CDlgList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgList)

public:
	CDlgList(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST };
#endif

private:
	CListCtrl m_DlgList;
	CString   m_strSelect;
	CString   m_strCaption;
	vector<CString> m_vecList;
	UINT      m_iIdx;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetCaption(const CString& _strCaption) { m_strCaption = _strCaption;}
	CString& GetSelect() { return m_strSelect; }
	UINT GetIndex() { return m_iIdx; }
	void AddData(const CString& _strData) { m_vecList.push_back(_strData); }

public:
	afx_msg void OnListDBClick(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};