#include "CTileMgrScript.h"
#include "CTransForm.h"
#include "CGrid.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CTile.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CResMgr.h"

void CTileMgrScript::TileOutLineSearch(Vec3 _vPos, vector<POINT>& _vecPos, vector<Vec3>* _vecClosePos)
{
	POINT ptIdx = {};
	if (GetPickingTile(_vPos, ptIdx) == nullptr)
		return;

	m_vecTile = &CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj()[0]->GetGrid()->GetVecObj();
	
	/*CGameObj* pStar = CResMgr::GetInst()->PrefabToCreateObj(L"Default",L"Star");
	pStar->GetTransForm()->SetPos((*m_vecTile)[ptIdx.y][ptIdx.x]->GetTransForm()->GetPos());*/

	m_ptSize.x = (long)(*m_vecTile)[0].size();
	m_ptSize.y = (long)m_vecTile->size();

	m_listOpen.push_back(SEARCH{ ptIdx ,0});

	bool bOpenPush = false;

	while (true)
	{
		bOpenPush = false;
		ptIdx = m_listOpen.front().ptPos;
		(*m_vecTile)[ptIdx.y][ptIdx.x]->GetTile()->SetClose(true);
		
		if (ptIdx.y % 2 == 0) // 짝수일때
		{
			bOpenPush |= PuseOpenList(ptIdx.x - 1, ptIdx.y + 1); // 왼쪽위
			bOpenPush |= PuseOpenList(ptIdx.x - 1, ptIdx.y - 1); // 왼쪽아래
			bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y + 1);     // 오른쪽 위
			bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y - 1);     // 오른쪽 아래
		}
		else if (ptIdx.y % 2 == 1) // 홀수 일때
		{
			bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y + 1);										// 왼쪽위 // 왼쪽아래 // 오른쪽 위 // 오른쪽 아래
			bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y - 1);	   
			bOpenPush |= PuseOpenList(ptIdx.x + 1, ptIdx.y + 1);	   
			bOpenPush |= PuseOpenList(ptIdx.x + 1, ptIdx.y - 1);	   
		}

		bOpenPush |= PuseOpenList(ptIdx.x - 1, ptIdx.y); // 왼쪽
		bOpenPush |= PuseOpenList(ptIdx.x + 1, ptIdx.y); // 오른쪽
		bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y + 2);     // 위
		bOpenPush |= PuseOpenList(ptIdx.x, ptIdx.y - 2);     // 아래

		if (bOpenPush)
		{
			m_vecFinal.push_back(ptIdx);
		}

		m_vecClose.push_back((*m_vecTile)[ptIdx.y][ptIdx.x]);
		m_listOpen.pop_front();

		if (m_listOpen.empty())
			break;
	}

	for (size_t i = 0; i < m_vecClose.size(); i++)
		m_vecClose[i]->GetTile()->SetClose(false);
	
	if (_vecClosePos != nullptr)
	{
		for (size_t i = 0; i < m_vecClose.size(); i++)
		{
			Vec3 vPos = m_vecClose[i]->GetTransForm()->GetPos();
			vPos.z = 9999.f;
			_vecClosePos->push_back(vPos);
		}
	}

	m_vecClose.clear();

	for (size_t i = 0; i < m_vecFinal.size(); i++)
		_vecPos.push_back(m_vecFinal[i]);
}

void CTileMgrScript::TurnTile(Vec3 _vPos, vector<TURNTILE>& _vecClosePos)
{
	POINT ptIdx = {};
	if (GetPickingTile(_vPos, ptIdx) == nullptr)
		return;

	m_vecTile = &CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj()[0]->GetGrid()->GetVecObj();

	m_ptSize.x = (long)(*m_vecTile)[0].size();
	m_ptSize.y = (long)(m_vecTile->size());

	m_listOpen.push_back(SEARCH{ ptIdx,m_iIdx });
	bool bPush = false;

	while (true)
	{
		ptIdx = m_listOpen.front().ptPos;
		(*m_vecTile)[ptIdx.y][ptIdx.x]->GetTile()->SetClose(true);
		
		bPush = false;

		if (ptIdx.y % 2 == 0) // 짝수일때
		{
			if(!CollCheck(ptIdx.x - 1, ptIdx.y + 1)) // 왼쪽위
				bPush = OnlyPushOpenList(ptIdx.x - 1, ptIdx.y + 2);
			if(!CollCheck(ptIdx.x - 1, ptIdx.y - 1)) // 왼쪽아래
				bPush = OnlyPushOpenList(ptIdx.x - 1, ptIdx.y - 2);
			if(!CollCheck(ptIdx.x, ptIdx.y + 1))     // 오른쪽 위
				bPush = OnlyPushOpenList(ptIdx.x + 1, ptIdx.y + 2);
			if(!CollCheck(ptIdx.x, ptIdx.y - 1))     // 오른쪽 아래
				bPush = OnlyPushOpenList(ptIdx.x + 1, ptIdx.y - 2);
		}
		else if (ptIdx.y % 2 == 1) // 홀수 일때
		{
			if(!CollCheck(ptIdx.x, ptIdx.y + 1))									// 왼쪽위 // 왼쪽아래 // 오른쪽 위 // 오른쪽 아래
				bPush = OnlyPushOpenList(ptIdx.x - 1, ptIdx.y + 2);
			if(!CollCheck(ptIdx.x, ptIdx.y - 1))
				bPush = OnlyPushOpenList(ptIdx.x - 1, ptIdx.y - 2);
			if(!CollCheck(ptIdx.x + 1, ptIdx.y + 1))
				bPush = OnlyPushOpenList(ptIdx.x + 1, ptIdx.y + 2);
			if(!CollCheck(ptIdx.x + 1, ptIdx.y - 1))
				bPush = OnlyPushOpenList(ptIdx.x + 1, ptIdx.y - 2);
		}

		//PuseOpenList(ptIdx.x - 1, ptIdx.y + 2); // 왼쪽위
		//PuseOpenList(ptIdx.x - 1, ptIdx.y - 2); // 왼쪽아래
		//PuseOpenList(ptIdx.x + 1, ptIdx.y + 2);     // 오른쪽 위
		//PuseOpenList(ptIdx.x + 1, ptIdx.y - 2);     // 오른쪽 아래
		
		if (!CollCheck(ptIdx.x - 1, ptIdx.y))// 왼쪽
			bPush = OnlyPushOpenList(ptIdx.x - 2, ptIdx.y);
		if (!CollCheck(ptIdx.x + 1, ptIdx.y)) // 오른쪽
			bPush = OnlyPushOpenList(ptIdx.x + 2, ptIdx.y);
		if (!CollCheck(ptIdx.x, ptIdx.y + 2))
			bPush = OnlyPushOpenList(ptIdx.x, ptIdx.y + 4);     // 위
		if (!CollCheck(ptIdx.x, ptIdx.y - 2))
			bPush = OnlyPushOpenList(ptIdx.x, ptIdx.y - 4);     // 아래

		if (bPush)
			m_iIdx++;

		m_vecIdx.push_back(m_listOpen.front().iIdx);
		m_vecClose.push_back((*m_vecTile)[ptIdx.y][ptIdx.x]);
		m_listOpen.pop_front();

		if (m_listOpen.empty())
			break;
	}

	m_iIdx = 0;

	for (size_t i = 0; i < m_vecClose.size(); i++)
		m_vecClose[i]->GetTile()->SetClose(false);

	for (size_t i = 0; i < m_vecClose.size(); i++)
	{
		Vec3 vPos = m_vecClose[i]->GetTransForm()->GetPos();
		vPos.z = 9999.f;
		_vecClosePos.push_back(TURNTILE{ vPos,m_vecIdx[i]});
	}

	m_vecIdx.clear();
	m_vecClose.clear();
}

bool CTileMgrScript::PuseOpenList(int _iX, int _iY)
{
	if (_iX < 0 || _iY < 0 || 
		m_ptSize.x <= _iX || m_ptSize.y <= _iY)
		return false;
	if ((*m_vecTile)[_iY][_iX]->GetTile()->GetClose() == true)
		return false;

	if ((*m_vecTile)[_iY][_iX]->GetTile()->GetCollType() == COLL_TYPE::COLL)
		return true;

	m_listOpen.push_back(SEARCH{ POINT{ _iX,_iY },m_iIdx });

	(*m_vecTile)[_iY][_iX]->GetTile()->SetClose(true);
	return false;
}

bool CTileMgrScript::OnlyPushOpenList(int _iX, int _iY)
{
	if (_iX < 0 || _iY < 0 ||
		m_ptSize.x <= _iX || m_ptSize.y <= _iY)
		return false;
	if ((*m_vecTile)[_iY][_iX]->GetTile()->GetClose() == true)
		return false;

	if ((*m_vecTile)[_iY][_iX]->GetTile()->GetCollType() == COLL_TYPE::COLL)
		return false;

	m_listOpen.push_back(SEARCH{ POINT{ _iX,_iY },m_iIdx });

	(*m_vecTile)[_iY][_iX]->GetTile()->SetClose(true);
	return true;
}

bool CTileMgrScript::CollCheck(int _iX, int _iY)
{
	if (_iX < 0 || _iY < 0 || m_ptSize.x <= _iX || m_ptSize.y <= _iY)
		return true;
	if ((*m_vecTile)[_iY][_iX]->GetTile()->GetCollType() == COLL_TYPE::COLL)
		return true;

	return false;
}

CTileMgrScript::CTileMgrScript()
	:m_ptSize{}
	,m_iIdx(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::CTILEMGRSCRIPT);
}

CTileMgrScript::~CTileMgrScript()
{
}

CGameObj * CTileMgrScript::GetPickingTile(Vec3 _vPos, POINT& _ptIdx)
{
	Vec3 vRealPos = _vPos;
	Vec3 vViewPos = g_World.mtxView.Translation();

	if (CSceneMgr::GetInst()->GetLayer(L"Tile") == nullptr)
		return nullptr;

	vector<CGameObj*>& pTile = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();

	if (pTile.empty())
		return nullptr;

	CGameObj* pObj = pTile[0];
	if (pObj->GetGrid() == nullptr)
		return nullptr;

	vector<vector<CGameObj*>>& vecObj = pObj->GetGrid()->GetVecObj();

	if (vecObj.empty())
		return nullptr;

	Vec3 vScale = pObj->GetTransForm()->GetScale();

	vRealPos += pObj->GetGrid()->GetTransformed() - vViewPos;

	int iX = int((vRealPos.x + vScale.x / 2.f) / vScale.x);
	int iY = int((vRealPos.y + vScale.y / 2.f) / vScale.y) * 2;


	if (iX < 0)
		return nullptr;
	if (iY < 0)
		return nullptr;
	if (iX >= (int)pObj->GetGrid()->GetCountX())
		return nullptr;
	if (iY >= (int)pObj->GetGrid()->GetCountY())
		return nullptr;

	Vec3 vTargetPos = vecObj[iY][iX]->GetTransForm()->GetPos() + vViewPos;
	Vec2 vStart;
	Vec2 vEnd;
	float fGradient = 0.f;

	//y = ax + b
	if (_vPos.x < vTargetPos.x && _vPos.y < vTargetPos.y) // 왼쪽 아래
	{
		vStart = Vec2(vTargetPos.x - vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y - vScale.y / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y - vScale.y / 2.f; // 선위에 좌표Y
		float Y = -(fGra * vTargetPos.x) + fMoveY; // Y절편 구하기
		fGradient = fGra * _vPos.x + Y; // 최종 방정식

		if (_vPos.y < fGradient)
		{
			--iY;
			--iX;
		}
	}
	else if (_vPos.x < vTargetPos.x && _vPos.y >= vTargetPos.y) // 왼쪽 위
	{
		vStart = Vec2(vTargetPos.x - vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y + vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y + vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * _vPos.x + Y;

		if (_vPos.y > fGradient)
		{
			--iX;
			++iY;
		}
	}
	else if (_vPos.x >= vTargetPos.x && _vPos.y >= vTargetPos.y) // 오른쪽 위
	{
		vStart = Vec2(vTargetPos.x + vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y + vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y + vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * _vPos.x + Y;

		if (_vPos.y > fGradient)
		{
			++iY;
		}
	}
	else if (_vPos.x >= vTargetPos.x && _vPos.y < vTargetPos.y) //오른쪽 아래
	{
		vStart = Vec2(vTargetPos.x + vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y - vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y - vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * _vPos.x + Y;

		if (_vPos.y < fGradient)
		{
			--iY;
		}
	}

	if (iX < 0)
		return nullptr;
	if (iY < 0)
		return nullptr;
	if (iX >= (int)pObj->GetGrid()->GetCountX())
		return nullptr;
	if (iY >= (int)pObj->GetGrid()->GetCountY())
		return nullptr;

	_ptIdx.x = iX;
	_ptIdx.y = iY;

	return vecObj[iY][iX];
}