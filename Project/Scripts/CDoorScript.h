#pragma once
#include <CScript.h>
class CDoorScript :
	public CScript
{
private:
	float m_fAccTime;
	bool  m_bEvent;
	bool  m_bLeft;
	bool  m_bRot;
	
public:
	virtual int Update();
	virtual int LateUpdate();
	CLONE(CDoorScript);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);


public:
	CDoorScript();
	~CDoorScript();
};