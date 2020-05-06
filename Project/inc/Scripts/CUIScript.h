#pragma once
#include <CScript.h>
class CUIScript :
	public CScript
{

public:
	CLONE(CUIScript);
	void Start();
public:
	CUIScript();
	~CUIScript();
};