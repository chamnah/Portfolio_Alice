#include "CFrustum.h"
#include "CCollider.h"

CFrustum::CFrustum()
{
	m_vProjPt[0] = Vec3(-1.f, 1.f, 0.f);
	m_vProjPt[1] = Vec3(1.f, 1.f, 0.f);
	m_vProjPt[2] = Vec3(1.f, -1.f, 0.f);
	m_vProjPt[3] = Vec3(-1.f, -1.f, 0.f);
	m_vProjPt[4] = Vec3(-1.f, 1.f, 1.f);
	m_vProjPt[5] = Vec3(1.f, 1.f, 1.f);
	m_vProjPt[6] = Vec3(1.f, -1.f, 1.f);
	m_vProjPt[7] = Vec3(-1.f, -1.f, 1.f);


	m_vBoxLocal[0] = Vec4(-0.5f,0.5f,-0.5f,1.f);
	m_vBoxLocal[1] = Vec4(0.5f,0.5f,-0.5f,1.f);
	m_vBoxLocal[2] = Vec4(0.5f,-0.5f,-0.5f,1.f);
	m_vBoxLocal[3] = Vec4(-0.5f,-0.5f,-0.5f,1.f);
	
	m_vBoxLocal[4] = Vec4(-0.5f, 0.5f, 0.5f, 1.f);
	m_vBoxLocal[5] = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	m_vBoxLocal[6] = Vec4(0.5f, -0.5f, 0.5f, 1.f);
	m_vBoxLocal[7] = Vec4(-0.5f, -0.5f, 0.5f, 1.f);
}

CFrustum::~CFrustum()
{
}

int CFrustum::FinalUpdate()
{
	Mtx matViewInv = g_World.mtxView.Invert();
	Mtx matProjInv = g_World.mtxProj.Invert();

	for (UINT i = 0; i < 8; i++)
	{
		m_vWorldPt[i] = XMVector3TransformCoord(m_vProjPt[i], matProjInv);
		m_vWorldPt[i] = XMVector3TransformCoord(m_vWorldPt[i], matViewInv);
	}

	m_vPlane[(UINT)PLANE_DIR::UP] = XMPlaneFromPoints(m_vWorldPt[4], m_vWorldPt[5], m_vWorldPt[0]);
	m_vPlane[(UINT)PLANE_DIR::DOWN] = XMPlaneFromPoints(m_vWorldPt[7], m_vWorldPt[3], m_vWorldPt[6]);
	m_vPlane[(UINT)PLANE_DIR::LEFT] = XMPlaneFromPoints(m_vWorldPt[4], m_vWorldPt[0], m_vWorldPt[7]);
	m_vPlane[(UINT)PLANE_DIR::RIGHT] = XMPlaneFromPoints(m_vWorldPt[5], m_vWorldPt[6], m_vWorldPt[1]);
	m_vPlane[(UINT)PLANE_DIR::FRONT] = XMPlaneFromPoints(m_vWorldPt[4], m_vWorldPt[7], m_vWorldPt[5]);
	m_vPlane[(UINT)PLANE_DIR::BACK] = XMPlaneFromPoints(m_vWorldPt[0], m_vWorldPt[1], m_vWorldPt[3]); // 3점에 대한 평면의 방정식 결과가 도출된다. xyz에는 법선 벡터 z에는 거리가 도출되는데 거리는-로 나온다.

	return 0;
}

bool CFrustum::CheckCullingPoint(const Vec3 & _vWorldPos)
{
	Vec4 vCull;
	for (size_t i = 0; i < (UINT)PLANE_DIR::END; i++)
	{
		vCull = XMPlaneDot(m_vPlane[i], Vec4(_vWorldPos,1.f)); // 임의의 점 Pos와 내적을 하면 w위치가 1이므로 ax + by + cz + dw = 0; w=0는 0이므로 ax + by + cz + dw =0이란 값이 나온다. 

		if (vCull.x > 0) //반환은 다 같은 값이 나온다. 그러므로 하나만 확인 해보면 된다.
			return false;
	}

	return true;
}

bool CFrustum::CheckCullingRange(Vec3 & _vWorldPos, float _fRange)
{
	//_vWorldPos.x = _vWorldScale.x * _vRotDir.x;
	
	Vec4 vCull;
	for (UINT i = 0; i < (UINT)PLANE_DIR::END; i++)
	{
		vCull = XMPlaneDot(m_vPlane[i],Vec4(_vWorldPos,1.f));

		if (vCull.x > _fRange)
		{
			return false;
		}
	}
	return true;
}

bool CFrustum::CheckCullingAABBBox(const Mtx& _mtxWorld)
{
	Vec4 vCull[2];
	Vec3 vMax = {};
	Vec3 vMin = {};
	Vec3 vPos = {};

	vMax = XMVector3TransformCoord(m_vBoxLocal[0], _mtxWorld);
	vMin = vMax;
	for (size_t i = 1; i < 8; i++)
	{
		vPos = XMVector3TransformCoord(m_vBoxLocal[i], _mtxWorld);
		
		if (vPos.x > vMax.x)
			vMax.x = vPos.x;
		if (vPos.y > vMax.y)
			vMax.y = vPos.y;
		if (vPos.z > vMax.z)
			vMax.z = vPos.z;
		
		if (vPos.x < vMin.x)
			vMin.x = vPos.x;
		if (vPos.y < vMin.y)
			vMin.y = vPos.y;
		if (vPos.z < vMin.z)
			vMin.z = vPos.z;
	}
	

	for (UINT i = 0; i < (UINT)PLANE_DIR::END; i++)
	{
		vCull[0] = XMPlaneDot(m_vPlane[i], Vec4(vMax, 1.f));
		vCull[1] = XMPlaneDot(m_vPlane[i], Vec4(vMin, 1.f));
		if (vCull[0].x > 0 && vCull[1].x > 0)
			return false;
	}

	return true;
}

bool CFrustum::CheckCullingOBBBox(const Mtx & _mtxWorld)
{
	Vec3 vCull = {};
	for (size_t i = 0; i < 8; i++)
	{
		vCull = XMVector3TransformCoord(m_vBoxLocal[i], _mtxWorld);
		if (CheckCullingPoint(vCull))
			return true;
	}

	return false;
}

bool CFrustum::CheckCullingCollider(CCollider* _Coll)
{
	if (_Coll->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		Vec3 vPos = _Coll->GetWorldPos();
		return CheckCullingRange(vPos, _Coll->GetRadius());
	}
	else if (_Coll->GetColliderType() == COLLIDER_TYPE::COLL_BOX ||
		_Coll->GetColliderType() == COLLIDER_TYPE::COLL_RECT)
	{
		float fUp = max(_Coll->GetScale().x, _Coll->GetScale().y);
		fUp = max(fUp, _Coll->GetScale().z);
		Vec3 vPos = _Coll->GetWorldPos();
		return CheckCullingRange(vPos, fUp);
	}

	return false;
}