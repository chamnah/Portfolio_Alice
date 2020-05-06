#pragma once
// CFilterComDlg 대화 상자

class CFilterComDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterComDlg)

public:
	CFilterComDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFilterComDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTERCOMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
};