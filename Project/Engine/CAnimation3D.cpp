#include "CAnimation3D.h"
#include "TimeMgr.h"

CAnimation3D::CAnimation3D(int _iStartFrm, int _iEndFrm, vector<tMtxBone>* _pVecBone,int* _pFrmCount)
	:m_iStartFrm(_iStartFrm),
	m_iEndFrm(_iEndFrm),
	m_pVecBone(_pVecBone),
	m_pFrmCount(_pFrmCount)
{
}

CAnimation3D::~CAnimation3D()
{
}

int CAnimation3D::Update()
{
	m_fAccTime += DT;
	int iFrmIndex = (m_fAccTime * (*m_pFrmCount)) + m_iStartFrm;

	if (iFrmIndex > m_iEndFrm)
	{
		m_fAccTime = 0.f;
		iFrmIndex = 0;
	}

	/*for (size_t i = 0; i < m_pVecBone->size(); ++i)
	{
		if (m_pVecBone->at(i).vecKeyFrm.empty())
		{
			m_vecFinalBoneMtx[i] = m_pVecBone->at(i).TransForm;
			continue;
		}

		int iFrmIndex = (int)(m_fCurTime * m_iFrameCount);
		int iFrmNextIndex = 0;

		if (iFrmIndex >= m_pVecAnimClip->at(m_iCurClip).iFrmLength - 1)
			iFrmNextIndex = iFrmIndex;
		else
			iFrmNextIndex = iFrmIndex + 1;

		tMtxKeyFrm& CurFrm = m_pVecBone->at(i).vecKeyFrm[iFrmIndex];
		tMtxKeyFrm& NextFrm = m_pVecBone->at(i).vecKeyFrm[iFrmNextIndex];

		float fPercent = (m_fCurTime - CurFrm.fTime) / (1.f / m_iFrameCount);

		Vec3 vScale1 = CurFrm.vScale;
		Vec3 vScale2 = NextFrm.vScale;

		Vec3 vTrans1 = CurFrm.vTranslate;
		Vec3 vTrans2 = NextFrm.vTranslate;

		Vec4 vQRot1 = XMLoadFloat4(&CurFrm.vQRot);
		Vec4 vQRot2 = XMLoadFloat4(&NextFrm.vQRot);

		XMVECTOR vScale = XMVectorLerp(vScale1, vScale2, fPercent);
		XMVECTOR vTrans = XMVectorLerp(vTrans1, vTrans2, fPercent);
		XMVECTOR vQRot = XMQuaternionSlerp(vQRot1, vQRot2, fPercent);
		XMVECTOR vCenter = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		m_vecFinalBoneMtx[i] = m_pVecBone->at(i).OffsetMtx * XMMatrixAffineTransformation(vScale, vCenter, vQRot, vTrans);
	}*/

	return 0;
}