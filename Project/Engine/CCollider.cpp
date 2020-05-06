#include "CCollider.h"
#include "CMaterial.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CTransForm.h"
#include "CConstBuffer.h"
#include "CDirect.h"
#include "CCollisionMgr.h"
#include "CScript.h"
#include "CSceneMgr.h"
#include "CIncident.h"

void CCollider::SetColliderType(COLLIDER_TYPE _eType)
{
	m_eCollType = _eType;

	if (m_eCollType == COLLIDER_TYPE::COLL_RECT)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollRect"); //CollRect RectMesh
		m_pMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_CIRCLE_2D)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollCircle");
		m_pMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_ISO)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"IsoMetricColl"); //IsoMetric  IsoMetricColl
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_BOX)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"); //IsoMetric  IsoMetricColl
		m_pMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"); //IsoMetric  IsoMetricColl
		m_pMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

DWORD CCollider::g_dwID = 0;

bool CCollider::IntersectsRay(Vec3 & Origin, Vec3 & Direction, float & Dist)
{
	if (m_eCollType == COLLIDER_TYPE::COLL_CIRCLE_3D)
	  return CircleIntersectsRay(Origin, Direction, Dist);
	else if (m_eCollType == COLLIDER_TYPE::COLL_BOX)
		return BoxIntersectsRay(Origin, Direction, Dist);
	return false;
}

bool CCollider::CircleIntersectsRay(Vec3 & Origin, Vec3 & Direction, float & Dist)
{
	XMVECTOR vCenter = XMLoadFloat3(&GetWorldPos());
	XMVECTOR vRadius = XMVectorReplicatePtr(&m_fRadius);

	// l is the vector from the ray origin to the center of the sphere.
	XMVECTOR l = XMVectorSubtract(vCenter, Origin);

	// s is the projection of the l onto the ray direction.
	XMVECTOR s = XMVector3Dot(l, Direction);

	XMVECTOR l2 = XMVector3Dot(l, l);

	XMVECTOR r2 = XMVectorMultiply(vRadius, vRadius);

	// m2 is squared distance from the center of the sphere to the projection.
	XMVECTOR m2 = XMVectorNegativeMultiplySubtract(s, s, l2);

	XMVECTOR NoIntersection;

	// If the ray origin is outside the sphere and the center of the sphere is 
	// behind the ray origin there is no intersection.
	NoIntersection = XMVectorAndInt(XMVectorLess(s, XMVectorZero()), XMVectorGreater(l2, r2));

	// If the squared distance from the center of the sphere to the projection
	// is greater than the radius squared the ray will miss the sphere.
	NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(m2, r2));

	// The ray hits the sphere, compute the nearest intersection point.
	XMVECTOR q = XMVectorSqrt(XMVectorSubtract(r2, m2));
	XMVECTOR t1 = XMVectorSubtract(s, q);
	XMVECTOR t2 = XMVectorAdd(s, q);

	XMVECTOR OriginInside = XMVectorLessOrEqual(l2, r2);
	XMVECTOR t = XMVectorSelect(t1, t2, OriginInside);

	if (XMVector4NotEqualInt(NoIntersection, XMVectorTrueInt()))
	{
		// Store the x-component to *pDist.
		XMStoreFloat(&Dist, t);
		return true;
	}

	Dist = 0.f;
	return false;
}

bool CCollider::BoxIntersectsRay(Vec3 & Origin, Vec3 & Direction, float & Dist)
{
	Mtx mtx;
	static const XMVECTORU32 SelectY = { { { XM_SELECT_0, XM_SELECT_1, XM_SELECT_0, XM_SELECT_0 } } };
	static const XMVECTORU32 SelectZ = { { { XM_SELECT_0, XM_SELECT_0, XM_SELECT_1, XM_SELECT_0 } } };

	// Load the box.
	XMVECTOR vCenter = XMLoadFloat3(&GetWorldPos());
	XMVECTOR vExtents = XMLoadFloat3(&m_vScale);
	XMVECTOR vOrientation = XMLoadFloat4(&TransForm()->RotationMtxToQuaternion());

	assert(DirectX::Internal::XMQuaternionIsUnit(vOrientation));

	// Get the boxes normalized side directions.
	XMMATRIX R = XMMatrixRotationQuaternion(vOrientation);

	// Adjust ray origin to be relative to center of the box.
	XMVECTOR TOrigin = XMVectorSubtract(vCenter, Origin);

	// Compute the dot product againt each axis of the box.
	XMVECTOR AxisDotOrigin = XMVector3Dot(R.r[0], TOrigin);
	AxisDotOrigin = XMVectorSelect(AxisDotOrigin, XMVector3Dot(R.r[1], TOrigin), SelectY);
	AxisDotOrigin = XMVectorSelect(AxisDotOrigin, XMVector3Dot(R.r[2], TOrigin), SelectZ);

	XMVECTOR AxisDotDirection = XMVector3Dot(R.r[0], Direction);
	AxisDotDirection = XMVectorSelect(AxisDotDirection, XMVector3Dot(R.r[1], Direction), SelectY);
	AxisDotDirection = XMVectorSelect(AxisDotDirection, XMVector3Dot(R.r[2], Direction), SelectZ);

	// if (fabs(AxisDotDirection) <= Epsilon) the ray is nearly parallel to the slab.
	XMVECTOR IsParallel = XMVectorLessOrEqual(XMVectorAbs(AxisDotDirection), g_RayEpsilon);

	// Test against all three axes simultaneously.
	XMVECTOR InverseAxisDotDirection = XMVectorReciprocal(AxisDotDirection);
	XMVECTOR t1 = XMVectorMultiply(XMVectorSubtract(AxisDotOrigin, vExtents), InverseAxisDotDirection);
	XMVECTOR t2 = XMVectorMultiply(XMVectorAdd(AxisDotOrigin, vExtents), InverseAxisDotDirection);

	// Compute the max of min(t1,t2) and the min of max(t1,t2) ensuring we don't
	// use the results from any directions parallel to the slab.
	XMVECTOR t_min = XMVectorSelect(XMVectorMin(t1, t2), g_FltMin, IsParallel);
	XMVECTOR t_max = XMVectorSelect(XMVectorMax(t1, t2), g_FltMax, IsParallel);

	// t_min.x = maximum( t_min.x, t_min.y, t_min.z );
	// t_max.x = minimum( t_max.x, t_max.y, t_max.z );
	t_min = XMVectorMax(t_min, XMVectorSplatY(t_min));  // x = max(x,y)
	t_min = XMVectorMax(t_min, XMVectorSplatZ(t_min));  // x = max(max(x,y),z)
	t_max = XMVectorMin(t_max, XMVectorSplatY(t_max));  // x = min(x,y)
	t_max = XMVectorMin(t_max, XMVectorSplatZ(t_max));  // x = min(min(x,y),z)

	// if ( t_min > t_max ) return false;
	XMVECTOR NoIntersection = XMVectorGreater(XMVectorSplatX(t_min), XMVectorSplatX(t_max));

	// if ( t_max < 0.0f ) return false;
	NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(XMVectorSplatX(t_max), XMVectorZero()));

	// if (IsParallel && (-Extents > AxisDotOrigin || Extents < AxisDotOrigin)) return false;
	XMVECTOR ParallelOverlap = XMVectorInBounds(AxisDotOrigin, vExtents);
	NoIntersection = XMVectorOrInt(NoIntersection, XMVectorAndCInt(IsParallel, ParallelOverlap));

	if (!DirectX::Internal::XMVector3AnyTrue(NoIntersection))
	{
		// Store the x-component to *pDist
		XMStoreFloat(&Dist, t_min);
		return true;
	}

	Dist = 0.f;
	return false;
}

CCollider::CCollider()
	:m_dwID(g_dwID++)
	, m_fRadius(1.f)
	, m_eCollType(COLLIDER_TYPE::COLL_RECT)
	, m_iCollMtrl(0)
	, m_bLand(false)
	, m_fLand(0.f)
{
	m_eType = COMPONENT_TYPE::COLLIDER;
	m_pMaterial = CResMgr::GetInst()->Load<CMaterial>(L"Coll");

	m_pMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	m_pMaterial->SetRasterizeState(RASTERIZE_TYPE::WIREFRAME);

	//m_vPos = Vec3(1,1,1);
	m_vScale = Vec3(1,1,1.f);
	//m_vRot = Vec3(0, 0, XM_PIDIV4);

	m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollRect");
}

CCollider::CCollider(const CCollider & _Coll)
{
	*this = _Coll;
	m_dwID = g_dwID++;
}

CCollider::~CCollider()
{
}

int CCollider::Update()
{
	Vec3 vDir;
	for (size_t i = 0; i < 3; i++)
	{
		vDir = TransForm()->GetLocalDir((DIR_TYPE)i);
		if (fabsf(vDir.y) ==  1.0f)
		{
			m_fLand = m_vScale[i];
			break;
		}
	}
	return 0;
}

int CCollider::FinalUpdate()
{
	MatrixUpdate();

	CCollisionMgr::GetInst()->AddCollider(this,GetLayerID());
	
	return 0;
}

void CCollider::Render()
{
	m_pMaterial->SetParam(&m_iCollMtrl, SHADER_PARAM::INT_0);

	g_World.mtxWorld = m_matWorld;
	g_World.mtxWVP = g_World.mtxWorld * g_World.mtxView * g_World.mtxProj;
	g_World.mtxWV = g_World.mtxWorld * g_World.mtxView;
	g_World.mtxVP = g_World.mtxView * g_World.mtxProj;
	g_World.mtxInvWorld = m_matWorld.Invert();

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"TransForm");

	pBuffer->UpdateData(&g_World, sizeof(TranformMatrix));
	pBuffer->UpdateRegister();

	m_pMaterial->UpdateData();
	m_pMesh->SetShader(m_pMaterial->GetShader());
	m_pMesh->Render(0);
}

void CCollider::MatrixUpdate()
{
	Mtx   matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	Mtx   matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	Mtx   matRot = XMMatrixIdentity();
	
	/*Mtx   p_matTrans = XMMatrixTranslation(TransForm()->GetPos().x, TransForm()->GetPos().y, TransForm()->GetPos().z);
	Mtx   p_matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	Mtx   p_matRot = XMMatrixRotationZ(TransForm()->GetRotaion().z);

	Mtx ParentMtx = p_matScale * p_matRot * p_matTrans;*/

	m_matWorld = matScale * matRot * matTrans;
	m_matRT = matRot * matTrans;
	Mtx mtxParent = TransForm()->GetRTMtx();

	m_mtxLocalRT = m_matRT * TransForm()->GetMinusRTMtx();
	m_matRT *= mtxParent;
	m_matWorld *= mtxParent;
}

void CCollider::Save(FILE* _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_eCollType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_fRadius, sizeof(float), 1, _pFile);
}

void CCollider::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_eCollType, sizeof(UINT), 1, _pFile);
	fread(&m_vPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fRadius, sizeof(float), 1, _pFile);

	SetColliderType(m_eCollType);
}

int CCollider::CollisionEnter(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
		Script->CollisionEnter(_pColl);

	if(Incident() != nullptr)
		Incident()->CollisionEnter(_pColl);

	return 0;
}

int CCollider::Collision(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
		Script->Collision(_pColl);

	if (Incident() != nullptr)
		Incident()->Collision(_pColl);
	return 0;
}

int CCollider::CollisionExit(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
		Script->CollisionExit(_pColl);

	if (Incident() != nullptr)
		Incident()->CollisionExit(_pColl);
	return 0;
}