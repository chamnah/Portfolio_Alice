#pragma once


// CAddAnim 대화 상자

class CAddAnim : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnim)

public:
	CAddAnim(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddAnim();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDANIM };
#endif

private:
	CEdit	   m_EditName;
	CString    m_strName;
	CString    m_strPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString&  GetName() { return m_strName; }
	CString&  GetPath() { return m_strPath; }

public:
	afx_msg void OnBnFindFolder();
};
