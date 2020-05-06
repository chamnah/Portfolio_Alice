#include "CRT.h"
#include "CDirect.h"
#include "CTexture.h"

void CRT::Clear()
{
	CONTEXT->ClearRenderTargetView(m_pTexture->GetRenderTargetView(), (float*)&m_vClearColor);
}

CRT::CRT(CTexture * _pTex,const Vec4 & _vColor)
	:m_pTexture(_pTex)
	,m_vClearColor(_vColor)
{
}

CRT::~CRT()
{
	SAFE_DELETE(m_pTexture);
}