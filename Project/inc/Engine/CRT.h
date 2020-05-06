#pragma once
#include "CParent.h"
class CTexture;
class CRT :
	public CParent
{
private:
	CTexture*    m_pTexture;
	Vec4		 m_vClearColor;

public:
	CTexture* GetTexture() { return m_pTexture; }
	Vec4&     GetColor() { return m_vClearColor; }
	void      Clear();
public:
	CRT(CTexture* _pTex,const Vec4& _vColor);
	~CRT();
};