// CScriptDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CScriptDlg.h"
#include "afxdialogex.h"
#include "CScriptMgr.h"
#include "CGameObj.h"
#include "CScript.h"
#define SHOW(a,b) b = #a 

// CScriptDlg 대화 상자

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_SCRIPT, pParent)
	, m_pCurScript(nullptr)
{

}

CScriptDlg::~CScriptDlg()
{
}

void CScriptDlg::Update(CGameObj* _pObj)
{
	m_pTargetObj = _pObj;

	if (m_pTargetObj == nullptr)
		return;
	if (m_pCurScript == nullptr)
		return;

	for (size_t i = 0; i < m_pCurScript->GetVecBool().size(); i++)
	{
		m_vecCheck[i]->SetCheck(m_pCurScript->GetVecBool()[i].bVal);
	}

	if (m_bFocus)
		return;
	CString strInt;
	for (size_t i = 0; i < m_pCurScript->GetVecInt().size(); i++)
	{
		int iInt = m_pCurScript->GetVecInt()[i].iVal;
		strInt.Format(L"%d",iInt);
		if(!m_vecInt.empty())
		m_vecInt[i]->SetWindowTextW(strInt);
	}
}

void CScriptDlg::Reset(CGameObj * _pObj)
{
	m_lbScript.ResetContent();
	const vector<CScript*>& vecScript = _pObj->GetScript();

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		wstring strName = CScriptMgr::GetScriptName(vecScript[i]);
		m_lbScript.AddString(strName.c_str());
	}

	m_cbScript.SetWindowTextW(L"");
	m_cbScript.SetCurSel(-1);
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbScript);
	DDX_Control(pDX, IDC_COMBO1, m_cbScript);

	for (size_t i = 0; i < m_vecCheck.size(); i++)
	{
		DDX_Control(pDX, IDC_CHECK1 + i, *(m_vecCheck[i]));
	}

	for (size_t i = 0; i < m_vecInt.size(); i++)
	{
		DDX_Control(pDX, IDC_EDIT1 + i, *(m_vecInt[i]));
	}
}

void CScriptDlg::Init()
{
	vector<wstring> vecScript;
	CScriptMgr::GetScriptInfo(vecScript);

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		m_cbScript.InsertString(i,vecScript[i].c_str());
	}

}

BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDlg::OnBnAddScript)
	ON_BN_CLICKED(IDC_BUTTON5, &CScriptDlg::OnBnDeleteScript)
	ON_LBN_SELCHANGE(IDC_LIST1, &CScriptDlg::OnLbnSelchangeList)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK1, IDC_CHECK1, &CScriptDlg::OnBnClicked)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT1, &CScriptDlg::OnEnChangeInt)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT1, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT1, &CComponentDlg::OnEditSetfocus)
END_MESSAGE_MAP()

void CScriptDlg::OnBnAddScript()
{
	if (m_pTargetObj == nullptr)
		return;

	if (m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;

	int iSel = m_cbScript.GetCurSel();
	if (iSel == LB_ERR)
		return;
	
	const vector<CScript*>& vecScript = m_pTargetObj->GetScript();

	CString Script;
	m_cbScript.GetLBText(iSel, Script);

	CScript* pCurScript = CScriptMgr::GetScript(Script.GetBuffer());

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == pCurScript->GetScriptType())
		{
			delete pCurScript;
			return;
		}
	}

	m_pTargetObj->AddComponent(pCurScript);
	pCurScript->Start();

	Reset(m_pTargetObj);
}

void CScriptDlg::OnBnDeleteScript()
{
	if (m_pTargetObj == nullptr)
		return;

	int iSel = m_lbScript.GetCurSel();
	if (iSel == LB_ERR)
		return;

	CString Script;
	m_lbScript.GetText(iSel, Script);

	vector<CScript*>& vecScript = m_pTargetObj->GetScript();
	vector<CScript*>::iterator iter =  vecScript.begin();
	
	for (; iter != vecScript.end(); ++iter)
	{
		wstring strName = CScriptMgr::GetScriptName(*iter);
		if (strName == Script.GetBuffer())
		{
			delete *iter;
			vecScript.erase(iter);
			m_pCurScript = nullptr;
			break;
		}
	} 

	Reset(m_pTargetObj);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CScriptDlg::OnLbnSelchangeList()
{
	m_pCurScript = nullptr;
	for (size_t i = 0; i < m_vecCheck.size(); i++)
	{
		m_vecCheck[i]->DestroyWindow();
		SAFE_DELETE(m_vecCheck[i]);
	}

	for (size_t i = 0; i < m_vecInt.size(); i++)
	{
		m_vecInt[i]->DestroyWindow();
		SAFE_DELETE(m_vecInt[i]);
	}

	m_vecCheck.clear();
	m_vecInt.clear();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pTargetObj == nullptr)
		return;

	int iSel = m_lbScript.GetCurSel();
	if (iSel == LB_ERR)
		return;

	CString Script;
	m_lbScript.GetText(iSel, Script);

	vector<CScript*>& vecScript = m_pTargetObj->GetScript();
	int iSortY = 0;
	for (size_t i = 0; i < vecScript.size(); i++)
	{
		wstring strName = CScriptMgr::GetScriptName(vecScript[i]);
		if (strName == Script.GetBuffer())
		{
			m_pCurScript = vecScript[i];
			vector<BoolInfo>& vecBool = vecScript[i]->GetVecBool();
			for (size_t j = 0; j < vecBool.size(); j++)
			{
				CButton* Button = new CButton;
				RECT rt = { 180,0 + iSortY * 50,300,50 + iSortY * 50 };
				Button->Create(vecBool[j].strName.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rt, this, IDC_CHECK1 + j);
				m_vecCheck.push_back(Button);
				iSortY++;
			}

			vector<IntInfo>& vecInt = vecScript[i]->GetVecInt();
			for (size_t j = 0; j < vecInt.size(); j++)
			{
				CEdit* Edit = new CEdit;
				RECT rt = { 180,10 + iSortY * 50,230,30 + iSortY * 50 };
				Edit->Create( WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,rt, this, IDC_EDIT1 + j);
				m_vecInt.push_back(Edit);
				iSortY++;
			}
			return;
		}
	}
}

void CScriptDlg::OnDestroy()
{
	CComponentDlg::OnDestroy();

	for (size_t i = 0; i < m_vecCheck.size(); i++)
	{
		m_vecCheck[i]->DestroyWindow();
		SAFE_DELETE(m_vecCheck[i]);
	}
	m_vecCheck.clear();


	for (size_t i = 0; i < m_vecInt.size(); i++)
	{
		m_vecInt[i]->DestroyWindow();
		SAFE_DELETE(m_vecInt[i]);
	}
	m_vecInt.clear();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CScriptDlg::OnBnClicked(UINT _id)
{
	for (size_t i = 0; i < m_vecCheck.size(); i++)
	{
		if (m_vecCheck[i]->GetDlgCtrlID() == _id)
		{
			int iCheck = m_vecCheck[i]->GetCheck();
			m_pCurScript->GetVecBool()[_id - IDC_CHECK1].bVal = (bool)iCheck;
			return;
		}
	}
}

void CScriptDlg::OnEnChangeInt(UINT _id)
{
	CString strInt;
	for (size_t i = 0; i < m_vecInt.size(); i++)
	{
		if (m_vecInt[i]->GetDlgCtrlID() == _id)
		{
			m_vecInt[i]->GetWindowTextW(strInt);
			int iInt = _wtoi(strInt);
			m_pCurScript->GetVecInt()[_id - IDC_EDIT1].iVal = iInt;
			return;
		}
	}
}