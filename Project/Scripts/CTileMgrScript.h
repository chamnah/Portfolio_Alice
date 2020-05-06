#pragma once
#include "CMgrScript.h"

struct SEARCH
{
	POINT ptPos;
	int   iIdx;
};

class CTileMgrScript :
	public CMgrScript<CTileMgrScript>
{
	CLONE(CTileMgrScript);

private:
	list<SEARCH> m_listOpen;
	POINT       m_ptSize;
	vector<vector<CGameObj*>>* m_vecTile;
	vector<CGameObj*> m_vecClose;
	vector<POINT> m_vecFinal;
	vector<int>   m_vecIdx;
	int           m_iIdx;

public:
	CGameObj* GetPickingTile(Vec3 _vPos, POINT& _ptIdx);
	void TileOutLineSearch(Vec3 _vPos,vector<POINT>& _vecPos, vector<Vec3>* _vecClosePos = nullptr);
	void TurnTile(Vec3 _vPos, vector<TURNTILE>& _vecClosePos);
	bool PuseOpenList(int _iX, int _iY);
	bool OnlyPushOpenList(int _iX, int _iY);
	bool CollCheck(int _iX, int _iY);

public:
	CTileMgrScript();
	~CTileMgrScript();
};