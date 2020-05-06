#pragma once


// CManageDlg 대화 상자

class CManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManageDlg)

public:
	CManageDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CManageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CScrollBar m_Scroll;
	int        m_iOldScroll;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	int GetScrollPosY() { return m_Scroll.GetScrollPos(); }
	void AddScrollPosY(int _iPos) 
	{
		m_iOldScroll = m_Scroll.GetScrollPos();
		m_Scroll.SetScrollPos(m_iOldScroll + _iPos);
	}
	CScrollBar& GetScrollBar() { return m_Scroll; }
	void SetOldPos(int _iOldPos) { m_iOldScroll = _iOldPos; }
	int GetOldPos() { return m_iOldScroll; }
};