#pragma once
#include "global.h"
class CStateMgr
{
	SINGLETON(CStateMgr);

	map<wstring, ID3D11BlendState*> m_mapBlendState;
	map<wstring, ID3D11DepthStencilState*> m_mapDepthState;

public:
	void CreateBlendState();
	void AddBlendState(const wstring& _wcsKey,ID3D11BlendState* _pBS);
	void AddDepthState(const wstring& _wcsKey, ID3D11DepthStencilState* _pBS);

	void BlendRender(const wstring& _wcsKey);
	void DepthRender(const wstring& _wcsKey);
	void DepthClear();
	ID3D11DepthStencilState* FindDepthState(const wstring& _wcsKey);
};