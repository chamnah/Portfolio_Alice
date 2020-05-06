#pragma once
#include "CParent.h"

class CCollider;
enum class PLANE_DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	END
};

class CFrustum :
	public CParent
{
private:
	Vec3   m_vProjPt[8];
	Vec3   m_vWorldPt[8];
	Vec4   m_vPlane[(UINT)PLANE_DIR::END];
	Vec4   m_vBoxLocal[8];
public:
	int FinalUpdate();
	bool CheckCullingPoint(const Vec3& _vWorldPos);
	bool CheckCullingRange(Vec3& _vWorldPos, float _fRange);
	bool CheckCullingAABBBox(const Mtx& _mtxWorld);
	bool CheckCullingOBBBox(const Mtx& _mtxWorld);
	bool CheckCullingCollider(CCollider* _Coll);

public:
	CFrustum();
	~CFrustum();
};