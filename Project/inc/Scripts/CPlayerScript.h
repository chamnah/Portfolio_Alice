#pragma once
#include <CScript.h>
class CPlayerScript :
	public CScript
{
private:
	bool   m_bVision;

public:
	int Update();
	CLONE(CPlayerScript)

public:
	CPlayerScript();
	~CPlayerScript();
};