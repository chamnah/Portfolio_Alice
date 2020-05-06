#pragma once
#include <CScript.h>
class CScoreScript :
	public CScript
{
public:
	CLONE(CScoreScript);
	virtual void Start();
	virtual int Update();
public:
	CScoreScript();
	~CScoreScript();
};