#pragma once
#include <CScript.h>
class CCollider;
class CWKnifeScript :
	public CScript
{
private:
	bool  m_bOn;
	UINT  m_iBoneIndex;
	CGameObj* m_pDecal;
	CCollider* m_pColl;

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl) {}
	virtual void CollisionExit(CCollider* _pColl) {}

public:
	CLONE(CWKnifeScript);
	virtual int Update();
	virtual int FinalUpdate();
	void SetColl();

public:
	CWKnifeScript();
	~CWKnifeScript();
};