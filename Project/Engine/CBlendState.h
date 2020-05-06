#pragma once
#include "CParent.h"
class CBlendState :
	public CParent
{
private:
	float    m_vFactor[4];
	ID3D11BlendState*    m_pBlend;

public:
	static CBlendState* CreateBlend(D3D11_BLEND_DESC& _Desc);
	void SetShader();
	void DefaultSetShader();

public:
	CBlendState();
	~CBlendState();
};