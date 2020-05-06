#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CCollider.h"
#include "CLayer.h"
#include "CTransForm.h"
#include "KeyMgr.h"

CCollisionMgr::CCollisionMgr()
{
	m_vCollCube[0] = Vec3(-0.5f,0.5f,-0.5f); //가까운 곳    // 0 ----- 1
	m_vCollCube[1] = Vec3(0.5f, 0.5f, -0.5f);               // |       |
	m_vCollCube[2] = Vec3(0.5f, -0.5f, -0.5f);              // |       |
	m_vCollCube[3] = Vec3(-0.5f, -0.5f, -0.5f);             // 3 ----- 2

	m_vCollCube[4] = Vec3(-0.5f, 0.5f, 0.5f); //먼 곳       // 4 ----- 5
	m_vCollCube[5] = Vec3(0.5f, 0.5f, 0.5f);                // |       |
	m_vCollCube[6] = Vec3(0.5f, -0.5f, 0.5f);               // |       |
	m_vCollCube[7] = Vec3(-0.5f, -0.5f, 0.5f);				// 7 ----- 6
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionCheck(UINT _iFirst, UINT _iSecond)
{
	UINT iColl = _iFirst;
	UINT iRow = _iSecond;

	if (iColl > iRow)
	{
		iColl = _iSecond;
		iRow = _iFirst;
	}

	UINT iCheck = 1 << iRow;
	m_arrCollCheck[iColl] |= iCheck;
}

void CCollisionMgr::CollisionCheck(wstring _wcsFirst, wstring _wcsSecond)
{
	UINT iFirst = CSceneMgr::GetInst()->GetLayer(_wcsFirst)->GetID();
	UINT iSecond = CSceneMgr::GetInst()->GetLayer(_wcsSecond)->GetID();

	CollisionCheck(iFirst,iSecond);
}

int CCollisionMgr::Update()
{
	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
	{
		for (UINT j = i; j < MAX_LAYER_COUNT; ++j)
		{
			if (m_arrCollCheck[i] & (1 << j))
				Collision(i,j);
		}
	}

	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
		m_arrColl[i].clear();

	for (size_t i = 0; i < m_vecDel.size(); i++)
	{
		delete m_vecDel[i];
	}
	m_vecDel.clear();

	return 0;
}

bool CCollisionMgr::IsObbColl(CCollider * _pFirst, CCollider * _pSecond)
{
	Vec3 vPoint[4] = {};
	Vec3 vSource[6] = {};
	Vec3 vProj[6] = {};

	vPoint[0] = XMVector3TransformCoord(m_vCollCube[0], _pFirst->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(m_vCollCube[1], _pFirst->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(m_vCollCube[3], _pFirst->GetWorldMtx());
	vPoint[3] = XMVector3TransformCoord(m_vCollCube[4], _pFirst->GetWorldMtx());

	vSource[0] = vPoint[1] - vPoint[0];
	vSource[1] = vPoint[2] - vPoint[0];
	vSource[2] = vPoint[3] - vPoint[0];

	vProj[0] = vSource[0]; 
	vProj[0].Normalize();

	vProj[1] = vSource[1];
	vProj[1].Normalize();

	vProj[2] = vSource[2];
	vProj[2].Normalize();

	vPoint[0] = XMVector3TransformCoord(m_vCollCube[0], _pSecond->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(m_vCollCube[1], _pSecond->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(m_vCollCube[3], _pSecond->GetWorldMtx());
	vPoint[3] = XMVector3TransformCoord(m_vCollCube[4], _pSecond->GetWorldMtx());

	vSource[3] = vPoint[1] - vPoint[0];
	vSource[4] = vPoint[2] - vPoint[0];
	vSource[5] = vPoint[3] - vPoint[0];

	vProj[3] = vSource[3];
	vProj[3].Normalize();

	vProj[4] = vSource[4];
	vProj[4].Normalize();

	vProj[5] = vSource[5];
	vProj[5].Normalize();

	Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, _pFirst->GetWorldMtx()) - XMVector3TransformCoord(Vec3::Zero, _pSecond->GetWorldMtx());

	for (size_t i = 0; i < 6; i++)
	{
		float fDist = 0.f;
		float fCenter = 0.f;
		for (size_t j = 0; j < 6; j++)
			fDist += fabsf(vSource[j].Dot(vProj[i]));

		fDist *= 0.5f;
		fCenter = fabsf(vCenter.Dot(vProj[i]));
		if (fCenter >= fDist)
			return false;
	}

	return true;
}

bool CCollisionMgr::IsCircleColl(CCollider * _pFirst, CCollider * _pSecond)
{
	float Dist = Vec3::Distance(_pFirst->GetWorldPos(), _pSecond->GetWorldPos());

	if (Dist <= _pFirst->GetRadius() + _pSecond->GetRadius())
	{
		_pFirst->SetCollMtrl(1);
		_pSecond->SetCollMtrl(1);
		return true;
	}

	_pFirst->SetCollMtrl(0);
	_pSecond->SetCollMtrl(0);
	return false;
}

bool CCollisionMgr::IsCircleObbColl(CCollider * _pFirst, CCollider * _pSecond)
{
	if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		CCollider* pTemp = _pFirst;
		_pFirst = _pSecond;
		_pSecond = pTemp;
	}

	Vec3 vBoxMin = _pFirst->GetWorldPos() - (_pFirst->GetScale() * 0.5f);
	Vec3 vBoxMax = _pFirst->GetWorldPos() + (_pFirst->GetScale() * 0.5f);

    Vec3 vSherePos = _pSecond->GetWorldPos();
	if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_RECT)
	{
		vSherePos -= _pFirst->GetWorldPos();
		vSherePos = XMVector3TransformCoord(vSherePos, _pFirst->GetLocalRT());
	}
	else
	{
		vSherePos -= _pFirst->GetWorldPos();
		vSherePos = XMVector3TransformCoord(vSherePos, _pFirst->GetWorldRTMtx());
	}
	Vec3 vNear;
	for (size_t i = 0; i < 3; i++)
	{
		vNear[i] = vSherePos[i];
		if (vNear[i] < vBoxMin[i])
			vNear[i] = vBoxMin[i];
		else if (vNear[i] > vBoxMax[i])
			vNear[i] = vBoxMax[i];
	}

	float Dis = Vec3::Distance(vSherePos, vNear);

	if (Dis < _pSecond->GetRadius())
	{
		_pFirst->SetCollMtrl(1);
		_pSecond->SetCollMtrl(1);
		return true;
	}

	_pFirst->SetCollMtrl(0);
	_pSecond->SetCollMtrl(0);
	return false;
}

void CCollisionMgr::Collision(UINT _iFirst, UINT _iSecond)
{
	for (UINT i = 0; i < m_arrColl[_iFirst].size(); ++i)
	{
		for (UINT j = 0; j < m_arrColl[_iSecond].size(); ++j)
		{
			if (m_arrColl[_iFirst][i] == m_arrColl[_iSecond][j])
				break;
			DiscID id;
			id.dwFirst = m_arrColl[_iFirst][i]->GetID();
			id.dwSecond = m_arrColl[_iSecond][j]->GetID();

			map<ULONGLONG,bool>::iterator iter = m_mapID.find(id.ID);

			if (IsCollision(m_arrColl[_iFirst][i], m_arrColl[_iSecond][j])) // 충돌
			{
				if (iter == m_mapID.end()) // 처음 충돌한 상태이다.
				{
					m_arrColl[_iFirst][i]->CollisionEnter(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionEnter(m_arrColl[_iFirst][i]);
					m_mapID.insert(make_pair(id.ID,true));
				}
				else if (iter->second == false) // 이전에 충돌을 안한 상태에서 충돌 했다.
				{
					iter->second = true;
					m_arrColl[_iFirst][i]->CollisionEnter(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionEnter(m_arrColl[_iFirst][i]);
				}
				else // 나머지 즉 충돌 중인 상태에서 또 충돌을 했다.
				{
					m_arrColl[_iFirst][i]->Collision(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->Collision(m_arrColl[_iFirst][i]);
				}
			}
			else if(iter != m_mapID.end()) // 충돌 안함
			{
				if (iter->second == true)
				{
					iter->second = false;

					m_arrColl[_iFirst][i]->CollisionExit(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionExit(m_arrColl[_iFirst][i]);
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider * _pFirst, CCollider * _pSecond)
{
	if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		return IsCircleColl(_pFirst, _pSecond);
	}
	else if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_BOX &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_BOX)
	{
		return IsObbColl(_pFirst, _pSecond);
	}
	else if ((_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_BOX &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D) ||
		(_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D &&
			_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_BOX))
	{
		return IsCircleObbColl(_pFirst, _pSecond);
	}
	else if ((_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_RECT &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D) ||
		(_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D &&
			_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_RECT))
	{
		return IsCircleObbColl(_pFirst, _pSecond);
	}

	static Vec3 CollRect[2][4];
	if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_RECT &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_RECT)
	{
		for (int i = 0; i < 2; ++i)
		{
			CollRect[i][0] = Vec3(-0.5f, 0.5f, 0.f);
			CollRect[i][1] = Vec3(0.5f, 0.5f, 0.f);
			CollRect[i][2] = Vec3(0.5f, -0.5f, 0.f);
			CollRect[i][3] = Vec3(-0.5f, -0.5f, 0.f);
		}
	}
	else
	{
		return false;
	}

	/*static Vec3 CollRect[4] =
	{
		Vec3(-0.5f,0.f,0.f)
		Vec3(0.f,0.5f,0.f)
		Vec3(0.5f,0.f,0.f)
		Vec3(0.f,-0.5f,0.f)
	};*/
	//_pFirst->MeshRender();
	Vec3 vPoint[3] = {};
	Vec3 vSource[4] = {};
	Vec3 vProj[4] = {};

	Mtx mtxFirst = _pFirst->GetWorldMtx();
	Mtx mtxSecond = _pSecond->GetWorldMtx();

	vPoint[0] = XMVector3TransformCoord(CollRect[0][3], _pFirst->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(CollRect[0][0], _pFirst->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(CollRect[0][2], _pFirst->GetWorldMtx());

	vSource[0] = vPoint[2] - vPoint[0];
	vSource[1] = vPoint[1] - vPoint[0];

	vProj[0] = vSource[0]; vProj[0].Normalize();
	vProj[1] = vSource[1]; vProj[1].Normalize();

	vPoint[0] = XMVector3TransformCoord(CollRect[1][3], _pSecond->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(CollRect[1][0], _pSecond->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(CollRect[1][2], _pSecond->GetWorldMtx());

	vSource[2] = vPoint[2] - vPoint[0];
	vSource[3] = vPoint[1] - vPoint[0];

	vProj[2] = vSource[2]; vProj[2].Normalize();
	vProj[3] = vSource[3]; vProj[3].Normalize();

	Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, _pFirst->GetWorldMtx()) - XMVector3TransformCoord(Vec3::Zero, _pSecond->GetWorldMtx());

	for (int i = 0; i < 4; ++i)
	{
		float fDis = 0.f;
		float fCenter = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDis += abs(vSource[j].Dot(vProj[i]));
		}

		fDis *= 0.5f;
		fCenter = abs(vCenter.Dot(vProj[i]));

		if (fCenter >= fDis)
			return false;
	}

	return true;
}

bool CCollisionMgr::MouseCollision(Vec3* _CollRect, UINT _iSize,CTransForm* _pTrans)
{
	static Vec3 CollPos[4];
	CollPos[0] = Vec3(-0.5f, 0.5f, 0.f);
	CollPos[1] = Vec3(0.5f, 0.5f, 0.f);
	CollPos[2] = Vec3(0.5f, -0.5f, 0.f);
	CollPos[3] = Vec3(-0.5f, -0.5f, 0.f);

	vector<Vec3> vecPoint;
	Vec3 vSource[4];
	Vec3 vDir[4];

	for (size_t i = 0; i < _iSize; i++)
		vecPoint.push_back(XMVector3TransformCoord(_CollRect[i], _pTrans->GetWorldMtx())); // 여기서 좌표 충돌 계산하면 된다.

	vSource[0] = vecPoint[0] - vecPoint[3];
	vSource[1] = vecPoint[2] - vecPoint[3];

	vDir[0] = vSource[0];
	vDir[1] = vSource[1];

	vDir[0].Normalize();
	vDir[1].Normalize();

	for (size_t i = 0; i < 4; i++)
	{
		CollPos[i] = XMVector3TransformCoord(CollPos[i], CKeyMgr::GetInst()->GetMtxMouse());
	}

	vSource[2] = CollPos[0] - CollPos[3];
	vSource[3] = CollPos[2] - CollPos[3];

	vDir[2] = vSource[2];
	vDir[3] = vSource[3];

	vDir[2].Normalize();
	vDir[3].Normalize();

	Vec3 vMousePos = CKeyMgr::GetInst()->GetDXMousePos();
	Vec3 vTargetPos = XMVector3TransformCoord(Vec3::Zero, _pTrans->GetWorldMtx());

	Vec3 vCenter = vTargetPos - vMousePos;

	for (int i = 0; i < 4; ++i)
	{
		float fDis = 0.f;
		float fCenter = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDis += abs(vSource[j].Dot(vDir[i]));
		}

		fDis *= 0.5f;
		fCenter = abs(vCenter.Dot(vDir[i]));

		if (fCenter >= fDis)
			return false;
	}
	return true;
}