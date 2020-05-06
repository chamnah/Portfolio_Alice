#pragma once
#include "CParent.h"
class CGameObj;
class CAnimation3D :
	public CParent
{
private:
	int       m_iStartFrm;
	int       m_iEndFrm;
	int       m_iFrmLength;
	vector<tMtxBone>* m_pVecBone;
	float     m_fAccTime;
	int*      m_pFrmCount;
	Vec3      m_vOffset;

public:
	int Update();

public:
	CAnimation3D(int _iStartFrm, int _iEndFrm, vector<tMtxBone>* _pVecBone, int* _pFrmCount);
	~CAnimation3D();
};