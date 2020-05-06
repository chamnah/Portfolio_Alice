#pragma once
#include <CScript.h>
class CFadeScript :
	public CScript
{
private:
	float m_fAccTime;

public:
	virtual int Update();

public:
	CLONE(CFadeScript);

public:
	CFadeScript();
	~CFadeScript();
};