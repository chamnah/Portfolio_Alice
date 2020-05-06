#pragma once
#include <CScript.h>
class CBodyScript :
	public CScript
{
public:
	CLONE(CBodyScript);
	virtual int Update();

public:
	CBodyScript();
	~CBodyScript();
};