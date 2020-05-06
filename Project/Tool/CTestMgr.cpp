#include "stdafx.h"
#include "CTestMgr.h"
#include "CGameObj.h"
#include "CCamera.h"
#include "CSceneMgr.h"
#include "CCameraScript.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CLight.h"
#include "CTransForm.h"
#include "CCarScript.h"
#include "CMesh.h"
#include "CAnimator2D.h"
#include "CRenderMgr.h"
#include "CButtonUIScript.h"
#include "CTerrain.h"
#include "CCore.h"
#include "CGizmoScript.h"
#include "CCollider.h"
#include "CMeshData.h"
#include "CGameCameraScript.h"
#include "CCameraScript.h"
#include "CSaveLoadMgr.h"
#include "CEventMgr.h"
#include "CPathMgr.h"
#include "CText.h"
#include "TimeMgr.h"
#include "CAnimator3D.h"
void CTestMgr::Init()
{
	wstring strName = L"";
	size_t length = strName.length();
	//CResMgr::GetInst()->AddResource<CTexture>(L"Suppressor",CTexture::CreateTexture(L"..\\content\\Texture\\Suppressor.png"));

	/*CMeshData* pData = CResMgr::GetInst()->LoadFBX(L"D:\\Choi\\Game_Release\\content\\Mesh\\DoctorOfficeBottom.FBX");
	CGameObj* pMeshDataObj = pData->instantiate();
	pMeshDataObj->SetName(L"DoctorOfficeBottom");
	pMeshDataObj->SetFrustum(false);
	CSceneMgr::GetInst()->AddObject(L"Default", pMeshDataObj);

	pData = CResMgr::GetInst()->LoadFBX(L"D:\\Choi\\Game_Release\\content\\Mesh\\DoctorOfficeFront.FBX");
	pMeshDataObj = pData->instantiate();
	pMeshDataObj->SetName(L"DoctorOfficeFront");
	pMeshDataObj->SetFrustum(false);
	CSceneMgr::GetInst()->AddObject(L"Default", pMeshDataObj);*/

	CGameObj* pObj = new CGameObj;
	pObj->AddComponent(new CCamera);
	CCameraScript* pCamScript = new CCameraScript;
	pCamScript->SetName(L"CameraScript");
	pObj->AddComponent(pCamScript);

	pObj->SetName(L"MainCamera");
	pObj->GetCamera()->SetPerspective(true);
	pObj->GetCamera()->SetMainCam(true);

	CSceneMgr::GetInst()->AddObject(L"Camera",pObj);
	/*CMeshData* pData = CResMgr::GetInst()->LoadMeshData(L"D:\\Choi\\Game_Release\\content\\Mesh\\MeshData\\Grunt_Mini.mdat");

	Vec4 vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			pObj = pData->instantiate();
			pObj->SetName(L"GameObject");
			pObj->GetTransForm()->SetPos(i * 100,-float(j * 200),0.f);
			pObj->GetAnimator3D()->FileLoad(L"D:\\Choi\\Game_Release\\content\\Anim\\Grunt_mini.anim");
			pObj->GetAnimator3D()->PlayAnim(L"attack01_A",true,true);
			CSceneMgr::GetInst()->AddObject(L"Default", pObj);
		}
	}*/

	/*CGameObj* pChild = new CGameObj;
	pChild->SetName(L"GameObject");
	pChild->GetTransForm()->SetScale(Vec3(50, 50, 1.f));

	pChild->AddComponent(new CMeshRender);
	pChild->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pChild->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred")->Clone());
	pChild->GetMeshRender()->SetParam(&vColor, SHADER_PARAM::MTRL);
	pObj->AddChild(pChild);*/

	/*pObj = new CGameObj;
	pObj->SetName(L"GameObject");
	pObj->GetTransForm()->SetScale(Vec3(100, 100, 1.f));

	pObj->AddComponent(new CMeshRender);
	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pObj->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"Forward")->Clone());
	vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	pObj->GetMeshRender()->SetParam(&vColor, SHADER_PARAM::MTRL);
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);*/

	pObj = new CGameObj;
	pObj->SetName(L"Text");
	pObj->GetTransForm()->SetPos(Vec3(70.f, 0.f, 0.f));
	pObj->GetTransForm()->SetScale(Vec3(70.f, 0.f, 0.f));
	CText* text = new CText;
	pObj->AddComponent(text);
	text->ChangeColor(Vec4(1.f, 0.f, 0.f, 1.f));
	text->SetSize(30.f);
	text->ChangeFontAndSize();
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	CTimeMgr::GetInst()->SetFPSObj(pObj);

	pObj = new CGameObj;
	pObj->SetName(L"Text");
	pObj->GetTransForm()->SetPos(Vec3(140.f, 30.f, 0.f));
	pObj->GetTransForm()->SetScale(Vec3(200.f, 0.f, 0.f));
	text = new CText;
	pObj->AddComponent(text);
	text->ChangeColor(Vec4(1.f, 0.f, 0.f, 1.f));
	text->SetSize(30.f);
	text->ChangeFontAndSize();
	text->SetText(L"Instancing On");
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	CTimeMgr::GetInst()->SetInstObj(pObj);

	/*pObj = new CGameObj;
	pObj->SetName(L"Sphere");
	pObj->AddComponent(new CMeshRender);

	pObj->GetTransForm()->SetScale(Vec3(100, 100, 100));
	pObj->GetTransForm()->SetPos(0.f, 100.f, -100.f);

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pObj->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred")->Clone(),0);

	CSceneMgr::GetInst()->AddObject(L"Default", pObj);

	pObj = new CGameObj;
	pObj->SetName(L"Sphere");
	pObj->AddComponent(new CMeshRender);

	pObj->GetTransForm()->SetScale(Vec3(100, 100, 100));
	pObj->GetTransForm()->SetPos(0.f, 150.f, -300.f);

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pObj->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred")->Clone(), 0);

	CSceneMgr::GetInst()->AddObject(L"Default", pObj);*/

	/*pObj = new CGameObj;
	pObj->AddComponent(new CCamera);

	pObj->SetName(L"UICamera");
	pObj->GetCamera()->SetPerspective(false);

	CSceneMgr::GetInst()->AddObject(L"Default", pObj);*/

	tLightColor  color = {};
	color.vDiff = Vec4(1.f,1.f,1.f,0.f);
	color.vAmb = Vec4(0.1f, 0.1f, 0.1f, 0.f);
	color.vSpec = Vec4(0.4f, 0.4f, 0.4f, 0.f);

	pObj = new CGameObj;
	pObj->SetName(L"Light");
	pObj->AddComponent(new CLight);

	/*pObj->GetLight()->SetLightType((int)LIGHT_TYPE::POINT);
	pObj->GetLight()->SetLightColor(color);
	pObj->GetLight()->SetDistance(2000.f);
	pObj->GetTransForm()->SetPos(Vec3(0.f, 500.f, -500.f));*/
	
	pObj->GetLight()->SetLightType((int)LIGHT_TYPE::DIR);
	pObj->GetLight()->SetLightColor(color);
	pObj->GetLight()->SetDistance(1300.f);
	pObj->GetLight()->SetDirection(Vec3(1.f, -1.f, 1.f));
	pObj->GetTransForm()->SetPos(Vec3(-1000.f, 1000.f, -1000.f));
	
	//pObj->GetLight()->SetDirection(Vec3(1.f,-1.f,1.f));
	//pObj->GetTransForm()->SetRotaion(Vec3(XM_PIDIV4, XM_PIDIV4,0.f));
	//pObj->GetLight()->SetAngle(0.99f);
	//pObj->GetLight()->SetDistance(1000.f);
	//pObj->GetTransForm()->SetPos(Vec3(0.f, 300.f, 0.f));

	CSceneMgr::GetInst()->AddObject(L"Light", pObj);
	
	/*pObj = new CGameObj;
	pObj->SetName(L"Plane");
	pObj->GetTransForm()->SetScale(Vec3(1000, 1000, 1000));
	pObj->GetTransForm()->SetPos(0.f, 100.f, 100.f);
	pObj->AddComponent(new CMeshRender);
	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pObj->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred")->Clone(), 0);
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);*/

	/*pObj = new CGameObj;
	pObj->SetName(L"Sphere");
	pObj->AddComponent(new CMeshRender);

	pObj->GetTransForm()->SetScale(Vec3(100, 100, 100));
	pObj->GetTransForm()->SetPos(0.f, 0.f, 0.f);

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred"), 0);

	CSceneMgr::GetInst()->AddObject(L"Player", pObj);*/

	m_pGizmo = new CGameObj;
	for (size_t i = 0; i < 3; i++)
	{
		m_pGizmoXYZ[i] = new CGameObj;
		m_pGizmoXYZ[i]->SetName(L"Gizmo");
		m_pGizmoXYZ[i]->GetTransForm()->SetScale(Vec3(100.f, 100.f, 100.f));
		m_pGizmoXYZ[i]->AddComponent(new CMeshRender);
		CGizmoScript* pScript = new CGizmoScript;
		pScript->SetDirType((DIR_TYPE)i);
		m_pGizmoXYZ[i]->AddComponent(pScript);
		m_pGizmoXYZ[i]->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"));
		m_pGizmoXYZ[i]->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Color"), 0);

		Vec3 vPos;
		vPos[i] = 300.f;
		m_pGizmoXYZ[i]->GetTransForm()->SetPos(vPos);

		Vec4 vColor = {};
		vColor[i] = 1.f;
		m_pGizmoXYZ[i]->GetMeshRender()->GetMtrl()->SetParam(&vColor, (SHADER_PARAM)((UINT)SHADER_PARAM::VEC4_0));
		m_pGizmo->AddChild(m_pGizmoXYZ[i]);
	}

	CTexture* pTex = nullptr;
	/*CGameObj* pFilter = new CGameObj;
	pFilter->AddComponent(new CMeshRender);
	pFilter->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"));
	pFilter->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"Decal")->Clone());
	pFilter->GetMeshRender()->SetParam(CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::POSITION),SHADER_PARAM::TEX_0);
	pFilter->GetMeshRender()->GetMtrl()->SetDepthStencil(CRenderMgr::GetInst()->GetDepthStencil(DEPTH_STENCIL_TYPE::NO_NO));
	pFilter->GetMeshRender()->GetMtrl()->SetRasterizeState(RASTERIZE_TYPE::CULL_FRONT);
	pFilter->GetMeshRender()->SetDeferred(false);

	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Texture\\Bubble.jpg";
	CTexture* pTex = CTexture::CreateTexture(strPath);
	CResMgr::GetInst()->AddResource<CTexture>(L"Bubble", pTex);

	pFilter->GetMeshRender()->SetParam(pTex, SHADER_PARAM::TEX_1);
	int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pFilter });*/ // Filter Component에 들어있는 View Matrix를 안넣어서 문제가 생김
	/*CGameObj* pDecal = CRenderMgr::GetInst()->FindFilter(L"Decal")->Clone();
	int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pDecal });*/

	/*CGameObj* pFilter = CRenderMgr::GetInst()->FindFilter(L"Decal")->Clone();
	int iLayer = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iLayer,(INT_PTR)pFilter });*/

	return;
	pObj = new CGameObj;
	pObj->SetName(L"Terrain");
	pObj->GetTransForm()->SetPos(Vec3(0.f, 0.f, 0.f));

	pObj->AddComponent(new CMeshRender);
	CTerrain* pTerrain = new CTerrain;
	pObj->AddComponent(pTerrain);
	pTerrain->Awake();

	pObj->GetTransForm()->SetScale(Vec3(50.f, 100.f, 50.f));
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);

	
	//pObj->GetTransForm()->SetRotaion(Vec3(XM_PIDIV2,0.f,0.f));
	/*pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"TessRectMesh"));
	pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Tess"));*/
	//pObj->GetMeshRender()->GetMtrl()->SetRasterizeState(RASTERIZE_TYPE::WIREFRAME);
	pTex = CTexture::CreateTexture(L"..\\content\\Texture\\8k_mars.jpg");
	CResMgr::GetInst()->AddResource<CTexture>(L"GeoMetry", pTex);

	CCarScript* pCarScript = new CCarScript;
	CGameObj* pPObj = new CGameObj;
	pPObj->SetName(L"Parent");
	pPObj->AddComponent(new CMeshRender);
	pPObj->GetTransForm()->SetScale(Vec3(50, 50, 50.f));
	pPObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pPObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred"),0);
	CSceneMgr::GetInst()->AddObject(L"Default", pPObj);

	pObj = new CGameObj;
	pObj->SetName(L"Cube");
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider);
	pObj->AddComponent(pCarScript);
	//pCarScript->Start();

	pObj->GetTransForm()->SetPos(Vec3(0.f,300.f,0.f));
	pObj->GetTransForm()->SetScale(Vec3(150, 150, 150.f));

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"));
	pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred"),0);

	pPObj->AddChild(pObj);

	pObj = new CGameObj;
	pObj->SetName(L"Cube");
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider);
	//pCarScript->Start();

	pObj->GetTransForm()->SetPos(Vec3(0.f, 300.f, 500.f));
	pObj->GetTransForm()->SetScale(Vec3(150, 150, 150.f));

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"));
	pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"3d_Deferred"),0);

	CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	//CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	CButtonUIScript* pButton = new CButtonUIScript;
	pButton->SetCollType(BUTTON_COLL::HEXAGON);

	pObj = new CGameObj;
	pObj->SetName(L"UI");
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(pButton);

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pObj->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"Std"),0);
	pObj->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));
	CSceneMgr::GetInst()->AddObject(L"UI", pObj);

	//CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Particle");
	//pMtrl->SetParam(pTex,SHADER_PARAM::TEX_0);

	// 파티클 객체
	CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"AddRain");
	CMaterial* pSubMtrl = CResMgr::GetInst()->Load<CMaterial>(L"DrawRain");

	pObj = new CGameObj;
	pObj->SetName(L"Particle");
	pObj->AddComponent(new CMeshRender);

	pObj->GetTransForm()->SetScale(Vec3(100,100,1.f));

	pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"Stream"));
	pObj->GetMeshRender()->SetMtrl(pMtrl,0);
	pObj->GetMeshRender()->SetSubMtrl(pSubMtrl);

	CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	//m_pMouse = new CGameObj; // Mouse Layer에다가 충돌체만 쓰려고 만들어서 실제로 레이어에 넣지는 않앗다.
	//m_pMouse->SetName(L"Mouse");
	//m_pMouse->SetID(CSceneMgr::GetInst()->GetLayerIndex(L"Mouse"));
	//m_pMouse->AddComponent(new CMeshRender);
	//m_pMouse->AddComponent(new CCollider);
	//m_pMouse->AddComponent(new CToolScript);
	//
	//m_pMouse->GetCollider()->SetScale(Vec3(50.f,50.f,1.f));
	//m_pMouse->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_ISO);

	//m_pMouse->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//m_pMouse->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));

	//CSceneMgr::GetInst()->AddObject(L"Mouse", m_pMouse);
	// 사운드 
}

void CTestMgr::Update()
{
	if (CSaveLoadMgr::GetLoading() == true)
		return;
	if (!m_bPicking)
	{
		m_pTargetGizmo = nullptr;
		float fDist = 0.f;
		float fCompare = 0.f;
		bool bFirst = true;
		CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

		if (pMainCam == nullptr)
			return;
		tRay& Ray = pMainCam->GetRay();

		for (size_t i = 0; i < 3; i++)
		{
			m_Box.Center = m_pGizmoXYZ[i]->GetTransForm()->GetWorldMtx().Translation();
			m_Box.Extents = m_pGizmoXYZ[i]->GetTransForm()->GetScale();
			if (m_Box.Intersects(Ray.vStart, Ray.vDir, fDist))
			{
				if (bFirst) // 처음 피킹된 경우
				{
					m_pTargetGizmo = m_pGizmoXYZ[i];
					bFirst = false;
					fCompare = fDist;
				}
				else if (fCompare < fDist) // 처음이 아닌경우고 거리가 더 짧은 경우
				{
					fCompare = fDist;
					m_pTargetGizmo = m_pGizmoXYZ[i];
				}
			}
		}
	}

	if (m_pTargetGizmo != nullptr)
		m_pTargetGizmo->Update();

	//m_pGizmo->Update();
	if (m_pGizmo->GetParent() != nullptr && m_pGizmo->GetParent()->GetDead() == true)
		m_pGizmo->SetParent(nullptr);
	m_pGizmo->FinalUpdate();

	for (size_t i = 0; i < 3; i++)
		CCore::GetInst()->PushOutObject(m_pGizmoXYZ[i]);

	if (CEventMgr::GetInst()->GetChangeScene())
	{
		CSceneMgr::GetInst()->SetDelObj(false);
		CSceneMgr::GetInst()->SetPlay(false);
		CCamera* pCam = CRenderMgr::GetInst()->GetMainCam();
		pCam->TransForm()->SetPos(Vec3(850.f, 700.f, 0.f));//20
		pCam->TransForm()->SetRotation(Vec3(XMConvertToRadians(20.f), 0.f, 0.f));
		pCam->GetOwner()->SetParent(nullptr);
		CSaveLoadMgr::SetChangeScene(true);
		wstring strPath = L"..\\content\\Scene\\";
		strPath += CEventMgr::GetInst()->GetSceneName() + L".scene";
		CSaveLoadMgr::SetPath(strPath);
		CEventMgr::GetInst()->OffChangeScene();
		CResMgr::GetInst()->Load<CSound>(L"Ch4_Queen_Entrance")->Stop();
		CResMgr::GetInst()->SoundPlay(L"Chap3_Mountains", -1);
	}

	//m_pTarget = nullptr;
	//m_pMouse->Update();
}

void CTestMgr::Render()
{
	/*m_pGizmo->Render();
	for (size_t i = 0; i < 3; i++)
		m_pGizmoXYZ[i]->Render();*/
	
	//m_pMouse->Render();
}

void CTestMgr::UpdateData()
{
	//if(!m_bStop)
	//m_pMouse->FinalUpdate();
}

void CTestMgr::SetTargetObj(CGameObj * _pTarget)
{
	m_pTarget = _pTarget;
	if(m_pTarget != nullptr && m_pTarget->GetDead())
		m_pTarget = nullptr;
	m_pGizmo->SetParent(m_pTarget);
}

CTestMgr::CTestMgr()
	:m_pTarget(nullptr)
	, m_eCollType(COLL_TYPE::NOCOLL)
	, m_pGizmo(nullptr)
	, m_pGizmoXYZ{}
	, m_bPicking(false)
	, m_pTargetGizmo(nullptr)
{
}

CTestMgr::~CTestMgr()
{
	SAFE_DELETE(m_pGizmo);
	//delete m_pMouse;
}