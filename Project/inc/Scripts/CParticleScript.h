#pragma once
#include <CScript.h>
class CMaterial;
class CParticleScript :
	public CScript
{
private:
	float m_fPushTime;
	CMaterial* m_pParticle;
	CMesh*   m_pMesh;
	int   m_iVtxCount;
	CTexture* pDrawTex;
	CTexture* pPosTex;
	float m_fDeathTime;
	int   m_iCount;

public:
	CLONE(CParticleScript);
	virtual void Start();
	virtual int Update();

public:
	void SetCount(int _iCount);

public:
	CParticleScript();
	~CParticleScript();
};