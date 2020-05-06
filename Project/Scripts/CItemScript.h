#pragma once
#include <CScript.h>
class CItemScript :
	public CScript
{
private:
	ITEM_TYPE m_eItemType;

public:
	CLONE(CItemScript);
	virtual int Update();

public:
	virtual void CollisionEnter(CCollider* _pColl);

	void SetItemType(ITEM_TYPE _eType) { m_eItemType = _eType; }

public:
	CItemScript();
	~CItemScript();
};