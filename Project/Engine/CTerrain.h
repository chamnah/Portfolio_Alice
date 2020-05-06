#pragma once
#include "CComponent.h"
class CMaterial;
class CTexture;
class CTerrain :
	public CComponent
{
private:
	UINT m_iFaceX;
	UINT m_iFaceY;
	CMaterial* m_pCSMtrl;
	CMaterial* m_pPickMtrl;
	CMaterial* m_pWeightMtrl;
	BRUSH_TYPE m_eBrushType;
	CTexture* m_pBrushTex;
	Vec2 m_vBrushSize;
	vector<CTexture*> vecWeightMap;
	CTexture* m_pArrTex;
	CMesh* m_pTerrainMesh;
	CTexture* m_pWeightTex;
	CTexture* m_pHeightMap;
	UINT      m_iGroupWidth;
	UINT      m_iGroupHeight;
	Vec2      m_vHeightMapSize;
	Vec2      m_vTeselRange;

public:
	void Awake();
	int Update();
	CLONE(CTerrain)

private:
	void CreateMesh();
	void CreateComputeShader();

public:
	bool Picking(Vec3& _vPos);

public:
	void SetBrushType(BRUSH_TYPE _eType) { m_eBrushType = _eType; }
	BRUSH_TYPE GetBrushType() { return m_eBrushType; }
	void SetBrushTexture(CTexture* _pTex);
	void SetBrushSize(float _fSize);
	Vec2 GetBrushSize() { return m_vBrushSize; }
	void SetWeightTexture(UINT _iIdx, CTexture* _pTex);
	void SetSelectWeight(UINT iIdx);
	void SetHeightMap(CTexture* _pTex);
	CTexture* GetHeightMap() { return m_pHeightMap; }
	void SetTeselRange(Vec2& _vRange);
	Vec2 GetTeselRange() { return m_vTeselRange; }
public: 
	CTerrain();
	~CTerrain();
};