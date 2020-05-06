#pragma once
#include <CScript.h>
class CTeapotCannonScript :
	public CScript
{
private:
	float m_fAccTime;

public:
	int Update();
	CLONE(CTeapotCannonScript)

public:
	CTeapotCannonScript();
	~CTeapotCannonScript();
};