#pragma once
#include <CScript.h>
class CUpgradeTextScript :
	public CScript
{
private:
	int m_iUp;
	WEAPON_TYPE m_eWType;
	bool bFirst;
public:
	CLONE(CUpgradeTextScript);
	virtual int Update();
	void Start();

public:
	CUpgradeTextScript();
	~CUpgradeTextScript();
};