#pragma once
#include <CScript.h>
class CButtonScript :
	public CScript
{
private:
	CTexture* m_pTex;

public:
	CLONE(CButtonScript);

public:
	virtual int Update();

	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

public:
	CButtonScript();
	~CButtonScript();
};