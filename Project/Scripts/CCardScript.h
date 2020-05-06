#pragma once
#include <CScript.h>
class CCardScript :
	public CScript
{
private:
	CGameObj* m_pRedLine[4];
	bool      m_bColl;
	wstring   m_strPrice[3];
	CGameObj* m_pPriceText;
public:
	CLONE(CCardScript);
	virtual void Start();
	virtual int Update();
public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CCardScript();
	~CCardScript();
};