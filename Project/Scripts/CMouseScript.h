#pragma once
#include <CScript.h>
class CMouseScript :
	public CScript
{
public:
	CLONE(CMouseScript);
	virtual int Update();


public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

public:
	CMouseScript();
	~CMouseScript();
};