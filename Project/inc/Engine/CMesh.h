#pragma once
#include "CRes.h"

struct tIndexInfo
{
	ID3D11Buffer*    pIB;
	UINT             iIdxCount;
	UINT             iIdxSize;
	DXGI_FORMAT      eFormat;
	D3D11_BUFFER_DESC tDesc;
	void*            pSysMem;
};

class CShader;
class CMaterial;
class CFBXLoader;
class CMesh :
	public CRes
{
private:
	ID3D11Buffer* m_pVB;
	ID3D11InputLayout* m_pLayout;
	ID3DBlob* m_pVSBlob;
	ID3D11Buffer* m_pDrawVB;
	ID3D11Buffer* m_pStreamVB;
	int m_iVtxSize;
	int m_iVtxCount;
	void* m_pVtxMem;
	D3D11_BUFFER_DESC m_descVB;
	bool m_bStreamVB;
	bool m_bFirstParticle;
	vector<tMtxBone> m_vecBone;
	vector<tMtxAnimClip> m_vecAnimClip;
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayout;
	vector<tIndexInfo> m_vecIdxInfo;
	int m_iOffet;
	int m_iOffetInst;
	CTexture*  m_pBoneTex;

public:
	static CMesh* CreateMesh(void* _pVTX,int iVTXSize,int iVTXCount,
		void* _pITX, int iITXSize, int iITXCount);

	static CMesh* CreateMesh(void* _pVTX, int _iVTXSize, int _iVTXCount,vector<tIndexInfo>& _vecInfo);

	void CreateStreamVB(UINT _iSize);
	void SetShader(CShader* _pShader, bool _bInstance = false);
	void Render(int _iSubSet);
	void InstancingRender(int _iSubSet);
	//void Render();
	void StreamRender(CMaterial* _FirstMtrl, CMaterial* _SecondMtrl);
	void VertexRender(int _iSubSet, int _iSize);
	bool IsStreamVB() { return m_bStreamVB; }
	static CMesh* CreateFromContainer(CFBXLoader* _loader);
	void AddInputLayout(const char* _pSemanticName,UINT _iSemanticIdx,DXGI_FORMAT _eFormat,UINT _iInputSlot,UINT _iInstancing, D3D11_INPUT_CLASSIFICATION _eInputClass = D3D11_INPUT_PER_VERTEX_DATA);
	UINT GetSubsetCount() { return m_vecIdxInfo.size(); }

	bool IsAnimation() { return !m_vecAnimClip.empty(); }
	vector<tMtxBone>* GetBone() { return &m_vecBone; }
	vector<tMtxAnimClip>* GetAnimClip() { return &m_vecAnimClip; }

	void SetBoneTex(CTexture* _pTex);
	CTexture* GetBoneTex() { return m_pBoneTex; }


public:
	void SaveMeshData(FILE* _pFile);
	static CMesh* LoadMeshData(FILE* _pFile);

public:
	CMesh();
	virtual ~CMesh();
};