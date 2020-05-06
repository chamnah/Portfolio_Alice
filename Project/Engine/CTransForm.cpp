#include "CTransForm.h"
#include "CConstBuffer.h"
#include "CDirect.h"

TranformMatrix g_World;

Vec3 g_vDir[(UINT)DIR_TYPE::END] = { Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f),Vec3(-1.f, 0.f, 1.f) };

void CTransForm::RotationMatrix(const Mtx & _mtxRot)
{
	Mtx mtxRot = XMMatrixRotationX(m_vRot.x + m_vAddRot.x);
	mtxRot = XMMatrixRotationY(m_vRot.y + m_vAddRot.y);
	mtxRot = XMMatrixRotationZ(m_vRot.z + m_vAddRot.z);

	mtxRot *= _mtxRot;
	m_vRot = DecomposeRot(mtxRot);
}

Vec4 CTransForm::RotationMtxToQuaternion()
{
	Vec4 q;
	float trace = m_mtxRot._11 + m_mtxRot._22 + m_mtxRot._33;
	if (trace > 0) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (m_mtxRot._32 - m_mtxRot._23) * s;
		q.y = (m_mtxRot._13 - m_mtxRot._31) * s;
		q.z = (m_mtxRot._21 - m_mtxRot._12) * s;
	}
	else {
		if (m_mtxRot._11 > m_mtxRot._22 && m_mtxRot._11 > m_mtxRot._33) {
			float s = 2.0f * sqrtf(1.0f + m_mtxRot._11 - m_mtxRot._22 - m_mtxRot._33);
			q.w = (m_mtxRot._32 - m_mtxRot._23) / s;
			q.x = 0.25f * s;
			q.y = (m_mtxRot._12 + m_mtxRot._21) / s;
			q.z = (m_mtxRot._13 + m_mtxRot._31) / s;
		}
		else if (m_mtxRot._22 > m_mtxRot._33) {
			float s = 2.0f * sqrtf(1.0f + m_mtxRot._22 - m_mtxRot._11 - m_mtxRot._33);
			q.w = (m_mtxRot._13 - m_mtxRot._31) / s;
			q.x = (m_mtxRot._12 + m_mtxRot._21) / s;
			q.y = 0.25f * s;
			q.z = (m_mtxRot._23 + m_mtxRot._32) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + m_mtxRot._33 - m_mtxRot._11 - m_mtxRot._22);
			q.w = (m_mtxRot._21 - m_mtxRot._12) / s;
			q.x = (m_mtxRot._13 + m_mtxRot._31) / s;
			q.y = (m_mtxRot._23 + m_mtxRot._32) / s;
			q.z = 0.25f * s;
		}
	}
	return q;
}

CTransForm::CTransForm()
	:m_vLocalDir{Vec3::Right,Vec3::Up,Vec3::Front}
	, m_vWorldDir{ Vec3::Right,Vec3::Up,Vec3::Front }
	, m_vScale{1.f,1.f,1.f}
	, m_vAddPos{}
	, m_vOldPos{}
{
	m_mtxParent = XMMatrixIdentity();
	m_eType = COMPONENT_TYPE::TRANSFORM;
	m_mtxOwner = XMMatrixIdentity();
}

CTransForm::~CTransForm()
{
}

int CTransForm::Update()
{
	return 0;
}

int CTransForm::LateUpdate()
{
	return 0;
}

int CTransForm::FinalUpdate()
{
	Mtx matTrans = XMMatrixTranslation(m_vPos.x + m_vAddPos.x, m_vPos.y + m_vAddPos.y, m_vPos.z + m_vAddPos.z);
	m_mtxRot = XMMatrixRotationX(m_vRot.x + m_vAddRot.x);
	m_mtxRot *= XMMatrixRotationY(m_vRot.y + m_vAddRot.y);
	m_mtxRot *= XMMatrixRotationZ(m_vRot.z + m_vAddRot.z);

	m_mtxMinusRT = XMMatrixRotationX(-(m_vRot.x + m_vAddRot.x));
	m_mtxMinusRT *= XMMatrixRotationY(m_vRot.y + m_vAddRot.y);
	m_mtxMinusRT *= XMMatrixRotationZ(m_vRot.z + m_vAddRot.z);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(g_vDir[i], m_mtxRot);
		m_vLocalDir[i].Normalize();
		m_vWorldDir[i] = m_vLocalDir[i];
	}
	Mtx matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	CGameObj* pOwner = GetParentOfOwner();

	if (pOwner == nullptr)
		m_mtxParent = XMMatrixIdentity();
	else
	{
		m_mtxParent = pOwner->GetTransForm()->GetRTMtx();
	
		for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(g_vDir[i], m_mtxOwner);
			m_vWorldDir[i].Normalize();

			m_vWorldDir[i] = XMVector3TransformNormal(m_vWorldDir[i], m_mtxParent);
			m_vWorldDir[i].Normalize();
		}
	}
	
	m_mtxWorld = matScale * m_mtxRot * matTrans * m_mtxOwner * m_mtxParent;
	m_mtxRT = m_mtxRot * matTrans * m_mtxOwner * m_mtxParent;
	m_mtxMinusRT = m_mtxMinusRT * matTrans * m_mtxOwner * m_mtxParent;

	return 0;
}

void CTransForm::UpdateData()
{
	m_mtxView = g_World.mtxView;
	g_World.mtxWorld = m_mtxWorld;
	g_World.mtxWVP = g_World.mtxWorld * g_World.mtxView * g_World.mtxProj;
	g_World.mtxWV = g_World.mtxWorld * g_World.mtxView;
	g_World.mtxVP = g_World.mtxView * g_World.mtxProj;
	m_mtxInvWorld = m_mtxWorld.Invert();
	g_World.mtxInvWorld = m_mtxInvWorld;

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"TransForm");

	pBuffer->UpdateData(&g_World,sizeof(TranformMatrix));
	pBuffer->UpdateRegisterAll();
}

void CTransForm::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_vScale,sizeof(Vec3),1,_pFile);
	fwrite(&m_vPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRot, sizeof(Vec3), 1, _pFile);
}

void CTransForm::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vRot, sizeof(Vec3), 1, _pFile);
}

void CTransForm::SetLook(Vec3 & _vDir)
{
	Vec3 vFront = _vDir;
	vFront.Normalize();

	Vec3 vRight = Vec3::Up.Cross(_vDir);
	vRight.Normalize();

	Vec3 vUp = vFront.Cross(vRight);
	vUp.Normalize();

	Mtx mtxRot = XMMatrixIdentity();
	mtxRot.Front(vFront);
	mtxRot.Right(vRight);
	mtxRot.Up(vUp);

	m_vRot = DecomposeRot(mtxRot);
}

void CTransForm::SetLookAddRot(Vec3 & _vDir)
{
	Vec3 vFront = _vDir;
	vFront.Normalize();

	Vec3 vRight = Vec3::Up.Cross(_vDir);
	vRight.Normalize();

	Vec3 vUp = vFront.Cross(vRight);
	vUp.Normalize();

	Mtx mtxRot = XMMatrixIdentity();
	mtxRot.Front(vFront);
	mtxRot.Right(vRight);
	mtxRot.Up(vUp);

	m_vAddRot = DecomposeRot(mtxRot);
}