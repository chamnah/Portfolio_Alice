#pragma once
#include <CScript.h>
class CBuyScript :
	public CScript
{
private:
	bool m_bColor;

public:
	CLONE(CBuyScript);
	virtual void Start();
	virtual int Update();

public:
	CBuyScript();
	~CBuyScript();
};