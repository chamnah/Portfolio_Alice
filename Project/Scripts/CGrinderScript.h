#pragma once
#include <CScript.h>
class CGrinderScript :
	public CScript
{
private:
	float m_fAccTime;
public:
	virtual void Start();
	virtual int Update();
	CLONE(CGrinderScript);
public:
	CGrinderScript();
	~CGrinderScript();
};