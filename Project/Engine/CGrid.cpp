#include "CGrid.h"
#include "CCollider.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "CSceneMgr.h"
#include "CCore.h"
#include "CTile.h"
#include "CLayer.h"
#include "CGameObj.h"
#include "CTransForm.h"
#include "CMeshRender.h"
#include "CResMgr.h"

CGrid::CGrid()
	:m_iCountX(10)
	, m_iCountY(10)
	, m_iOldCountX(10)
	, m_iOldCountY(10)
	, m_ptStart{}
	, m_ptEnd{}
{
	m_vecTileObj.resize(m_iCountY);
	m_eType = COMPONENT_TYPE::GRID;
}

CGrid::~CGrid()
{
	UINT iSizeY = (UINT)m_vecTileObj.size();
	UINT iSizeX = 0;
	if (iSizeY != 0)
		iSizeX = (UINT)m_vecTileObj[0].size();

	for (UINT i = 0; i < iSizeY; ++i)
	{
		for (UINT j = 0; j < iSizeX; ++j)
		{
			delete m_vecTileObj[i][j];
			m_vecTileObj[i][j] = nullptr;
		}
		m_vecTileObj[i].clear();
	}
	m_vecTileObj.clear();
}

void CGrid::Awake()
{
	ChangeTile();
}

int CGrid::Update()
{
	m_vTileSize = TransForm()->GetScale();

	if (m_iOldCountX != m_iCountX || m_iOldCountY != m_iCountY)
	{
		ChangeTile();
	}
	CurViewCalc();

	for (int i = m_ptStart.y; i < m_ptEnd.y + m_ptStart.y; ++i)
	{
		for (int j = m_ptStart.x; j < m_ptEnd.x + m_ptStart.x; ++j)
		{
			CGameObj* pObj = m_vecTileObj[i][j];

			float fPosX = (j * m_vTileSize.x) + ((i % 2) * (m_vTileSize.x / 2.f));
			float fPosY = (i * (m_vTileSize.y / 2.f));
			
			fPosX -= CCore::GetInst()->GetResolution().x / 2.f; //m_iCountX * TransForm()->GetScale().x / 2.f
			fPosY -= CCore::GetInst()->GetResolution().y / 2.f; //m_iCountY * TransForm()->GetScale().y / 4.f

			pObj->GetTransForm()->SetPos(Vec3(TransForm()->GetPos().x + fPosX, TransForm()->GetPos().y + fPosY, 100.f));
			if(!pObj->GetTile()->GetScale())
				pObj->GetTransForm()->SetScale(m_vTileSize);
			//pObj->GetCollider()->SetScale(m_vTileSize);

			m_vecTileObj[i][j]->Update();
		}
	}
	return 0;
}

void CGrid::Render()
{
	for (int i = m_ptStart.y; i < m_ptEnd.y + m_ptStart.y; ++i)
	{
		for (int j = m_ptStart.x; j < m_ptEnd.x + m_ptStart.x; ++j)
		{
			m_vecTileObj[i][j]->Render();
		}
	}
}

int CGrid::FinalUpdate()
{
	for (int i = m_ptStart.y; i < m_ptEnd.y + m_ptStart.y; ++i)
	{
		for (int j = m_ptStart.x; j < m_ptEnd.x + m_ptStart.x; ++j)
		{
			m_vecTileObj[i][j]->FinalUpdate();
		}
	}

	return 0;
}

void CGrid::ChangeTile()
{
	for (int i = 0; i < m_iOldCountY; ++i)
	{
		SafeArrayDelete(m_vecTileObj[i]);
	}
	m_vecTileObj.clear();

	m_vecTileObj.resize(m_iCountY);

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			CGameObj* pObj = new CGameObj;
			pObj->AddComponent(new CMeshRender);
			pObj->AddComponent(new CTile);
			pObj->GetTransForm()->SetScale(TransForm()->GetScale());
			pObj->SetID(CSceneMgr::GetInst()->GetLayerIndex(L"Tile"));
			pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"IsoMetricColl"));
			pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Coll"),0);

			m_vecTileObj[i].push_back(pObj);
			//pObj->AddComponent(new CCollider);
			/*pObj->GetCollider()->SetScale(TransForm()->GetScale());
			pObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_ISO);*/
		}
	}

	m_iOldCountX = m_iCountX;
	m_iOldCountY = m_iCountY;
}

void CGrid::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_iCountX, sizeof(int), 1, _pFile);
	fwrite(&m_iCountY, sizeof(int), 1, _pFile);

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			m_vecTileObj[i][j]->Save(_pFile);
		}
	}
}

void CGrid::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_iCountX, sizeof(int), 1, _pFile);
	fread(&m_iCountY, sizeof(int), 1, _pFile);

	m_iOldCountX = m_iCountX;
	m_iOldCountY = m_iCountY;

	m_vecTileObj.resize(m_iCountY);

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			CGameObj* pObj = new CGameObj;
			pObj->Load(_pFile);
			UINT iId = CSceneMgr::GetInst()->GetLayerIndex(L"Tile");
			pObj->SetID(iId);
			m_vecTileObj[i].push_back(pObj);
		}
	}

	//ChangeTile();
}

void CGrid::CurViewCalc()
{	
	if (m_vTileSize.x <= 0 || m_vTileSize.y <= 0)
		return;

	Vec3 vCamPos = -(g_World.mtxView.Translation());
	tResolution vResolution = CCore::GetInst()->GetResolution();

	m_ptEnd.x = (long)(vResolution.x / m_vTileSize.x);
	m_ptEnd.y = (long)((vResolution.y / m_vTileSize.y) * 2);

	Vec2 vGridSize = { m_vTileSize.x * m_iCountX,m_vTileSize.y * m_iCountY };

	m_ptStart.x = (long)(vCamPos.x / m_vTileSize.x);
	m_ptStart.y = (long)((vCamPos.y / m_vTileSize.y) * 2);

	if (m_ptStart.x + m_ptEnd.x >= m_iCountX)
		m_ptEnd.x = m_iCountX - m_ptStart.x;

	if (m_ptStart.y + m_ptEnd.y >= m_iCountY) // 현재 왼쪽으로 갓을경우 터진다.
		m_ptEnd.y = m_iCountY - m_ptStart.y;

	if (m_ptStart.x < 0)
		m_ptStart.x = 0;

	if (m_ptStart.y < 0)
		m_ptStart.y = 0;

	if (m_ptEnd.x < 0)
		m_ptEnd.x = 0;

	if (m_ptEnd.y < 0)
		m_ptEnd.y = 0;

}	
	
Vec3 CGrid::GetTransformed()
{	
	return Vec3(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f, 0.f);
}	
	
void CGrid::ChangeSize(UINT _iX, UINT _iY)
{
}