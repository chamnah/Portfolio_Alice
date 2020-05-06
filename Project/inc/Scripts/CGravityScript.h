#pragma once
#include <CScript.h>
class CGravityScript :
	public CScript
{
public:
	CLONE(CGravityScript);

	virtual int Update();

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CGravityScript();
	~CGravityScript();
};