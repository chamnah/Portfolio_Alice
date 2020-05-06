#pragma once
#include "CComponent.h"
class CGameObj;
class CGrid :
	public CComponent
{
private:
	vector<vector<CGameObj*>> m_vecTileObj;

	int   m_iOldCountX;
	int   m_iOldCountY;

	int   m_iCountX;
	int   m_iCountY;

	Vec3  m_vTileSize;

	POINT m_ptStart;
	POINT m_ptEnd;

public:
	CLONE(CGrid);
	virtual void Awake();
	virtual int Update();
	virtual void Render();
	virtual int FinalUpdate();
	void ChangeTile();

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

	void CurViewCalc();

public:
	void SetCount(UINT _iCountX, UINT _iCountY) { m_iCountX = _iCountX; m_iCountY = _iCountY; }
	void SetCountX(UINT _iX) { m_iCountX = _iX; }
	void SetCountY(UINT _iY) { m_iCountY = _iY; }
	UINT GetCountX() { return m_iCountX; }
	UINT GetCountY() { return m_iCountY; }
	vector<vector<CGameObj*>>& GetVecObj() {return m_vecTileObj;}
	Vec3 GetTransformed();
	void ChangeSize(UINT _iX, UINT _iY);

public:
	CGrid();
	~CGrid();
};