#include "CStateMgr.h"
#include "CDirect.h"

void CStateMgr::CreateBlendState()
{
}

void CStateMgr::AddBlendState(const wstring& _wcsKey, ID3D11BlendState* _pBS)
{
	
	if (_pBS == nullptr)
		assert(NULL);
	
	map<wstring, ID3D11BlendState*>::iterator iter = m_mapBlendState.find(_wcsKey);

	if (iter != m_mapBlendState.end())
		assert(NULL);
	
	m_mapBlendState.insert(make_pair(_wcsKey, _pBS));
}

void CStateMgr::AddDepthState(const wstring & _wcsKey, ID3D11DepthStencilState * _pBS)
{
	if (_pBS == nullptr)
		assert(NULL);

	map<wstring, ID3D11DepthStencilState*>::iterator iter = m_mapDepthState.find(_wcsKey);

	if (iter != m_mapDepthState.end())
		assert(NULL);

	m_mapDepthState.insert(make_pair(_wcsKey, _pBS));
}

void CStateMgr::BlendRender(const wstring & _wcsKey)
{
	map<wstring, ID3D11BlendState*>::iterator iter = m_mapBlendState.find(_wcsKey);

	if (iter == m_mapBlendState.end())
		assert(NULL);

	float fFactor[4] = {};
	CONTEXT->OMSetBlendState(iter->second, fFactor, 0xffffffff);
}

void CStateMgr::DepthRender(const wstring & _wcsKey)
{
	map<wstring, ID3D11DepthStencilState*>::iterator iter = m_mapDepthState.find(_wcsKey);

	if (iter == m_mapDepthState.end())
		assert(NULL);

	float fFactor[4] = {};
	CONTEXT->OMSetDepthStencilState(iter->second, 1);
}

void CStateMgr::DepthClear()
{
	CONTEXT->OMSetDepthStencilState(0, 0);
}

ID3D11DepthStencilState* CStateMgr::FindDepthState(const wstring & _wcsKey)
{

	map<wstring, ID3D11DepthStencilState*>::iterator iter = m_mapDepthState.find(_wcsKey);

	if (iter == m_mapDepthState.end())
		assert(NULL);

	return iter->second;
}

CStateMgr::CStateMgr()
{
}

CStateMgr::~CStateMgr()
{
	SafeMapRelease(m_mapBlendState);
	SafeMapRelease(m_mapDepthState);
}