#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "CConstBuffer.h"
#include "CDirect.h"
#include "CTexture.h"
#include "CResMgr.h"

CAnimator2D::CAnimator2D()
	:m_bRepeat(true)
	, m_pCurAnim(nullptr)
	, m_bPlay(true)
	, m_wcsTemp(L"")
	, m_bReset(false)
	, m_wcsPath(L"")

{	
	m_eType = COMPONENT_TYPE::ANIMATION_2D;
}

CAnimator2D::CAnimator2D(const CAnimator2D & _Anim)
{
	*this = _Anim;

	for (auto& mapAnim : m_mapAnim)
	{
	 	CAnimation2D* pAnim = CAnimation2D::Create(mapAnim.second->GetPath(), mapAnim.first,Vec2(),0.02f);

		pAnim->GetAllFrame() = mapAnim.second->GetAllFrame();
		pAnim->SetMoveFrm(0);

		mapAnim.second = pAnim;
	}

	m_pCurAnim = nullptr; // 현재 이녀석이 계속 Null로 나온다.
}

	
CAnimator2D::~CAnimator2D()
{	
	//ClearAnimation();
	SafeMapDelete(m_mapAnim);
}	
	
int CAnimator2D::Update()
{
	if (m_pCurAnim != nullptr && m_bPlay)
	{
		m_pCurAnim->Update();

		if (m_pCurAnim->GetFinish() && m_bRepeat)
		{
			m_pCurAnim->SetMoveFrm(0);
		}
	}
	return 0;
}

void CAnimator2D::UpdateData()
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->UpdateData();
}

void CAnimator2D::AddAnimation(CTexture* _pTex, const Vec2 & _vLT, const Vec2 & _vSize, UINT _iMaxFrm, const wstring & _wcsKey)
{	
	if (m_mapAnim.find(_wcsKey) != m_mapAnim.end())
		assert(NULL);

	CAnimation2D* pAnim = CAnimation2D::Create(_pTex,_vLT,_vSize,_iMaxFrm,_wcsKey);

	CResMgr::GetInst()->AddResource<CTexture>(_wcsKey,_pTex);
	m_mapAnim.insert(make_pair(_wcsKey,pAnim));

	m_bReset = true;
}

CAnimation2D* CAnimator2D::AddAnimation(const wstring & _wcsPath, const wstring & _wcsKey, const Vec2& _vOffset, float _fDuration) // bin으로부터 상대경로임 
{
	if (m_mapAnim.find(_wcsKey) != m_mapAnim.end())
		assert(NULL);

	CAnimation2D* pAnim = CAnimation2D::Create(_wcsPath, _wcsKey, _vOffset, _fDuration);

	pAnim->SetOwner(GetOwner());
	m_mapAnim.insert(make_pair(_wcsKey, pAnim));
	m_bReset = true;

	return pAnim;
}

void CAnimator2D::RemoveAnimation(const wstring & _wcsKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_wcsKey);

	if (iter == m_mapAnim.end())
		assert(NULL);

	m_pCurAnim = nullptr;

	delete iter->second;
	m_mapAnim.erase(iter);

	m_bReset = true;
}

void CAnimator2D::PlayAnim(const wstring & _wcsKey, bool _bRepeat)
{
	map<wstring,CAnimation2D*>::iterator iter = m_mapAnim.find(_wcsKey);

	if (iter == m_mapAnim.end())
		assert(NULL);

	m_pCurAnim = iter->second;
	m_bRepeat = _bRepeat;
}

void CAnimator2D::ClearData()
{
	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"AnimParam");
	pBuffer->ClearData();
}

bool CAnimator2D::GetAnimFinish()
{
	if (m_pCurAnim != nullptr)
		return m_pCurAnim->GetFinish();
	else
		return true;
}

const wstring & CAnimator2D::GetCurKey()
{
	if (m_pCurAnim != nullptr)
		return m_pCurAnim->GetName();
	else
		return m_wcsTemp;
}

void CAnimator2D::AnimReset()
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->SetMoveFrm(0);
}

void CAnimator2D::SetOffset(Vec2& _vOffset)
{
	if (m_pCurAnim != nullptr)
		m_pCurAnim->GetCurFrame().vOffset = _vOffset;
}

Vec2 CAnimator2D::GetOffset()
{
	if (m_pCurAnim == nullptr)
		return Vec2();

	return m_pCurAnim->GetCurFrame().vOffset;
}

void CAnimator2D::SetDurationTime(float _fDuration)
{
	if (m_pCurAnim != nullptr)
	m_pCurAnim->SetDurationTime(_fDuration);
}

void CAnimator2D::ClearAnimation()
{
	if (m_mapAnim.size() > 0)
	{
		map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.begin();
		for (; iter != m_mapAnim.end();)
		{
			if (iter->second == nullptr)
				continue;

			/*vector<tAnim>& vecFrm = iter->second->GetAllFrame();

			for (UINT i = 0; i < vecFrm.size(); ++i)
			{
				CResMgr::GetInst()->RemoveResource(vecFrm[i].pTex->GetName(),RESOURCE_TYPE::TEXTURE);
			}*/

			delete iter->second;
			iter->second = nullptr;
			iter = m_mapAnim.erase(iter);
		}
	}
}

UINT CAnimator2D::GetCurFrame()
{
	return m_pCurAnim->GetFrame();
}

void CAnimator2D::SetMoveFrm(UINT _iFrm)
{
	m_pCurAnim->SetMoveFrm(_iFrm);
}

CTexture * CAnimator2D::GetCurAnimTexture()
{
	if (m_pCurAnim == nullptr)
		return nullptr;
	return m_pCurAnim->GetCurFrame().pTex;
}

void CAnimator2D::SetAnimSize(float _fSize)
{
	vector<tAnim>& vecAnim = m_pCurAnim->GetAllFrame();

	for (UINT i = 0; i < vecAnim.size(); ++i)
	{
		vecAnim[i].vSize.x = _fSize;
	}
}

void CAnimator2D::SetAutoSize(bool _bSize)
{
	m_pCurAnim->SetAutoSize(_bSize);
}

void CAnimator2D::SetCurFrmCutSize(Vec4 & _vSize)
{
	m_pCurAnim->SetCurFrmCutSize(_vSize);
}

void CAnimator2D::AnimSetOwner()
{
	for (auto& mapAnim : m_mapAnim)
	{
		mapAnim.second->SetOwner(GetOwner());
	}
}

void CAnimator2D::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);
	SaveWString(m_wcsPath,_pFile);
}

void CAnimator2D::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	m_wcsPath = LoadWString(_pFile);

	if (m_wcsPath == L"")
		return;

	FILE* pFile = NULL;
	_wfopen_s(&pFile, m_wcsPath.c_str(), L"rb");

	int iClipSize = 0;
	fread(&iClipSize, sizeof(int), 1, pFile);

	int iNameLength = 0;
	int iPathLength = 0;

	wstring wcsPath;
	wstring wcsName;

	Vec2 vOffset = {};
	float fDuration = 0.f;
	int iFrmSize = 0;
	Vec2 vLT;
	Vec2 vSize;

	ClearAnimation();

	for (int i = 0; i < iClipSize; ++i)
	{
		wchar_t wcsName[MAX_PATH] = L"";
		wchar_t wcsPath[MAX_PATH] = L"";

		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(&wcsName, sizeof(wchar_t), iNameLength, pFile);

		fread(&iPathLength, sizeof(int), 1, pFile);
		fread(&wcsPath, sizeof(wchar_t), iPathLength, pFile);

		fread(&iFrmSize, sizeof(int), 1, pFile);

		CAnimation2D* pAnim = AddAnimation(wcsPath, wcsName);
		vector<tAnim>& vecFrm = pAnim->GetAllFrame();

		for (int i = 0; i < iFrmSize; ++i)
		{
			fread(&vOffset, sizeof(Vec2), 1, pFile);
			fread(&fDuration, sizeof(float), 1, pFile);
			fread(&vLT, sizeof(Vec2), 1, pFile);
			fread(&vSize, sizeof(Vec2), 1, pFile);

			vecFrm[i].vOffset = vOffset;
			vecFrm[i].fDuration = fDuration;
		}
	}

	fclose(pFile);

	IsPlay(true);
	ChangeAnimation(nullptr);
}