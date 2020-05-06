#pragma once
#include <CScript.h>
class CGameObj;
class CGameCameraScript :
	public CScript
{
private:
	CGameObj*   m_pPlayer;
	Vec3        m_vOldMousePos;
	Mtx			m_mtxOwner;
	Vec3		m_vRot;
	bool        m_bPlayer;
	Vec3        m_vOld;
	bool        m_bExit;
	float       m_fDist;
	float		m_fDifference;

public:
	int Update();
	virtual int LateUpdate();
	virtual int FinalUpdate();
	CLONE(CGameCameraScript);
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);
public:
	CGameCameraScript();
	~CGameCameraScript();
};