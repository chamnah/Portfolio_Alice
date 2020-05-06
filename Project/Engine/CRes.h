#pragma once
#include "CParent.h"
class CRes : public CParent
{
protected:
	wstring m_wcsPath;
	UINT    m_iID;

public:
	void SetPath(const wstring& _wcsPath) { m_wcsPath = _wcsPath; }
	wstring& GetPath() { return m_wcsPath; }
	UINT GetResID() { return m_iID; }
public:
virtual void Save(FILE* _pFile) {}
void Load(FILE* _pFile) {}

public:
	CRes();
	virtual ~CRes();
};