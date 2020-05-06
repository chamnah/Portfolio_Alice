#pragma once
#include <CScript.h>
class CGameObj;
class CHPScript :
	public CScript
{
public:
	int m_iCurHP;
	int m_iMaxHP;
	vector<CGameObj*> m_vecHP;

public:
	CLONE(CHPScript);

public:
	void Start();
	int Update();
public:
	void AddHP(int _iAdd);
	void SubHP();
	void Reset();

public:
	CHPScript();
	~CHPScript();
};