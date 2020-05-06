#pragma once
#include "global.h"
class CShader;
class CShaderMgr
{
	SINGLETON(CShaderMgr);

private:
	map<wstring, CShader*> m_mapShader;
	vector<CShader*> m_vecShaderClone;

public:
	void Init();
	void AddShader(wstring _wcsKey,CShader* _pShader);
	void AddShader(CShader* _pShader) { m_vecShaderClone.push_back(_pShader); }
	CShader* FindShader(wstring _wcsKey);
};