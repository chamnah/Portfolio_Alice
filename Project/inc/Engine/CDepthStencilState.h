#pragma once
#include "CParent.h"

class CDepthStencilState :
	public CParent
{
private:
	ID3D11DepthStencilState*    m_pDepthStencil;

public:
	void Create(D3D11_DEPTH_STENCIL_DESC& _Desc);
	void SetRef(UINT _Ref = 0U);
	void DefaultSetRef();
	CDepthStencilState();
	~CDepthStencilState();
};