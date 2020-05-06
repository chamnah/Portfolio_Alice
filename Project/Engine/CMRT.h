#pragma once
#include "CParent.h"
class CRT;
class CTexture;
class CMRT :
	public CParent
{
private:
	CRT*    m_arrRT[8];
	CTexture* m_pDSTex;
	D3D11_VIEWPORT m_VP;

public:
	void Create(CRT** _arrRT,CTexture* _pDSTex, UINT iSize);
	void OMSet();
	void Clear();

public:
	CMRT();
	~CMRT();
};