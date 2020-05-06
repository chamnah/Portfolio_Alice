#include "CPathMgr.h"

CPathMgr::CPathMgr()
	:m_strPath(L"")
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	GetCurrentDirectory(MAX_PATH, m_strPath);

	for (size_t iNum = wcslen(m_strPath) - 1; iNum >= 0; --iNum)
	{
		if (m_strPath[iNum] == '\\')
		{
			m_strPath[iNum + 1] = NULL;
			break;
		}
	}

	wcscat_s(m_strPath,L"content\\");
}

wstring CPathMgr::GetPathSlice(const wstring & _wcsPath)
{
	wstring wcsName;

	for (size_t iNum = wcslen(_wcsPath.c_str()) - 1; iNum >= 0; --iNum)
	{
		if (_wcsPath[iNum] == '\\')
		{
			for (size_t i = iNum + 1; i < wcslen(_wcsPath.c_str()); ++i)
			{
				wcsName += _wcsPath[i];
			}

			return wcsName;
		}
	}

	return wcsName;
}

wstring CPathMgr::AbsoluteToRelativeResPath(const wstring & _wcsPath)
{
	size_t iIdx = _wcsPath.find(L"content");

	wstring wcsPath = L"..\\";

	for (size_t i = iIdx; i < _wcsPath.size(); ++i)
	{
		wcsPath += _wcsPath[i];
	}

	return wcsPath;
}

wchar_t* CPathMgr::GetFileName(const wstring& _strPath)
{
	_wsplitpath_s(_strPath.c_str(), NULL,0,NULL,0, m_strName, 50,NULL,0);
	return m_strName;
}

wchar_t * CPathMgr::GetFileExtension(const wstring & _strPath)
{
	_wsplitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, m_strExt, 10);
	return m_strExt;
}
