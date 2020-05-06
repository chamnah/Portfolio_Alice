#pragma once
#include <CScript.h>


struct tTrail
{
	Vec3 vPos;
	float fAge;
};

class CMaterial;
class CTrailScript :
	public CScript
{
private:
	CTexture* m_pPosTex;
	CTexture* m_pDrawTex;
	CTexture* m_pTempTex;
	tTrail m_arrTrail[20];
	int   m_iCurTrail;
	CMaterial* m_pCSMtrl;
	Vec4      m_vPrePos;
	Vec4      m_vUpPos;
	Vec4      m_vDownPos;


public:
	virtual int FinalUpdate();
	CLONE(CTrailScript);

public:
	CTrailScript();
	~CTrailScript();
};