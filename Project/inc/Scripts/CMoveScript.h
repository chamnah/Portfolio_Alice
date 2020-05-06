#pragma once
#include <CScript.h>
class CMoveScript :
	public CScript
{
private:
	float m_fSpeed;

public:
	CLONE(CMoveScript);
	virtual int Update();

public:
	CMoveScript();
	~CMoveScript();
};