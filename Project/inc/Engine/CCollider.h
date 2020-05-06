#pragma once
#include "CComponent.h"

class CMaterial;
class CMesh;
class CCollider :
	public CComponent
{
private:
	static DWORD  g_dwID;

private:
	Vec3   m_vPos;
	Vec3   m_vScale;
	Vec3   m_vRot;
	Mtx	   m_matWorld;
	Mtx	   m_matRT;
	Mtx    m_mtxLocalRT;

	float  m_fRadius;

	CMaterial*  m_pMaterial;
	CMesh*      m_pMesh;

	COLLIDER_TYPE  m_eCollType;

	DWORD   m_dwID;
	int     m_iCollMtrl;
	bool    m_bLand;
	float   m_fLand;

public:
	int Update();
	int FinalUpdate();
	void Render();
	void MatrixUpdate();
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	int CollisionEnter(CCollider* _pColl);
	int Collision(CCollider* _pColl);
	int CollisionExit(CCollider* _pColl);

	CLONE(CCollider);

	void SetColliderType(COLLIDER_TYPE _eType);
	COLLIDER_TYPE GetColliderType() { return m_eCollType; }
	Mtx& GetWorldMtx() { return m_matWorld; }
	Mtx& GetWorldRTMtx() { return m_matRT; }
	DWORD GetID() { return m_dwID; }
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetPos(Vec3 _vPos) { m_vPos = _vPos;}
	Vec3 GetPos() { return m_vPos; }
	Vec3 GetWorldPos() 
	{ 
		return Vec3(XMVector3TransformCoord(Vec3::Zero, m_matWorld)); 
	}
	void SetScale(Vec3 _vScale) { m_vScale = _vScale; }
	Vec3 GetScale() { return m_vScale; }
	void SetRadius(float _fRad) { m_fRadius = _fRad; }
	float GetRadius() { return m_fRadius; }
	void SetCollMtrl(int _iColl) { m_iCollMtrl = _iColl; }
	//Vec3 GetPos() { return XMVector3TransformCoord(Vec3::Zero, m_matWorld); }
	void SetLand(bool _bLand) { m_bLand = _bLand; }
	bool GetLand() { return m_bLand; }
	void SetLandHeight(float _fLand) { m_fLand = _fLand; }
	float GetLandHeight() { return m_fLand; }
	Mtx& GetLocalRT() { return m_mtxLocalRT; }

	bool IntersectsRay(Vec3& Origin, Vec3& Direction, float& Dist);
	bool CircleIntersectsRay(Vec3& Origin, Vec3& Direction, float& Dist);
	bool BoxIntersectsRay(Vec3& Origin, Vec3& Direction, float& Dist);
public:
	CCollider();
	CCollider(const CCollider& _Coll);
	virtual ~CCollider();
};