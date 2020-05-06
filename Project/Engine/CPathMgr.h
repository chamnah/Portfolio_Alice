#include "global.h"

#pragma once
class CPathMgr
{
private:
	wchar_t m_strPath[MAX_PATH];
	wchar_t m_strName[MAX_PATH];
	wchar_t m_strExt[MAX_PATH];

public:
	void Init();
	wstring GetPathSlice(const wstring& _wcsPath);
	wstring AbsoluteToRelativeResPath(const wstring& _wcsPath);
	wchar_t* GetPath() { return m_strPath; }
	wchar_t* GetFileName(const wstring& _strPath);
	wchar_t* GetFileExtension(const wstring& _strPath);
public:
	SINGLETON(CPathMgr);
};