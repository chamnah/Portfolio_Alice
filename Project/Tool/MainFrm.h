
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ChildView.h"
#include "CMySplitterWnd.h"
#include "CComView.h"
class CDockView;
class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	
// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	//CChildView    m_wndView;
	CMySplitterWnd    m_Splitter;
	//CMySplitterWnd    m_SubSplitter;
	//CComView		  m_ComView;
	//CAnimView		  m_AnimationView;
	CDockView*		  m_View[(UINT)VIEW_TYPE::END];

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	//CWnd* GetComponentView() { return  m_Splitter.GetPane(0, 2); }
	HWND GetGameHwnd() { return m_Splitter.GetPane(1, 0)->m_hWnd; }
	CWnd* GetGameView() { return m_Splitter.GetPane(1, 0); }
	CDockView* GetView(VIEW_TYPE _eType) { return m_View[(UINT)_eType]; }

	void Init();
	void Update();
	afx_msg void OnViewAnimation();
	afx_msg void OnDestroy();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnTileChagesize();
	afx_msg void OnSavePrefab();
	afx_msg void OnLoadPrefab();
	afx_msg void OnObjectAddfbxobj();
	afx_msg void OnObjectAddmeshdata();
};