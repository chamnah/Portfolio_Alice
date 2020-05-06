#pragma once
#include "global.h"
	
union DiscID
{	
	struct 
	{
		DWORD dwFirst;
		DWORD dwSecond;
	};

	ULONGLONG ID;
};	
	
class CCollider;
class CTransForm;
class CCollisionMgr
{	
	SINGLETON(CCollisionMgr);

private:
	map<ULONGLONG, bool>  m_mapID;
	UINT   m_arrCollCheck[MAX_LAYER_COUNT];
	vector<CCollider*> m_arrColl[MAX_LAYER_COUNT];
	Vec3 m_vCollCube[8];
	vector<CCollider*> m_vecDel;

public:
	void CollisionCheck(UINT _iFirst,UINT _iSecond);
	void CollisionCheck(wstring _wcsFirst, wstring _wcsSecond);
	void Collision(UINT _iFirst, UINT _iSecond);
	bool IsCollision(CCollider* _pFirst, CCollider* _pSecond);
	bool MouseCollision(Vec3* _CollRect,UINT _iSize, CTransForm* _pTrans);
	int	 Update();
	void AddCollider(CCollider* _pColl,UINT iLayerID) 
	{ 
		if(iLayerID < MAX_LAYER_COUNT)
		m_arrColl[iLayerID].push_back(_pColl); 
	}
	bool IsObbColl(CCollider * _pFirst, CCollider * _pSecond);
	bool IsCircleColl(CCollider * _pFirst, CCollider * _pSecond);
	bool IsCircleObbColl(CCollider * _pFirst, CCollider * _pSecond);
	void AddDel(CCollider* _pDel) { m_vecDel.push_back(_pDel); }
};	