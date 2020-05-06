#include "CAnimation2D.h"
#include "CTexture.h"
#include "TimeMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CResMgr.h"
#include "CGameObj.h"
#include "CTransForm.h"

CAnimation2D::CAnimation2D()
	:m_fAccTime(0.f)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_bAutoSize(true)
{
}

CAnimation2D::~CAnimation2D()
{
}

CAnimation2D* CAnimation2D::Create(CTexture* _pTex, const Vec2 & _vLT, const Vec2 & _vSize, UINT iMaxFrm, const wstring & _wcsKey)
{
	CAnimation2D* pAnim = new CAnimation2D;
	tAnim frm = {};

	for (UINT i = 0; i < iMaxFrm; ++i)
	{
		frm.pTex = _pTex;
		frm.vLT = Vec2((_vLT.x / (float)_pTex->GetWidth()) + i * (_vSize.x / (float)_pTex->GetWidth()), _vLT.y / (float)_pTex->GetHeight());
		frm.vSize = Vec2(_vSize.x / (float)_pTex->GetWidth(), _vSize.y / (float)_pTex->GetHeight());
		frm.fDuration = 0.2f;

		pAnim->m_vecAnim.push_back(frm);
	}

	pAnim->SetName(_wcsKey);

	return pAnim;
}

CAnimation2D * CAnimation2D::Create(const wstring & _wcsFolderPath, const wstring & _wcsKey, const Vec2& _vOffset, float _fDuration)
{
	WIN32_FIND_DATA data = {};
	wstring wcsPath = _wcsFolderPath + L"*.png";
	HANDLE hFind = FindFirstFile(wcsPath.c_str(), &data);
	
	CAnimation2D* pAnim = new CAnimation2D;
	tAnim frm = {};

	if (hFind)
	{
		while (true)
		{
			frm.vLT = Vec2(0.f, 0.f);
			frm.vSize = Vec2(1.f, 1.f);
			frm.vOffset = _vOffset;
			frm.fDuration = _fDuration;
			CResMgr::GetInst()->AddResource<CTexture>(_wcsFolderPath + data.cFileName, CTexture::CreateTexture(_wcsFolderPath + data.cFileName));
			frm.pTex = CResMgr::GetInst()->Load<CTexture>(_wcsFolderPath + data.cFileName);
			pAnim->m_vecAnim.push_back(frm);

			if (!FindNextFile(hFind, &data))
				break;
		}
	}

	FindClose(hFind);

	pAnim->SetName(_wcsKey);
	pAnim->m_wcsPath = _wcsFolderPath;

	return pAnim;
}

int CAnimation2D::Update()
{
	m_fAccTime += DT;

	if (m_vecAnim[m_iCurFrm].fDuration <= m_fAccTime)
	{
		while (true)
		{
			m_fAccTime -= m_vecAnim[m_iCurFrm].fDuration;
			++m_iCurFrm;
		
			if (m_iCurFrm >= m_vecAnim.size())
			{
				m_bFinish = true;
				m_iCurFrm = (UINT)(m_vecAnim.size() - 1);
				break;
			}
			else if (m_fAccTime < m_vecAnim[m_iCurFrm].fDuration)
				break;
		}
	}

	if (m_bAutoSize)
	{
		float fX = (float)m_vecAnim[m_iCurFrm].pTex->GetWidth();
		float fY = (float)m_vecAnim[m_iCurFrm].pTex->GetHeight();

		if (m_pOwner != nullptr)
			m_pOwner->GetTransForm()->SetScale(Vec3(fX, fY, 1.f));
	}
	return 0;
}

void CAnimation2D::UpdateData()
{
	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"AnimParam");

	tAnimParam tParam = {};
	tParam.g_fLT = m_vecAnim[m_iCurFrm].vLT;
	tParam.g_fSize = m_vecAnim[m_iCurFrm].vSize;
	tParam.g_iUse = true;
	tParam.g_vOffset = m_vecAnim[m_iCurFrm].vOffset;
	pBuffer->UpdateData(&tParam,sizeof(tParam));
	pBuffer->UpdateRegisterAll();
	m_vecAnim[m_iCurFrm].pTex->SetRegisterAll(15);
}

void CAnimation2D::SetMoveFrm(UINT _iFrm)
{
	if (m_vecAnim.size() <= _iFrm)
		assert(NULL);

	m_iCurFrm = _iFrm;
	m_bFinish = false;

	if (m_bAutoSize)
	{
		float fX = (float)m_vecAnim[m_iCurFrm].pTex->GetWidth();
		float fY = (float)m_vecAnim[m_iCurFrm].pTex->GetHeight();

		if (m_pOwner != nullptr)
			m_pOwner->GetTransForm()->SetScale(Vec3(fX, fY, 1.f));
	}
}

void CAnimation2D::SetCurFrmCutSize(Vec4 & _vSize)
{
	m_vecAnim[m_iCurFrm].vLT.x = _vSize.x / (float)m_vecAnim[m_iCurFrm].pTex->GetWidth();
	m_vecAnim[m_iCurFrm].vLT.y = _vSize.y / (float)m_vecAnim[m_iCurFrm].pTex->GetHeight();
	
	m_vecAnim[m_iCurFrm].vSize.x = _vSize.z / (float)m_vecAnim[m_iCurFrm].pTex->GetWidth();;
	m_vecAnim[m_iCurFrm].vSize.y = _vSize.w / (float)m_vecAnim[m_iCurFrm].pTex->GetHeight();;
}

void CAnimation2D::SetDurationTime(float _fDuration)
{ 
	for(UINT i = 0; i < m_vecAnim.size(); ++i)
		m_vecAnim[i].fDuration = _fDuration; 
}