#pragma once
#include <CScript.h>
class CCollisionScript :
	public CScript
{
public:
	CLONE(CCollisionScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

public:
	CCollisionScript();
	~CCollisionScript();
};