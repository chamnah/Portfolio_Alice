#include "CResMgr.h"
#include "CMesh.h"
#include "CPtr.h"
#include "CShaderMgr.h"
#include "CResMgr.h"
#include "CComponent.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CPathMgr.h"
#include "CMeshData.h"

CResMgr::CResMgr()
{
}


CResMgr::~CResMgr()
{
	SAFE_ARRAY_MAP_DELETE(m_mapRes, (UINT)RESOURCE_TYPE::END);
	SafeArrayDelete(m_vecClone);
}

void CResMgr::DefaultRectMesh()
{

	VTX vtxPoint = {};

	vtxPoint.vPos = Vec3(0.f, 0.f, 0.f);
	vtxPoint.vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	UINT iPt = 0;

	CMesh* Mesh = CMesh::CreateMesh(&vtxPoint, sizeof(VTX), 1, &iPt, sizeof(UINT), 1);
	AddResource<CMesh>(L"PointMesh", Mesh);

	VTX arrVTX[4] = {};

	arrVTX[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f,0.f);
	arrVTX[0].vNormal = Vec3(0.f,0.f,-1.f);

	arrVTX[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);
	arrVTX[1].vNormal = Vec3(0.f, 0.f, -1.f);

	arrVTX[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);
	arrVTX[2].vNormal = Vec3(0.f, 0.f, -1.f);

	arrVTX[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f,1.f);
	arrVTX[3].vNormal = Vec3(0.f, 0.f, -1.f);

	IDX arrIndex[2] =
	{
		0,1,2,
		0,2,3
	};

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"RectMesh",Mesh);

	UINT iIndex[5] ={	0,1,2,3,0};
	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, iIndex, sizeof(UINT), 5); //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	AddResource<CMesh>(L"CollRect", Mesh);

	arrVTX[0].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(1.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(1.f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"LeftRectMesh", Mesh);

	arrVTX[0].vPos = Vec3(-1.f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(-1.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"RightRectMesh", Mesh);

	arrVTX[0].vPos = Vec3(-0.5f, 0.f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.5f, 0.f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, iIndex, sizeof(UINT), 5); //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	AddResource<CMesh>(L"IsoMetricColl", Mesh);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"IsoMetric", Mesh);

	vector<VTX>  vecVTX;
	vector<UINT> vecId;

	UINT iSlice = 64;
	float fAngle = XM_2PI / (float)iSlice;

	VTX vtx;
	vtx.vPos = Vec3(0.f, 0.f, 0.f);
	vtx.vColor = Vec4(1.f,0.f,0.f,1.f);

	vecVTX.push_back(vtx);

	for (UINT i = 0; i < iSlice; ++i)
	{
		vtx.vPos = Vec3(cosf(fAngle * i) * 0.5f,sinf(fAngle * i) * 0.5f,0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX.push_back(vtx);
	}

	for (UINT i = 0; i < iSlice - 1; ++i)
	{
		vecId.push_back(0);
		vecId.push_back(i + 2);
		vecId.push_back(i + 1);
	}

	vecId.push_back(0);
	vecId.push_back(1);
	vecId.push_back(iSlice);

	Mesh = CMesh::CreateMesh(&vecVTX[0], sizeof(VTX), (int)vecVTX.size(), &vecId[0], sizeof(UINT), (int)vecId.size());
	AddResource<CMesh>(L"Circle", Mesh);

	vector<VTX> vecVTX2;
	vector<UINT> vecId2;

	for (UINT i = 0; i < iSlice; ++i)
	{
		vtx.vPos = Vec3(cosf(fAngle * i) * 0.5f, sinf(fAngle * i) * 0.5f, 0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX2.push_back(vtx);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecId2.push_back(i);
	}

	vecId2.push_back(0);

	Mesh = CMesh::CreateMesh(&vecVTX2[0], sizeof(VTX), (int)vecVTX2.size(), &vecId2[0], sizeof(UINT), (int)vecId2.size()); //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	AddResource<CMesh>(L"CollCircle", Mesh);

	VTX arrCube[24] = {};

	// À­¸é
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vTex = Vec2(0.25f, 0.34f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vTex = Vec2(0.5f, 0.34f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vTex = Vec2(0.5f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vTex = Vec2(0.25f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// ¾Æ·§ ¸é	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vTex = Vec2(0.25f, 1.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vTex = Vec2(0.5f, 1.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vTex = Vec2(0.5f, 0.66f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vTex = Vec2(0.25f, 0.66f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// ¿ÞÂÊ ¸é
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vTex = Vec2(0.25f, 0.34f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vTex = Vec2(0.f, 0.34f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vTex = Vec2(0.f, 0.66f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vTex = Vec2(0.25f, 0.66f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// ¿À¸¥ÂÊ ¸é  
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vTex = Vec2(0.75f, 0.34f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vTex = Vec2(0.5f, 0.34f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vTex = Vec2(0.5f, 0.66f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vTex = Vec2(0.75f, 0.66f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// ¾Õ ¸é
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vTex = Vec2(0.5f, 0.34f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vTex = Vec2(0.25f, 0.34f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vTex = Vec2(0.25f, 0.66f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vTex = Vec2(0.5f, 0.66f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// µÞ ¸é
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vTex = Vec2(1.f, 0.34f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vTex = Vec2(0.75f, 0.34f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vTex = Vec2(0.75f, 0.66f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vTex = Vec2(1.f, 0.66f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	IDX iIdx[12] = {};

	for (UINT i = 0; i < 12; i += 2)
	{
		iIdx[i] = IDX{ i * 2, i * 2 + 1, i * 2 + 2 };
		iIdx[i + 1] = IDX{i * 2, i * 2 + 2, i * 2 + 3};
	}

	Mesh = CMesh::CreateMesh(arrCube, sizeof(VTX), 24, iIdx, sizeof(UINT), 36);
	AddResource<CMesh>(L"SkyCube", Mesh);

	// À­¸é
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vTex = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vTex = Vec2(1.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vTex = Vec2(1.f, 1.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vTex = Vec2(0.f, 1.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// ¾Æ·§ ¸é	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vTex = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vTex = Vec2(1.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vTex = Vec2(1.f, 1.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vTex = Vec2(0.f, 1.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// ¿ÞÂÊ ¸é
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vTex = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vTex = Vec2(1.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vTex = Vec2(1.f, 1.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vTex = Vec2(0.f, 1.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// ¿À¸¥ÂÊ ¸é  
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vTex = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vTex = Vec2(1.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vTex = Vec2(1.f, 1.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vTex = Vec2(0.f, 1.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// ¾Õ ¸é
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vTex = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vTex = Vec2(1.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vTex = Vec2(1.f, 1.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vTex = Vec2(0.f, 1.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// µÞ ¸é
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vTex = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vTex = Vec2(1.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vTex = Vec2(1.f, 1.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vTex = Vec2(0.f, 1.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	Mesh = CMesh::CreateMesh(arrCube, sizeof(VTX), 24, iIdx, sizeof(UINT), 36);
	AddResource<CMesh>(L"CubeMesh", Mesh);

	// Sphere

	vecVTX.clear();
	vector<UINT> vecIDX;
	VTX v;

	float fRadius = 1.f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vTex = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	// Body
	UINT iStackCount = 40; // °¡·Î ºÐÇÒ °³¼ö
	UINT iSliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vTex = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVTX.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vTex = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVTX.push_back(v);

	// ÀÎµ¦½º
	// ºÏ±ØÁ¡
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIDX.push_back(0);
		vecIDX.push_back(i + 2);
		vecIDX.push_back(i + 1);
	}

	// ¸öÅë
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIDX.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIDX.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIDX.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIDX.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIDX.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIDX.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ³²±ØÁ¡
	UINT iBottomIdx = UINT(vecVTX.size() - 1);
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIDX.push_back(iBottomIdx);
		vecIDX.push_back(iBottomIdx - (i + 2));
		vecIDX.push_back(iBottomIdx - (i + 1));
	}

	Mesh = CMesh::CreateMesh(&vecVTX[0], sizeof(VTX), (int)vecVTX.size(), &vecIDX[0], sizeof(UINT), (int)vecIDX.size());
	AddResource<CMesh>(L"SphereMesh", Mesh);

	VTX arrPoint = {};
	UINT iPoint = 0;

	Mesh = CMesh::CreateMesh(&arrPoint, sizeof(VTX), 1, &iPoint, sizeof(UINT), 1);
	AddResource<CMesh>(L"Stream", Mesh);
	Mesh->CreateStreamVB(10000);

	Mesh = CMesh::CreateMesh(&arrPoint, sizeof(VTX), 20, &iPoint, sizeof(UINT), 1);
	AddResource<CMesh>(L"StreamMesh", Mesh);
}
	
void CResMgr::GridMesh(int _iVertexX, int _iVertexZ, int _iWidth, int _iDepth)
{	
	UINT vertexCount = _iVertexX * _iVertexZ;           // ÃÑ Á¤Á¡ °¹¼ö
	UINT faceCount = (_iVertexX - 1)*(_iVertexZ - 1) * 2; // ÃÑ »ï°¢Çü °¹¼ö *2 ¾ÈÇÏ¸é »ç°¢Çü °¹¼ö

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*_iWidth;
	float halfDepth = 0.5f*_iDepth;

	float dx = float(_iWidth / (_iVertexX - 1)); // °¡·Î °£°Ý
	float dz = float(_iDepth / (_iVertexZ - 1)); // ±íÀÌ ¹æÇâ °£°Ý


	vector<VTX> vecVTX;
	vecVTX.resize(vertexCount);

	for (int i = 0; i < _iVertexZ; ++i)
	{
		float z = halfDepth - i * dz;
		for (int j = 0; j < _iVertexX; ++j)
		{
			float x = -halfWidth + j * dx;
			float y = 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
			vecVTX[i*_iVertexX + j].vPos = Vec3(x, y, z);
			vecVTX[i*_iVertexX + j].vColor = Vec4(1.f, 0.f, 1.f,1.f);
			vecVTX[i*_iVertexX + j].vTex = Vec2((float)j,(float)i);
		}
	}

	vecVTX[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	
	//
	// Create the indices.
	//

	vector<IDX> vecIDX;
	vecIDX.resize(faceCount);

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (int i = 0; i < _iVertexZ - 1; ++i)
	{
		for (int j = 0; j < _iVertexX - 1; ++j)
		{
			vecIDX[k].first = i * _iVertexX + j;
			vecIDX[k].second = i * _iVertexX + j + 1;
			vecIDX[k].third = (i + 1)*_iVertexX + j;

			vecIDX[k + 1].first = (i + 1)*_iVertexX + j;
			vecIDX[k + 1].second = i * _iVertexX + j + 1;
			vecIDX[k + 1].third = (i + 1)*_iVertexX + j + 1;

			k += 2; // next quad
		}
	}

	CMesh* Mesh = CMesh::CreateMesh(&vecVTX[0], sizeof(VTX), (int)vecVTX.size(), &vecIDX[0], sizeof(IDX), (int)vecIDX.size());
	AddResource<CMesh>(L"Grid", Mesh);
}

void CResMgr::GetDeleteClone(CMaterial * _pMtrl)
{
	vector<CMaterial*>::iterator iter = m_vecClone.begin();
	for (; iter != m_vecClone.end(); ++iter)
	{
		if (*iter == _pMtrl)
		{
			delete *iter;
			m_vecClone.erase(iter);
			break;
		}
	}

}

CGameObj * CResMgr::PrefabToCreateObj(wstring _wcsLayer, wstring _wcsPrefabName)
{
	CPrefab* pPrefab = Load<CPrefab>(_wcsPrefabName);
	CGameObj* pObj = pPrefab->Instantiate();

	UINT iIdx = CSceneMgr::GetInst()->GetLayerIndex(_wcsLayer);
	tEvent event = tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iIdx,(INT_PTR)pObj };
	CEventMgr::GetInst()->AddEvent(event);

	return pObj;
}
void CResMgr::RemoveResource(const wstring & _wcsFind, RESOURCE_TYPE _eType)
{
	map<wstring, CRes*>::iterator ResIter = m_mapRes[(UINT)_eType].find(_wcsFind);

	if (ResIter == m_mapRes[(UINT)_eType].end())
		assert(NULL);

	delete ResIter->second;
	ResIter->second = nullptr;
	m_mapRes[(UINT)_eType].erase(ResIter);
}


void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Color"));
	AddResource<CMaterial>(L"Color", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Coll"));
	AddResource<CMaterial>(L"Coll", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Std"));
	AddResource<CMaterial>(L"Std", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Spectrum"));
	AddResource<CMaterial>(L"Spectrum", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"TexColor"));
	AddResource<CMaterial>(L"TexColor", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"SkyBox"));
	AddResource<CMaterial>(L"SkyBox", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Planet"));
	AddResource<CMaterial>(L"Planet", pMtrl);

	/*pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Light"));
	AddResource<CMaterial>(L"Light", pMtrl);*/

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"3d_Deferred"));
	AddResource<CMaterial>(L"3d_Deferred", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Merge"));
	AddResource<CMaterial>(L"Merge", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"PointLight"));
	AddResource<CMaterial>(L"PointLight", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"DirLight"));
	AddResource<CMaterial>(L"DirLight", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Volume"));
	AddResource<CMaterial>(L"Volume", pMtrl);


	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Vision"));
	AddResource<CMaterial>(L"Vision", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Shadow"));
	AddResource<CMaterial>(L"Shadow", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"PointShadow"));
	AddResource<CMaterial>(L"PointShadow", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Terrain"));
	AddResource<CMaterial>(L"Terrain", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Particle"));
	AddResource<CMaterial>(L"Particle", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"AddRain"));
	AddResource<CMaterial>(L"AddRain", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"DrawRain"));
	AddResource<CMaterial>(L"DrawRain", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Compute"));
	AddResource<CMaterial>(L"Compute", pMtrl); 
	
	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Picking"));
	AddResource<CMaterial>(L"Picking", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"TexClear"));
	AddResource<CMaterial>(L"TexClear", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Weight"));
	AddResource<CMaterial>(L"Weight", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"AddTrail"));
	AddResource<CMaterial>(L"AddTrail", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Trail"));
	AddResource<CMaterial>(L"Trail", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Billboard"));
	AddResource<CMaterial>(L"Billboard", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"AddTrailParticle"));
	AddResource<CMaterial>(L"AddTrailParticle", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"TrailParticle"));
	AddResource<CMaterial>(L"TrailParticle", pMtrl);
	pMtrl->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Decal"));
	AddResource<CMaterial>(L"Decal", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Forward"));
	AddResource<CMaterial>(L"Forward", pMtrl); 

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"PostEffect"));
	AddResource<CMaterial>(L"PostEffect", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"DistortionSphere"));
	AddResource<CMaterial>(L"DistortionSphere", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"HorBlur"));
	AddResource<CMaterial>(L"HorBlur", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"VerBlur"));
	AddResource<CMaterial>(L"VerBlur", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Blur"));
	AddResource<CMaterial>(L"Blur", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Fade"));
	AddResource<CMaterial>(L"Fade", pMtrl);
}

void CResMgr::SoundInit()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::SoundPlay(const wstring & _wcsSound,int _iCount, bool _bRepeat)
{
	CSound* pSound = CResMgr::GetInst()->Load<CSound>(_wcsSound);

	if (pSound != nullptr)
	{
		if (!_bRepeat)
		{
			if (pSound->IsPlaying())
				return;
		}
		pSound->Play(1);
	}
	else
	{
		wstring wcsPath = CPathMgr::GetInst()->GetPath();
		wcsPath += L"Sound\\" + _wcsSound;
		wcsPath += L".ogg";
		pSound = new CSound;
		pSound->Load(wcsPath, _wcsSound);
		CResMgr::GetInst()->AddResource<CSound>(_wcsSound, pSound);
		pSound->Play(_iCount);
	}
}

CTexture* CResMgr::CreateTexture(const wstring _strKey,UINT _iWidth, UINT _iHeight, UINT _iFlag, DXGI_FORMAT _eFormat,D3D11_USAGE _eUsage)
{
	CTexture* pTex = CTexture::CreateTexture(_iWidth,_iHeight,_iFlag,_eFormat, _eUsage);
	m_mapRes[(UINT)RESOURCE_TYPE::TEXTURE].insert(make_pair(_strKey, pTex));
	pTex->SetName(_strKey);
	return pTex;
}

CTexture * CResMgr::CreateTexture(const wstring _strKey, ID3D11Texture2D * _pTex2D)
{
	CTexture* pTex = CTexture::CreateTexture(_pTex2D);
	m_mapRes[(UINT)RESOURCE_TYPE::TEXTURE].insert(make_pair(_strKey, pTex));
	return pTex;
}

CTexture * CResMgr::LoadAndAddTexture(const wstring _strKey, const wstring _strFileName)
{
	CTexture* pTex = Load<CTexture>(_strKey);
	if (pTex == nullptr)
	{
		wstring strPath = CPathMgr::GetInst()->GetPath();
		strPath += L"Texture\\";
		strPath += _strFileName;
		pTex = CTexture::CreateTexture(strPath);
		CResMgr::GetInst()->AddResource<CTexture>(_strKey, pTex);
	}
	return pTex;
}

CMeshData * CResMgr::LoadFBX(const wstring _strPath)
{
	CMeshData* pMeshData = new CMeshData;
	pMeshData->LoadFBX(_strPath);
	AddResource<CMeshData>(pMeshData->GetName(), pMeshData);

	return pMeshData;
}

CMeshData * CResMgr::LoadMeshData(const wstring _strPath)
{
	CMeshData* pMeshData = CMeshData::LoadMeshDataFile(_strPath);
	AddResource<CMeshData>(pMeshData->GetName(), pMeshData);
	return pMeshData;
}