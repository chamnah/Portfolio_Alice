#pragma once
#include <CScript.h>
class CButtonUIScript :
	public CScript
{
private:
	Vec3 m_vRectColl[2];
	BUTTON_COLL m_eColl;

public:
	CLONE(CButtonUIScript);
	virtual int Update();

public:
	bool MouseCollision();

public:
	void SetCollType(BUTTON_COLL _eType) { m_eColl = _eType; }

public:
	CButtonUIScript();
	~CButtonUIScript();
};