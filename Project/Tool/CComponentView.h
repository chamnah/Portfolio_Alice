#pragma once


// CComponentView 보기
class CGameObj;
class CComponentDlg;
class CComponentView : public CFormView
{
	DECLARE_DYNCREATE(CComponentView)

private:
	CComponentDlg*   m_arrComDlg[(UINT)COMPONENT_TYPE::END];
	CGameObj*		 m_pTargetObj;

protected:
	CComponentView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CComponentView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

public:
	void Init();
	void Update();
	void DialogSort();
};