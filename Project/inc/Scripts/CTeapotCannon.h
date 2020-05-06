#pragma once
#include <CScript.h>
class CTeapotCannon :
	public CScript
{
private:
	float m_fAccTime;

public:
	int Update();
	CLONE(CTeapotCannon)

public:
	CTeapotCannon();
	~CTeapotCannon();
};