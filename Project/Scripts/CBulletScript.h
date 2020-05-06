#pragma once
#include <CScript.h>
class CBulletScript :
	public CScript
{
private:
	Vec3  m_vDir;
	float m_fSpeed;
	bool  m_bDeath;
	Vec3  m_vCreatePos;

public:
	CLONE(CBulletScript);
	void Start();
	virtual int Update();
	virtual void CollisionEnter(CCollider* _pColl);

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDir(const Vec3& _vDir);
	void StartPos(Vec3 _vStart) { m_vCreatePos = _vStart; }

public:
	CBulletScript();
	~CBulletScript();
};