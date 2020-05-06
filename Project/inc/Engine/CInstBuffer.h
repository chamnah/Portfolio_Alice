#pragma once
#include "CParent.h"
class CTexture;
class CInstBuffer :
	public CParent
{
private:
	ID3D11Buffer* m_pInstBuff;
	UINT          m_iMaxCount;
	vector<InstData> m_vecInst;
	vector<Mtx>  m_vecBone;
	int          m_iAnimCount;

public:
	SINGLETON(CInstBuffer);

public:
	void Init();
	void SetData(CTexture* _pBoneTex);

public:
	void AddInstData(InstData _tData) { m_vecInst.push_back(_tData); }
	void AddBoneMtx(vector<Mtx>& _mtxBone);
	void Clear() { m_vecInst.clear(); m_vecBone.clear(); m_iAnimCount = 0;}
	ID3D11Buffer* GetInstBuffer() { return m_pInstBuff; }
	UINT GetInstBufferCount() { return m_vecInst.size(); }
	void Resize();
};