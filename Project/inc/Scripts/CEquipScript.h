#pragma once
#include <CScript.h>
class CEquipScript :
	public CScript
{
private:
	int  m_iBoneIdx;
	CGameObj* m_pParentObj;

public:
	virtual void Start();
	virtual int Update();
	CLONE(CEquipScript);

public:
	void SetEquipBone(const wstring& _strBone, CGameObj* _pParent);

public:
	CEquipScript();
	~CEquipScript();
};