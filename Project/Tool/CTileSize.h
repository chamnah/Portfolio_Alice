#pragma once


// CTileSize 대화 상자

class CTileSize : public CDialogEx
{
	DECLARE_DYNAMIC(CTileSize)

public:
	CTileSize(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTileSize();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILE_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CString m_strX;
	CString m_strY;
	CEdit	m_editSizeX;
	CEdit	m_editSizeY;

public:
	POINT GetSize();
	virtual void OnOK();
};
