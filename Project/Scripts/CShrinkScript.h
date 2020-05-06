#pragma once
#include <CScript.h>
class CShrinkScript :
	public CScript
{
private:
	float m_fAccTime;
	Vec4  m_vColor;

public:
	CLONE(CShrinkScript);
	virtual int Update();
public:
	CShrinkScript();
	~CShrinkScript();
};