#pragma once
#include <CScript.h>

enum class CAR_DIR
{
	STOP,
	FRONT,
	BACK
};

class CCarScript :
	public CScript
{
private:
	float m_fAccSpeed;
	float m_fSpeed;
	float m_fFinalSpeed;
	float m_fFriction;
	Vec3  m_vDir;
	CAR_DIR m_eDir;

public:
	virtual void Start();
	virtual int Update();
	CLONE(CCarScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CCarScript();
	virtual ~CCarScript();
};