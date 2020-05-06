#include "CBlendState.h"
#include "CDirect.h"

CBlendState::CBlendState()
	:m_vFactor{}
	, m_pBlend(nullptr)
{
}

CBlendState::~CBlendState()
{
	SAFE_RELEASE(m_pBlend);
}

CBlendState* CBlendState::CreateBlend(D3D11_BLEND_DESC & _Desc)
{
	CBlendState* pBlend = new CBlendState;
	DEVICE->CreateBlendState(&_Desc,&pBlend->m_pBlend);
	assert(pBlend->m_pBlend);

	return pBlend;
}

void CBlendState::SetShader()
{
	assert(m_pBlend);
	CONTEXT->OMSetBlendState(m_pBlend, m_vFactor, 0xffffffff);
}

void CBlendState::DefaultSetShader()
{
	float fFactor[4] = {};
	CONTEXT->OMSetBlendState(nullptr, fFactor,0xffffffff);
}