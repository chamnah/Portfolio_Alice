#pragma once
#include "CParent.h"
class CInstancing :
	public CParent
{
private:
	ID3D11Buffer* m_pInstBuff;
	UINT          m_iMaxCount;
	vector<InstData> m_vecInst;

public:
	void Init();
	void SetData();

public:
	void AddInstData(InstData& _tData) { m_vecInst.push_back(_tData); }
	ID3D11Buffer* GetInstBuffer() { return m_pInstBuff; }

public:
	CInstancing();
	~CInstancing();
};