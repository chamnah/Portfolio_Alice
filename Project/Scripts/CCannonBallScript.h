#pragma once
#include <CScript.h>
class CCannonBallScript :
	public CScript
{
private:
	Vec3  m_vDir;
	float m_fSpeed;
	float m_fUpPower;
	float m_fGravity;
	bool  m_bDeath;
	float m_fAccTime;

public:
	int Update();
	CLONE(CCannonBallScript)

public:
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetDir(const Vec3& _vDir) { m_vDir = _vDir; }

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CCannonBallScript();
	~CCannonBallScript();
};