#pragma once
#include <CScript.h>
class CTitleScript :
	public CScript
{
private:
	float m_fAccTime;
	bool  m_bEnter;

public:
	void Start();
	virtual int Update();
	CLONE(CTitleScript);

public:
	CTitleScript();
	~CTitleScript();
};