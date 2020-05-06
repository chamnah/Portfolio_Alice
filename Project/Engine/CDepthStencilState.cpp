#include "CDepthStencilState.h"
#include "CDirect.h"

CDepthStencilState::CDepthStencilState()
{
}

CDepthStencilState::~CDepthStencilState()
{
	m_pDepthStencil->Release();
}

void CDepthStencilState::Create(D3D11_DEPTH_STENCIL_DESC & _Desc)
{
	DEVICE->CreateDepthStencilState(&_Desc, &m_pDepthStencil);
	assert(m_pDepthStencil);
}

void CDepthStencilState::SetRef(UINT _Ref)
{
	assert(m_pDepthStencil);
	CONTEXT->OMSetDepthStencilState(m_pDepthStencil, _Ref);
}

void CDepthStencilState::DefaultSetRef()
{
	CONTEXT->OMSetDepthStencilState(nullptr, 0);
}