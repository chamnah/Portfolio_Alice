#pragma once
#include "CScript.h"

class CCameraScript :
	public CScript
{
private:
	Vec3 m_vOldMousePos;

public:
	int FinalUpdate();
	CLONE(CCameraScript)

public:
	CCameraScript();
	virtual ~CCameraScript();
};