#pragma once
#include <CScript.h>
	
enum class ATTACK_TYPE
{	
	NONE,
	TRACE,
	MELEE,
	RANGE,
	QUAKE,
	FIREBALL
};	
	
class CEquipScript;
class CGruntScript :
	public CScript
{	
private:
	int m_iHP;
	float m_fSpeed;
	Vec3  m_vPlayerDir;
	bool  m_bAttack;
	wstring m_strCurAnim;
	bool  m_bCombo;
	bool  m_bSpawn;
	ATTACK_TYPE m_eAttType;
	ATTACK_TYPE m_ePreType;
	float m_fIdleTime;
	CGameObj* m_pPlayer;
	CGameObj* m_pBullet;
	CEquipScript* m_pEquip;
	CGameObj* m_pAttack;
	bool m_bDamage;

public:
	CLONE(CGruntScript);
	virtual void Start();
	virtual int Update();
	void Idle();
	void Trace();
	void MeleeAttack();
	void Range();
	void FireBall();

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CGruntScript();
	~CGruntScript();
};	