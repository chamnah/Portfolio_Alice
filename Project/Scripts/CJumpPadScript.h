#pragma once
#include <CScript.h>
class CJumpPadScript :
	public CScript
{
public:
	CLONE(CJumpPadScript);
	virtual void Start();
	virtual int Update();

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CJumpPadScript();
	~CJumpPadScript();
};