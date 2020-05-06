#include "CTerrain.h"
#include "CMesh.h"
#include "CResMgr.h"
#include "CMeshRender.h"
#include "KeyMgr.h"
#include "CDirect.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTransForm.h"

CTerrain::CTerrain()
	:m_iFaceX(10),
	m_iFaceY(10),
	m_pCSMtrl(nullptr),
	m_pPickMtrl(nullptr),
	m_pWeightMtrl(nullptr),
	m_eBrushType(BRUSH_TYPE::HEIGHT),
	m_vBrushSize(0.1f,0.1f),
	m_pArrTex(nullptr),
	m_pTerrainMesh(nullptr),
	m_pWeightTex(nullptr),
	m_iGroupWidth(1),
	m_iGroupHeight(512)
{
	m_vTeselRange = Vec2(200.f, 600.f);
	m_eType = COMPONENT_TYPE::TERRAIN;
	vecWeightMap.resize(4);
	for (size_t i = 0; i < vecWeightMap.size(); i++)
	{
		vecWeightMap[i] = nullptr;
	}
}

CTerrain::~CTerrain()
{
	SAFE_DELETE(m_pArrTex);
	SAFE_DELETE(m_pTerrainMesh);
	SAFE_DELETE(m_pWeightTex);
}

void CTerrain::Awake()
{
	//CTexture* pTex = CTexture::CreateTexture(L"..\\content\\Texture\\Height\\Film_Set_SciFi_Terrain_01_H.tga", D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	CTexture* pTex = CTexture::CreateTexture(512,512, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResMgr::GetInst()->AddResource<CTexture>(L"HeightMap", pTex);

	m_iFaceX = (pTex->GetWidth() / 16) + 1;
	m_iFaceY = (pTex->GetHeight() / 16) + 1;

	CreateMesh();
	CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Terrain");
	MeshRender()->SetMtrl(pMtrl,0);

	//pTex = CTexture::CreateTexture(, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);

	pMtrl->SetParam(pTex,SHADER_PARAM::TEX_0);
	pMtrl->SetParam(&Vec2(m_iFaceX, m_iFaceY), SHADER_PARAM::VEC2_2);

	Vec2 vTexel = {1.f / (m_iFaceX * 16.f),1.f / (m_iFaceY * 16.f) };

	pMtrl->SetParam(&vTexel.x, SHADER_PARAM::FLOAT_0);
	pMtrl->SetParam(&vTexel.y, SHADER_PARAM::FLOAT_1);
	pMtrl->SetParam(&m_vBrushSize, SHADER_PARAM::VEC2_1);

	CreateComputeShader();//D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST

	CTexture* pWeightTex = CResMgr::GetInst()->Load<CTexture>(L"WeightMap");
	pMtrl->SetParam(pWeightTex,SHADER_PARAM::TEX_2);

	pMtrl->SetRasterizeState(RASTERIZE_TYPE::WIREFRAME);

	vector<CTexture*> vecTex;
	vecTex.push_back(CTexture::CreateTexture(L"..\\content\\Texture\\Tile\\TILE_01.tga", D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT));
	CResMgr::GetInst()->AddResource<CTexture>(L"arrTex0", vecTex[0]);
	vecWeightMap[0] = vecTex[0];
	vecTex.push_back(CTexture::CreateTexture(L"..\\content\\Texture\\Tile\\TILE_02.tga", D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT));
	CResMgr::GetInst()->AddResource<CTexture>(L"arrTex1", vecTex[1]);
	vecWeightMap[1] = vecTex[1];
	CTexture* pArrTex = CTexture::CreateTexture(vecTex, D3D11_BIND_SHADER_RESOURCE);
	CResMgr::GetInst()->AddResource<CTexture>(L"TextureMap", pArrTex);
	pMtrl->SetParam(pArrTex, SHADER_PARAM::ARRTEX_0);
	SetHeightMap(pTex);

	SetTeselRange(m_vTeselRange);
}

int CTerrain::Update()
{
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCam();
	if (pCam)
	{
		Vec4 vCamPos = Vec4(pCam->TransForm()->GetWorldPos(), 1.f);
		MeshRender()->GetMtrl()->SetParam(&vCamPos, SHADER_PARAM::VEC4_0);
	}
	Vec3 vPos = {};
	if (!Picking(vPos))
		return 0;

	Vec2 vUV = Vec2(vPos.x / m_iFaceX, 1 - (vPos.z / m_iFaceY)); // Picking한 터레인 전체를 하나의 UV로써 판단해서 만듬

	MeshRender()->GetMtrl()->SetParam(&vUV, SHADER_PARAM::VEC2_0);
	if (KEYHOLD(KEY_TYPE::KEY_LBTN))
	{
		if (m_eBrushType == BRUSH_TYPE::HEIGHT)
		{
			m_pCSMtrl->SetParam(&vUV, SHADER_PARAM::VEC2_0);
			m_pCSMtrl->Excute(m_iGroupWidth, m_iGroupHeight, 1);
		}
		else if (m_eBrushType == BRUSH_TYPE::WEIGHT)
		{
			m_pWeightMtrl->SetParam(&vUV, SHADER_PARAM::VEC2_0);
			m_pWeightMtrl->Excute(m_iGroupWidth, m_iGroupHeight, 1);
		}
	}

	return 0;
}

void CTerrain::CreateMesh()
{
	VTX vtx;
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	for (UINT i = 0; i <= m_iFaceY; i++)
	{
		for (UINT j = 0; j <= m_iFaceX; j++)
		{
			vtx.vPos = Vec3(j,0.f, i);
			vtx.vColor = Vec4(1.f, 0.f, 1.f,1.f);
			vtx.vNormal = Vec3(0.f, 1.f, 0.f);
			vtx.vTex = Vec2((float)j / (float)m_iFaceX, 1.f - ((float)i / (float)m_iFaceY));
			vtx.vTangent = Vec3(1.f,0.f,0.f);
			vtx.vBinormal = Vec3(0.f,0.f,-1.f);
			vecVtx.push_back(vtx);
		}
	}
	int iVertexX = m_iFaceX + 1;

	for (UINT i = 0; i < m_iFaceY; i++)
	{
		for (UINT j = 0; j < m_iFaceX; j++)
		{
			vecIdx.push_back((j + 1) + i * iVertexX);//     ↑
			vecIdx.push_back(j + i * iVertexX);      //	    ↑
			vecIdx.push_back(j + (i + 1) * iVertexX);//      ←←←

			vecIdx.push_back(j + (i + 1) * iVertexX);		 //   →→→
			vecIdx.push_back((j + 1) + (i + 1) * iVertexX);	 //        ↓
			vecIdx.push_back((j + 1) + i * iVertexX);		 //        ↓

			//vecIdx.push_back(j + i * iVertexX);             //   →→→
			//vecIdx.push_back(j + (i + 1) * iVertexX);       //  ↑
			//vecIdx.push_back((j + 1) + (i + 1) * iVertexX); //  ↑

			//vecIdx.push_back(j + i * iVertexX);             //      ↗
			//vecIdx.push_back((j + 1) + (i + 1) * iVertexX); //    ↗ ↓
			//vecIdx.push_back((j + 1) + i * iVertexX);       //  ↗   ↓
		}
	}

	SAFE_DELETE(m_pTerrainMesh);
	m_pTerrainMesh = CMesh::CreateMesh(&vecVtx[0], sizeof(VTX), vecVtx.size(), &vecIdx[0], sizeof(UINT), vecIdx.size());
	//CMesh* pMesh = CMesh::CreateMesh(&vecVtx[0],sizeof(VTX), vecVtx.size(),&vecIdx[0],sizeof(UINT) , vecIdx.size());
	//CResMgr::GetInst()->AddResource<CMesh>(L"TerrainMesh",pMesh);
	MeshRender()->SetMesh(m_pTerrainMesh);
}

void CTerrain::CreateComputeShader()
{
	// Height Map 수정하는 Compute Shader
	m_pCSMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Compute");
	CTexture* pHeightTex = CResMgr::GetInst()->Load<CTexture>(L"HeightMap");
	m_pCSMtrl->SetParam(pHeightTex,SHADER_PARAM::RWTEX_0);
	m_pCSMtrl->SetParam(&m_vBrushSize, SHADER_PARAM::VEC2_1);
	// 나의 마우스가 피킹된 좌표를 구하는 Com Shader;

	m_pPickMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Picking");
	CTexture* pTex = CTexture::CreateTexture(1,1, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResMgr::GetInst()->AddResource<CTexture>(L"Picking",pTex);
	m_pPickMtrl->SetParam(pTex,SHADER_PARAM::RWTEX_0);
	m_pPickMtrl->SetParam(&Vec2(m_iFaceX, m_iFaceY), SHADER_PARAM::VEC2_0);

	m_pWeightTex = CTexture::CreateTexture(512, 512, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pWeightMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Weight");
	m_pWeightMtrl->SetParam(m_pWeightTex, SHADER_PARAM::RWTEX_0);
	m_pWeightMtrl->SetParam(&m_vBrushSize, SHADER_PARAM::VEC2_1);

	CResMgr::GetInst()->AddResource<CTexture>(L"WeightMap", m_pWeightTex);
	UINT iColor = 0;
	m_pWeightMtrl->SetParam(&iColor, SHADER_PARAM::INT_0);
}

bool CTerrain::Picking(Vec3& _vPos)
{
	if (CRenderMgr::GetInst()->GetMainCam() == nullptr)
		return false;

	tRay& Ray = CRenderMgr::GetInst()->GetMainCam()->GetRay();

	Vec4 vLocalPos = XMVector3TransformCoord(Ray.vStart, TransForm()->GetMtxInvWorld());
	Vec4 vLocalDir = XMVector3TransformCoord(Ray.vDir, TransForm()->GetMtxInvWorld());
	vLocalDir.Normalize();

	m_pPickMtrl->SetParam(&vLocalPos, SHADER_PARAM::VEC4_0);
	m_pPickMtrl->SetParam(&vLocalDir, SHADER_PARAM::VEC4_1);

	int iGroup = ((m_iFaceX * m_iFaceY * 2) / 512) + 1;


	static CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(L"Picking");
	pTex->Clear(Vec4(-1.f, 0.f, 0.f, 0.f));

	m_pPickMtrl->Excute(iGroup, 1, 1);
	Vec3* pData = (Vec3*)pTex->GetPixel();
	_vPos = *pData;

	if (KEYTAB(KEY_TYPE::KEY_LBTN))
	{
		int i = 0;
	}

	if (_vPos.x == -1)
		return false;

	return true;
}

void CTerrain::SetBrushTexture(CTexture * _pTex)
{
	m_pBrushTex = _pTex;
	m_pCSMtrl->SetParam(m_pBrushTex, SHADER_PARAM::TEX_0);
	m_pWeightMtrl->SetParam(m_pBrushTex, SHADER_PARAM::TEX_0);
	MeshRender()->GetMtrl()->SetParam(m_pBrushTex, SHADER_PARAM::TEX_1);
}

void CTerrain::SetBrushSize(float _fSize)
{
	m_vBrushSize = Vec2(_fSize, _fSize);
	
	MeshRender()->GetMtrl()->SetParam(&m_vBrushSize,SHADER_PARAM::VEC2_1);
	m_pCSMtrl->SetParam(&m_vBrushSize, SHADER_PARAM::VEC2_1);
	m_pWeightMtrl->SetParam(&m_vBrushSize, SHADER_PARAM::VEC2_1);
}

void CTerrain::SetWeightTexture(UINT _iIdx, CTexture * _pTex)
{
	vecWeightMap[_iIdx] = _pTex;

	vector<CTexture*> vecTemp;
	for (size_t i = 0; i < vecWeightMap.size(); i++)
	{
		if(vecWeightMap[i] != nullptr)
			vecTemp.push_back(vecWeightMap[i]);
	}

	SAFE_DELETE(m_pArrTex);
	m_pArrTex = CTexture::CreateTexture(vecTemp, D3D11_BIND_SHADER_RESOURCE);

	MeshRender()->GetMtrl()->SetParam(m_pArrTex, SHADER_PARAM::ARRTEX_0);
}

void CTerrain::SetSelectWeight(UINT iIdx)
{
	m_pWeightMtrl->SetParam(&iIdx, SHADER_PARAM::INT_0);
}

void CTerrain::SetHeightMap(CTexture * _pTex)
{
	Vec2 vTexel = { 1.f / (m_iFaceX * 16.f),1.f / (m_iFaceY * 16.f) };
	m_iFaceX = (_pTex->GetWidth() / 16) + 1;
	m_iFaceY = (_pTex->GetHeight() / 16) + 1;

	m_iGroupWidth = _pTex->GetWidth() / 513 + 1;;
	m_iGroupHeight = _pTex->GetHeight();

	m_vHeightMapSize = Vec2(_pTex->GetWidth(), _pTex->GetHeight());
	m_pCSMtrl->SetParam(&m_vHeightMapSize, SHADER_PARAM::VEC2_2);
	MeshRender()->SetParam(_pTex, SHADER_PARAM::TEX_0);
	MeshRender()->SetParam(&Vec2(m_iFaceX, m_iFaceY), SHADER_PARAM::VEC2_2);
	MeshRender()->SetParam(&vTexel.x, SHADER_PARAM::FLOAT_0);
	MeshRender()->SetParam(&vTexel.y, SHADER_PARAM::FLOAT_1);

	CreateMesh();

	m_pPickMtrl->SetParam(&Vec2(m_iFaceX, m_iFaceY), SHADER_PARAM::VEC2_0);
	m_pCSMtrl->SetParam(_pTex, SHADER_PARAM::RWTEX_0);

	CResMgr::GetInst()->DeleteResource<CTexture>(L"WeightMap");
	m_pWeightTex = CTexture::CreateTexture(_pTex->GetWidth(), _pTex->GetHeight(), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pWeightMtrl->SetParam(m_pWeightTex, SHADER_PARAM::RWTEX_0);
	m_pWeightMtrl->SetParam(&m_vHeightMapSize, SHADER_PARAM::VEC2_2);
	CResMgr::GetInst()->AddResource<CTexture>(L"WeightMap", m_pWeightTex);
	MeshRender()->GetMtrl()->SetParam(m_pWeightTex, SHADER_PARAM::TEX_2);
}

void CTerrain::SetTeselRange(Vec2& _vRange)
{
	m_vTeselRange = _vRange;
	MeshRender()->GetMtrl()->SetParam(&m_vTeselRange.x, SHADER_PARAM::FLOAT_2);
	MeshRender()->GetMtrl()->SetParam(&m_vTeselRange.y, SHADER_PARAM::FLOAT_3);
}