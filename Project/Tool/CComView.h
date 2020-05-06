#pragma once

#include "CDockView.h"

class CComponentDlg;
class CAddComponentDlg;
class CManageDlg;
class CLayerDlg;
class CPaintDlg;

enum class COMDLG_TYPE
{
	TRANSFORM,
	MESH_RENDER,
	CAMERA,
	COLLIDER,
	ANIMATOR_2D,
	ANIMATOR_3D,
	GRID,
	INCIDENT,
	LIGHT,
	TILE,
	//FILTER,
	TERRAIN,
	SCRIPT,
	TEXT,
	END,
};

class CComView : public CDockView
{
	DECLARE_DYNAMIC(CComView)
	
private:
	CComponentDlg*   m_arrComDlg[(UINT)COMDLG_TYPE::END];
	CAddComponentDlg* m_pAddCom;
	CManageDlg*      m_Scroll;
	CLayerDlg*       m_Layer;
	CPaintDlg*       m_pPaint;
	int              m_iOldScrollPos;
public:
	CComView();
	virtual ~CComView();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void Init();
	void Update();
	void DialogSort();
	CLayerDlg* GetLayerDlg() { return m_Layer; }
	CComponentDlg* GetCameraDlg() { return m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]; }
	void Reset();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
};