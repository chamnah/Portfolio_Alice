#pragma once
#include <CScript.h>
class CSkirtScript :
	public CScript
{
private:
	int m_iBoneIndex;
	bool m_bOn;

public:
	CLONE(CSkirtScript);
	virtual int Update();

public:
	CSkirtScript();
	~CSkirtScript();
};

