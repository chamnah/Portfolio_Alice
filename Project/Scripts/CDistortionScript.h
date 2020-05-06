#pragma once
#include <CScript.h>
class CDistortionScript :
	public CScript
{
private:
	float m_fAccTime;
	Vec3  m_vChangeScale;
	bool  m_bDeath;

public:
	virtual int Update();
	CLONE(CDistortionScript);

public:
	CDistortionScript();
	~CDistortionScript();
};