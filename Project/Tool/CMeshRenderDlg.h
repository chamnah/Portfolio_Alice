#pragma once

#include "CComponentDlg.h"

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshRenderDlg();

public:
	void Update(CGameObj* _pObj);
	void Init();
	virtual void Reset(CGameObj* _pObj);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMesh();
	afx_msg void OnBnClickedMaterial();
	afx_msg void OnBnAddTexture();
	afx_msg void OnBnClickedBlend();
private:
	CEdit m_Blend;
public:
	afx_msg void OnBnClickedDepthStencil();
	afx_msg void OnAddTexture2();

private:
	CEdit m_Tex1;
	CEdit m_Tex2;
	CEdit m_EditMesh;
	CEdit m_EditMaterial;
	CComboBox m_cbSubset;
	CEdit m_editDiff[4];
public:
	void Sort();
	afx_msg void OnBnClickedButtonRasterize();
	CEdit m_editRasterize;
private:
	CButton m_checkShadow;
	CEdit m_editDepth;
public:
	afx_msg void OnEnChangeDiffuse(UINT _id);
	afx_msg void OnBnCheckShadow();
	CComboBox m_cbRenderType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeRenderType();
	CEdit m_editUV[2];
	afx_msg void OnEnChangeUV(UINT _iId);
	afx_msg void OnBnClickedSampler();
private:
	CEdit m_editSampler;
};