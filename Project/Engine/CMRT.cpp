#include "CMRT.h"
#include "CDirect.h"
#include "CRT.h"
#include "CTexture.h"

CMRT::CMRT()
	:m_pDSTex(nullptr)
	, m_arrRT{}
{
}

CMRT::~CMRT()
{
	SAFE_DELETE(m_pDSTex);
}

void CMRT::Create(CRT ** _arrRT, CTexture * _pDSTex,UINT iSize)
{
	m_VP.TopLeftX = 0.f;
	m_VP.TopLeftY = 0.f;
	m_VP.Width = (float)(_arrRT[0]->GetTexture()->GetWidth());
	m_VP.Height = (float)(_arrRT[0]->GetTexture()->GetHeight());
	m_VP.MinDepth = 0.f;
	m_VP.MaxDepth = 1.f;

	memcpy(m_arrRT, _arrRT,sizeof(void*) * iSize); // 포인터 갯수만큼 크기 잡음
	m_pDSTex = _pDSTex;
}

void CMRT::OMSet()
{
	CONTEXT->RSSetViewports(1, &m_VP);
	ID3D11RenderTargetView* arrView[8] = {};
	size_t i = 0;
	for (; i < 8; i++)
	{
		if (m_arrRT[i] == nullptr)
		{
			arrView[i] = nullptr;
			continue;
		}
		arrView[i] = m_arrRT[i]->GetTexture()->GetRenderTargetView();
	}
	 
	if (m_pDSTex != nullptr)
		CONTEXT->OMSetRenderTargets(i, arrView, m_pDSTex->GetDepthStencilView());
	else
	{
		ID3D11DepthStencilView* pView = nullptr;
		CONTEXT->OMGetRenderTargets(0, nullptr, &pView);
		pView->Release();
		CONTEXT->OMSetRenderTargets(i, arrView, pView); 
	}
}

void CMRT::Clear()
{
	for (size_t i = 0; i < 8; i++)
	{
		if (m_arrRT[i] == nullptr)
			break;
		m_arrRT[i]->Clear();
	}
	if(m_pDSTex != nullptr)
	CONTEXT->ClearDepthStencilView(m_pDSTex->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}