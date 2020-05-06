#pragma once
#include <CScript.h>
class CDirLightScript :
	public CScript
{
public:
	CLONE(CDirLightScript);
	virtual int Update();
public:
	CDirLightScript();
	~CDirLightScript();
};