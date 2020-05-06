#pragma once
#include <CScript.h>
class CMonsterSpawnScript :
	public CScript
{
public:
	CLONE(CMonsterSpawnScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CMonsterSpawnScript();
	~CMonsterSpawnScript();
};