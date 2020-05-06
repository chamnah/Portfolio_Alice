#pragma once
#include "CScript.h"
class CDeferredScript :
	public CScript
{
private:
	int m_iSelect;
	int m_iMax;
public:
	int Update();
	CLONE(CDeferredScript)

public:
	CDeferredScript();
	~CDeferredScript();
};