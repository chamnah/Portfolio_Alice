#include "CAnimator3D.h"
#include "TimeMgr.h"
#include "CTexture.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CAnimation3D.h"
#include "CMesh.h"
#include "CRenderMgr.h"
#include "KeyMgr.h"

CAnimator3D::CAnimator3D()
	:m_iCurClip(0)
	, m_iFrameCount(30)
	, m_pCurAnim(nullptr)
	, m_bBegin(true)
	, m_bRepeat(false)
	, m_bFinish(false)
	, m_iAnimCenter(-1)
	, m_bChange(false)
	, m_pChangeAnim(nullptr)
	, m_iFrmStart(0)
	, m_iCurFrm(0)
{
	m_eType = COMPONENT_TYPE::ANIMATION_3D;
}

CAnimator3D::CAnimator3D(const CAnimator3D & _Anim)
{
	*this = _Anim;

	for (auto& mapAnim : m_mapAnim3D)
	{
		tAnim3D* pAnim = new tAnim3D;
		map<wstring, tAnim3D*>::const_iterator iter = _Anim.m_mapAnim3D.find(mapAnim.first);
		*pAnim = *iter->second;
		mapAnim.second = pAnim;
	}

	m_pCurAnim = nullptr;


	for (size_t i = 0; i < m_pVecBone->size(); i++)
	{
		if (m_pVecBone->at(i).strName == L"RefBox")
		{
			m_iAnimCenter = i;
		}
	}
}

CAnimator3D::~CAnimator3D()
{
	if(m_mapAnim3D.size() != 0)
		SafeMapDelete(m_mapAnim3D);
}

void CAnimator3D::SetAnimClip(vector<tMtxAnimClip>* _pVecAnimClip)
{
	m_pVecAnimClip = _pVecAnimClip;
	m_vecClipTime.resize(m_pVecAnimClip->size());
}

void CAnimator3D::SetBone(vector<tMtxBone>* _pVecBone)
{
	m_pVecBone = _pVecBone;

	for (size_t i = 0; i < m_pVecBone->size(); i++)
	{
		if (m_pVecBone->at(i).strName == L"RefBox")
		{
			m_iAnimCenter = i;
		}
	}
	
	m_vecFinalBoneMtx.resize(m_pVecBone->size());
	m_vecWorldBoneMtx.resize(m_pVecBone->size());
}

int CAnimator3D::Update()
{
	if (m_pCurAnim == nullptr)
	{
		m_iCurFrm = 0;
		return 0;
	}
	m_fCurTime += DT;
	m_bFinish = false;

	// 1152, 1168
	m_iCurFrm = (int)(m_fCurTime * m_iFrameCount);
	int iFrmIndex = m_iCurFrm + m_pCurAnim->iStartFrm;
	if (iFrmIndex > m_pCurAnim->iEndFrm)
	{
		if (m_bRepeat)
		{
			m_fCurTime = 0.f;
			iFrmIndex = m_pCurAnim->iStartFrm;
		}
		else
		{
			iFrmIndex = m_pCurAnim->iEndFrm;
			m_bFinish = true;
		}
	}

	int iFrmNextIndex = 0;

	if (iFrmIndex == m_pCurAnim->iEndFrm)
	{
		if(m_bRepeat == true)
			iFrmNextIndex = m_pCurAnim->iStartFrm;
		else
			iFrmNextIndex = m_pCurAnim->iEndFrm;
	}
	else
		iFrmNextIndex = iFrmIndex + 1;



	for (size_t i = 0; i < m_pVecBone->size(); i++)
	{
		if (m_pVecBone->at(i).vecKeyFrm.empty())
		{
			m_vecFinalBoneMtx[i] = m_pVecBone->at(i).TransForm;
			continue;
		}

		tMtxKeyFrm& CurFrm = m_pVecBone->at(i).vecKeyFrm[iFrmIndex];
		tMtxKeyFrm& NextFrm = m_pVecBone->at(i).vecKeyFrm[iFrmNextIndex];

		float fPercent = (m_fCurTime + m_pVecBone->at(i).vecKeyFrm[m_pCurAnim->iStartFrm].fTime - CurFrm.fTime) / ( 1.f / m_iFrameCount);

		Vec3 vScale1 = CurFrm.vScale;
		Vec3 vScale2 = NextFrm.vScale;

		Vec3 vTrans1 = CurFrm.vTranslate;
		Vec3 vTrans2 = NextFrm.vTranslate;

		Vec4 vQRot1 = XMLoadFloat4(&CurFrm.vQRot);
		Vec4 vQRot2 = XMLoadFloat4(&NextFrm.vQRot);

		XMVECTOR vScale = XMVectorLerp(vScale1,vScale2,fPercent);
		XMVECTOR vTrans = XMVectorLerp(vTrans1, vTrans2, fPercent);
		XMVECTOR vQRot = XMQuaternionSlerp(vQRot1, vQRot2, fPercent);
		XMVECTOR vCenter = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		if (m_iAnimCenter == i)
		{
			m_vCenterPos = vTrans;
		}
		vTrans.m128_f32[2] -= m_vCenterPos.z;
		m_vecWorldBoneMtx[i] = XMMatrixAffineTransformation(vScale, vCenter, vQRot, vTrans);
		m_vecFinalBoneMtx[i] = m_pVecBone->at(i).OffsetMtx * XMMatrixAffineTransformation(vScale, vCenter,vQRot,vTrans);
	}

	return 0;
}

void CAnimator3D::UpdateData()
{
	m_pBoneTex->SetData(&m_vecFinalBoneMtx[0],m_vecFinalBoneMtx.size() * sizeof(Mtx));
	vector<CMaterial*>& vecMtrl = MeshRender()->GetVecMtrl();

	for (size_t i = 0; i < vecMtrl.size(); i++)
	{
		if (vecMtrl[i] == nullptr)
			continue;
		vecMtrl[i]->SetParam(m_pBoneTex,SHADER_PARAM::TEX_7);
	}
}

void CAnimator3D::AddAnimation(const wstring _strKey,int _iStartFrm, int _iEndFrm)
{
	tAnim3D* Anim = new tAnim3D;
	Anim->iStartFrm = _iStartFrm;
	Anim->iEndFrm = _iEndFrm;
	Anim->iFrmLength = _iEndFrm + 1 - _iStartFrm;
	m_mapAnim3D.insert(make_pair(_strKey, Anim));
	
	m_pCurAnim = Anim;
	m_pCurAnim->strKey = _strKey;
	//CAnimation3D* pAnim = new CAnimation3D(_iStartFrm,_iEndFrm,m_pVecBone,&m_iFrameCount);
	//m_mapAnim3D.insert(make_pair(_strKey, pAnim));
}

bool CAnimator3D::AddAnimation(const wstring _strKey, int _iFrmLength)
{
	if (m_iFrmStart + _iFrmLength - 1 > m_pVecAnimClip->at(0).iEndFrm)
	{
		MessageBox(CRenderMgr::GetInst()->GetGameHwnd(), L"애니메이션 Frame 범위를 초과하였습니다.", L"오류", MB_OK);
		return false;
	}

	tAnim3D* Anim = new tAnim3D;
	Anim->iStartFrm = m_iFrmStart;
	Anim->iEndFrm = m_iFrmStart + _iFrmLength - 1;
	m_iFrmStart += _iFrmLength;
	Anim->iFrmLength = _iFrmLength;
	m_mapAnim3D.insert(make_pair(_strKey, Anim));

	m_pCurAnim = Anim;
	m_pCurAnim->strKey = _strKey;
	return true;
}

void CAnimator3D::DeleteAnimation(const wstring _strKey)
{
	map<wstring, tAnim3D*>::iterator iter = m_mapAnim3D.begin();
	for (; iter != m_mapAnim3D.end();)
	{
		if (iter->first == _strKey)
		{
			//m_iFrmStart -= iter->second->iFrmLength;
			delete iter->second;
			iter = m_mapAnim3D.erase(iter);
			m_pCurAnim = nullptr;
			break;
		}
		else
			++iter;
	}
}

void CAnimator3D::PlayAnim(wstring _strKey,bool _bRepeat,bool _bReset)
{
	map<wstring, tAnim3D*>::iterator iter = m_mapAnim3D.find(_strKey);
	if (iter == m_mapAnim3D.end())
		assert(NULL);

	if (m_pCurAnim != iter->second || _bReset)
	{
		m_bRepeat = _bRepeat;

		m_bFinish = false;
		m_bChange = true;
		m_fCurTime = 0.f;
		m_pChangeAnim = m_pCurAnim;
		m_pCurAnim = iter->second;
		m_pCurAnim->strKey = _strKey;
	}
}

void CAnimator3D::NextAnim()
{
	if (m_bBegin)
	{
		m_bBegin = false;
		m_NextAnim = m_mapAnim3D.begin();

	}
	else
	{
		m_NextAnim++;
		if (m_NextAnim == m_mapAnim3D.end())
		{
			m_NextAnim = m_mapAnim3D.begin();
		}
	}

	m_fCurTime = 0.f;
	m_pCurAnim = m_NextAnim->second;
}

bool CAnimator3D::GetCurAnimKey(wstring _strKey)
{
	if (m_pCurAnim == nullptr)
		return false;
	
	if(m_pCurAnim->strKey == _strKey)
		return true;

	return false;
}

void CAnimator3D::ChangeText(wstring _strKey,wstring _strUpdate)
{
	map<wstring, tAnim3D*>::iterator iter = m_mapAnim3D.find(_strKey);
	if (iter == m_mapAnim3D.end())
		assert(NULL);
	tAnim3D* anim = new tAnim3D;
	*anim =  *iter->second;
	anim->strKey = _strUpdate;
	delete iter->second;
	m_mapAnim3D.erase(iter);
	m_mapAnim3D.insert(make_pair(anim->strKey,anim));
}

void CAnimator3D::DeleteAll()
{
	map<wstring, tAnim3D*>::iterator iter = m_mapAnim3D.begin();
	for (; iter != m_mapAnim3D.end();)
	{
		delete iter->second;
		iter = m_mapAnim3D.erase(iter);
		m_iFrmStart = 0;
		break;
	}
}

void CAnimator3D::Save(FILE * _pFile)
{
	map<wstring, tAnim3D*>::iterator iter = m_mapAnim3D.begin();
	int iAnimSize = m_mapAnim3D.size();
	fwrite(&iAnimSize,sizeof(int),1,_pFile);
	
	for (; iter != m_mapAnim3D.end(); ++iter)
	{
		SaveWString(iter->first, _pFile);
		fwrite(&(iter->second->iStartFrm),sizeof(int),1,_pFile);
		fwrite(&(iter->second->iEndFrm), sizeof(int), 1, _pFile);
		fwrite(&(iter->second->iFrmLength), sizeof(int), 1, _pFile);
	}
}

void CAnimator3D::Load(FILE * _pFile)
{
	DeleteAll();
	CMeshRender* pRender = MeshRender();
	CMesh* pMesh = pRender->GetMesh();
	
	m_pBoneTex = pMesh->GetBoneTex();
	if (m_pBoneTex == nullptr)
	{
		m_pBoneTex = CTexture::CreateTexture(pMesh->GetBone()->size() * 4, 1, D3D10_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC);
		pMesh->SetBoneTex(m_pBoneTex);
	}

	m_pVecBone = pMesh->GetBone();
	m_pVecAnimClip = pMesh->GetAnimClip();
	m_fCurTime = 0.f;
	m_pCurAnim = nullptr;

	m_vecClipTime.resize(m_pVecAnimClip->size());
	m_vecFinalBoneMtx.resize(m_pVecBone->size());
	m_vecWorldBoneMtx.resize(m_pVecBone->size());

	for (size_t i = 0; i < m_pVecBone->size(); i++)
	{
		if (m_pVecBone->at(i).strName == L"RefBox")
			m_iAnimCenter = i;
	}

	int iAnimSize = 0;
	fread(&iAnimSize, sizeof(int), 1, _pFile);

	m_iFrmStart = 0;
	for (int i = 0; i < iAnimSize; i++)
	{
		wstring strKey = LoadWString(_pFile);
		tAnim3D* anim = new tAnim3D;
		fread(&anim->iStartFrm, sizeof(int), 1, _pFile);
		fread(&anim->iEndFrm, sizeof(int), 1, _pFile);
		fread(&anim->iFrmLength, sizeof(int), 1, _pFile);
		anim->strKey = strKey;
		m_mapAnim3D.insert(make_pair(strKey, anim));

		if (m_iFrmStart < anim->iEndFrm)
			m_iFrmStart = anim->iEndFrm;
	}
	m_iFrmStart += 1;
}

void CAnimator3D::FileSave(const wstring & _strPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strPath.c_str(),L"wb");
	Save(pFile);
	fclose(pFile);
}

void CAnimator3D::FileLoad(const wstring & _strPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strPath.c_str(), L"rb");
	Load(pFile);
	fclose(pFile);
}

/*
if (m_pCurAnim == nullptr)
		return 0;

	m_fCurTime = 0.f;
	m_vecClipTime[m_iCurClip] += DT;

	if (m_vecClipTime[m_iCurClip] >= m_pVecAnimClip->at(m_iCurClip).dTimeLength)
		m_vecClipTime[m_iCurClip] = 0.f;

	m_fCurTime += DT;//m_pVecAnimClip->at(m_iCurClip).dStartTime + m_vecClipTime[m_iCurClip];

	int iFrmIndex = (int)(m_fCurTime * m_iFrameCount);
	int iFrmNextIndex = 0;
	for (size_t i = 0; i < m_pVecBone->size(); i++)
	{
		if (m_pVecBone->at(i).vecKeyFrm.empty())
		{
			m_vecFinalBoneMtx[i] = m_pVecBone->at(i).TransForm;
			continue;
		}




		if (iFrmIndex >= m_pVecAnimClip->at(m_iCurClip).iFrmLength - 1)
			iFrmNextIndex = iFrmIndex;
		else
			iFrmNextIndex = iFrmIndex + 1;

		tMtxKeyFrm& CurFrm = m_pVecBone->at(i).vecKeyFrm[iFrmIndex];
		tMtxKeyFrm& NextFrm = m_pVecBone->at(i).vecKeyFrm[iFrmNextIndex];

		float fPercent = (m_fCurTime - CurFrm.fTime) / ( 1.f / m_iFrameCount);

		Vec3 vScale1 = CurFrm.vScale;
		Vec3 vScale2 = NextFrm.vScale;

		Vec3 vTrans1 = CurFrm.vTranslate;
		Vec3 vTrans2 = NextFrm.vTranslate;

		Vec4 vQRot1 = XMLoadFloat4(&CurFrm.vQRot);
		Vec4 vQRot2 = XMLoadFloat4(&NextFrm.vQRot);

		XMVECTOR vScale = XMVectorLerp(vScale1,vScale2,fPercent);
		XMVECTOR vTrans = XMVectorLerp(vTrans1, vTrans2, fPercent);
		XMVECTOR vQRot = XMQuaternionSlerp(vQRot1, vQRot2, fPercent);
		XMVECTOR vCenter = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		m_vecFinalBoneMtx[i] = m_pVecBone->at(i).OffsetMtx * XMMatrixAffineTransformation(vScale, vCenter,vQRot,vTrans);
	}
*/