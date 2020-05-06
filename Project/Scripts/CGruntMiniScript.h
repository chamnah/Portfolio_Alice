#pragma once
#include <CScript.h>
class CGruntMiniScript :
	public CScript
{
private:
	int m_iHP;
	float m_fSpeed;
	Vec3  m_vRight;
	Vec3  m_vPlayerDir;
	bool  m_bAttack;
	wstring m_strAttack;
	bool  m_bCombo;
	bool  m_bSpawn;
	CGameObj* m_pAttObj;
	bool  m_bDamage;
public:
	CLONE(CGruntMiniScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	virtual void Start();
	virtual int Update();
public:
	CGruntMiniScript();
	~CGruntMiniScript();
};