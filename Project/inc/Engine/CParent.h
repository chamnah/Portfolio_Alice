#pragma once
#include "global.h"

class CParent
{
private:
	wstring m_wcsName;

public:
	void SetName(const wstring& _wcsName) { m_wcsName = _wcsName; }
	const wstring& GetName() const { return m_wcsName; }

public:
	CParent();
	virtual ~CParent();
};