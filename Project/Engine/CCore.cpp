#include "CCore.h"
#include "CDirect.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CResMgr.h"
#include "CShaderMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"
#include "CGameObj.h"
#include "CPlayerMgr.h"
#include "CCamera.h"
#include "CInstBuffer.h"
//#include "FontEngine.h"
//#include "extensions/PxDefaultAllocator.h"
//#include "extensions/PxDefaultErrorCallback.h"
//#include "PxFoundation.h"
//#include "PxPhysicsAPI.h"
//#include "mfapi.h"
//#pragma comment(lib,"Mfplat")

//using namespace physx;
//#pragma comment(lib,"PhysX_64")
//#pragma comment(lib,"PhysXCommon_64")
//#pragma comment(lib,"PhysXCooking_64")
//#pragma comment(lib,"PhysXExtensions_static_64")
//#pragma comment(lib,"PhysXFoundation_64")
//#pragma comment(lib,"PhysXPvdSDK_static_64")        
//#pragma comment(lib,"LowLevel_static_64")
//#pragma comment(lib,"LowLevelAABB_static_64")
//#pragma comment(lib,"LowLevelDynamics_static_64")
//#pragma comment(lib,"PhysXCharacterKinematic_static_64")
//#pragma comment(lib,"PhysXTask_static_64")
//#pragma comment(lib,"PhysXVehicle_static_64")
//IGraphBuilder *pGraph;
//IMediaControl * pControl;
//IMediaEvent   *pEvent;

//CGameObj* g_pGameObj = nullptr;
//PxScene*       m_pScene = nullptr;
tGlobal g_Global = {};

CCore::CCore()
{
}

CCore::~CCore()
{
	SAFE_RELEASE(textFormat);
	SAFE_RELEASE(wFactory);
	SAFE_RELEASE(Rt);
	SAFE_RELEASE(brush);
	SAFE_RELEASE(factory);

	/*pControl->Release();
	pEvent->Release();
	pGraph->Release();*/
	//CoUninitialize();
}

void CCore::Init(const tResolution& _res, HWND _hwnd, bool _bTool)
{
	//FbxManager* pFbx;
	//if (_bTool == true)
	//{
	//	RECT rt = {};
	//	GetClientRect(_hwnd, &rt);
	//	m_tResolution.x = rt.right - rt.left;
	//	m_tResolution.y = rt.bottom - rt.top; // 툴용으로 만듬
	//}
	//else
	//PhysxInit();

	g_Global.fWidth = _res.x;
	g_Global.fHeight = _res.y;

	m_tResolution = _res;
	CDirect::GetInst()->Init(_res,_hwnd);
	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init(_hwnd);
	CShaderMgr::GetInst()->Init();
	CInstBuffer::GetInst()->Init();
	CKeyMgr::GetInst()->SetHandle(_hwnd);

	//CResMgr::GetInst()->GridMesh(50,50,160,160);
	CResMgr::GetInst()->DefaultRectMesh();
	CResMgr::GetInst()->CreateDefaultMaterial();
	CResMgr::GetInst()->SoundInit();
	CRenderMgr::GetInst()->Init(_res.x, _res.y, _hwnd);

	if (FAILED(D2DInit(_hwnd, _res.x, _res.y, &factory, &Rt)))
	{
		MessageBox(_hwnd, TEXT("Direct2D 초기화 실패"), TEXT("에러"), MB_ICONERROR);
		Clean();
		return;
	}

	if (!Setup())
	{
		MessageBox(_hwnd, TEXT("Setup 실패"), TEXT("에러"), MB_ICONERROR);
		Clean();
		return;
	}
	
	CDirect::GetInst()->CreateConstBuffer(L"TransForm",sizeof(TranformMatrix),0);
	CDirect::GetInst()->CreateConstBuffer(L"ShaderParam",sizeof(ShaderParam),1);
	CDirect::GetInst()->CreateConstBuffer(L"AnimParam", sizeof(tAnimParam), 2);
	CDirect::GetInst()->CreateConstBuffer(L"LightInfo", sizeof(tLightInfo), 3);
	CDirect::GetInst()->CreateConstBuffer(L"Global", sizeof(tGlobal), 4);
	CDirect::GetInst()->CreateConstBuffer(L"Target", sizeof(tTarget), 5);


	CSceneMgr::GetInst()->Init();

	/*TextInfo Info;
	Info.wcsText = L"게임시작";
	Info.vLeftTop = Vec2(100,100);
	Info.vRightBottom = Vec2(300, 200);
	m_vecText.push_back(Info);*/

	/*CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	IBaseFilter* pBF = NULL;
	IGraphBuilder* g_graph;
	IBaseFilter* g_filter = NULL;
	IVMRFilterConfig9* filterConfig;
	HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL,CLSCTX_INPROC,IID_IBaseFilter,(LPVOID *)&pBF);

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&g_graph);
	CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&g_filter);
	g_filter->QueryInterface(IID_IVMRFilterConfig9, reinterpret_cast<void**>(&filterConfig));

	DEVICE->STRE
	filterConfig->SetRenderingMode(VMR9Mode_Renderless);
	filterConfig->SetNumberOfStreams(2);*/
	/*pGraph = NULL;
	pControl = NULL;
	pEvent = NULL;*/

	// Initialize the COM library.
	//HRESULT hr;// CoInitialize(NULL);
	//if (FAILED(hr))
	//{
	//	printf("ERROR - Could not initialize COM library");
	//	return;
	//}

	/*HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	wstring wcsPath = CPathMgr::GetInst()->GetPath();
	wcsPath += L"Logo.wmv";
	pGraph->RenderFile(wcsPath.c_str(), NULL);

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	IVideoWindow *pVidWin = NULL;
	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
	pVidWin->put_Owner((OAHWND)_hwnd);
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVidWin->SetWindowPosition(0, 0, 800, 600);
	pVidWin->put_Visible(OATRUE);
	*/

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	
	/*m_hVideo = MCIWndCreate(_hwnd, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD, wcsPath.c_str());

	MoveWindow(m_hVideo, 0, 0, 800, 600, NULL);
	MCIWndPlay(m_hVideo);

	MCIWndEnd(m_hVideo);*/
}

int CCore::Update()
{
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();
	CSceneMgr::GetInst()->Update();
	CRenderMgr::GetInst()->FinalUpdate();
	CEventMgr::GetInst()->Update();
	CPlayerMgr::GetInst()->Update();

	return 0;
}

void CCore::Render()
{
	CRenderMgr::GetInst()->Clear();
	CRenderMgr::GetInst()->Render();
	
	/*Rt->BeginDraw();
	TextInfo text;
	wchar_t szTime[10] = L"";
	_itow_s(CTimeMgr::GetInst()->GetFPS(), szTime,10);
	text.wcsText = szTime;
	text.vLeftTop = Vec2(m_tResolution.x - 100.f, 50);
	text.vRightBottom = Vec2(m_tResolution.x, 150);
	m_vecText.push_back(text);

	for (size_t i = 0; i < m_vecText.size(); i++)
	{
		D2D1_POINT_2F pt;
		pt.x = ((m_vecText[i].vRightBottom.x - m_vecText[i].vLeftTop.x) * 0.5f) + m_vecText[i].vLeftTop.x;
		pt.y = ((m_vecText[i].vRightBottom.y - m_vecText[i].vLeftTop.y) * 0.5f) + m_vecText[i].vLeftTop.y;
		Rt->SetTransform(D2D1::Matrix3x2F::Rotation(45.f, pt));
		Rt->DrawTextW(
			m_vecText[i].wcsText.c_str(),
			(UINT32)m_vecText[i].wcsText.length(),
			textFormat,
			D2D1::RectF(m_vecText[i].vLeftTop.x, m_vecText[i].vLeftTop.y, m_vecText[i].vRightBottom.x, m_vecText[i].vRightBottom.y),
			brush
		);
	}
	Rt->EndDraw();*/

	CCamera* pMain = CRenderMgr::GetInst()->GetMainCam();
	if (pMain != nullptr)
	{
		g_World.mtxView = pMain->GetMtxView();
		g_World.mtxProj = pMain->GetMtxProj();
		for (size_t i = 0; i < m_vecOutObj.size(); i++)
		{
			m_vecOutObj[i]->Render();
		}
	}
	m_vecOutObj.clear();

	/*m_pScene->simulate(1.0f / 60.0f);
	m_pScene->fetchResults(true);*/

	CRenderMgr::GetInst()->Present();

	// Run the graph.
	//HRESULT hr = pControl->Run();
	//if (SUCCEEDED(hr))
	//{
	//	// Wait for completion.
	//	long evCode;
	//	pEvent->WaitForCompletion(INFINITE, &evCode);

	//	// Note: Do not use INFINITE in a real application, because it
	//	// can block indefinitely.
	//}
	m_vecText.clear();
}

HRESULT CCore::D2DInit(HWND hWnd, int width, int height,
	ID2D1Factory **factory, ID2D1RenderTarget **Rt)
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory);
	if (FAILED(hr))
		return hr;
	//(*factory)->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height)), Rt);
	//(*factory)->CreateDxgiSurfaceRenderTarget()
	IDXGISurface* pBackBuffer = nullptr;
	CRenderMgr::GetInst()->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	FLOAT dpiX;
	FLOAT dpiY;
	//(*factory)->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	hr = (*factory)->CreateDxgiSurfaceRenderTarget(
		pBackBuffer,
		&props,
		Rt
	);

	pBackBuffer->Release();

	return hr;
}

bool CCore::Setup()
{
	if (FAILED(DWInit(L"맑은 고딕", 30.0f, &wFactory, &textFormat)))
		return false;

	Rt->CreateSolidColorBrush(D2D1::ColorF(0.f, 203.f / 255.f, 247.f / 255.f), &brush);

	return true;
}

HRESULT CCore::DWInit(LPCWSTR fontname, float size,
	IDWriteFactory **factory, IDWriteTextFormat **format)
{
	HRESULT hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*factory),
		(IUnknown**)factory
	);
	if (FAILED(hr))
		return hr;

	hr = (*factory)->CreateTextFormat(
		fontname,
		0,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		format
	);

	return hr;
}

void CCore::Clean()
{
	SAFE_RELEASE(textFormat);
	SAFE_RELEASE(wFactory);
	SAFE_RELEASE(Rt);
	SAFE_RELEASE(brush);
	SAFE_RELEASE(factory);
}

/*
void CCore::PhysxInit()
{
	PxDefaultAllocator  gDefaultAllocatorCallback;

	PxDefaultErrorCallback    gDefaultErrorCallback;

	PxFoundation* mFoundation;
	PxPhysics*    mPhysics;
	PxCooking*    mCooking;
	PxPvd*		  mPvd;
	PxCudaContextManager* gCudaContextManager = nullptr;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	if (!mFoundation)
		assert(NULL);

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	bool recordMemoryAllocations = true;
	PxTolerancesScale scale;

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, scale, recordMemoryAllocations, mPvd);

	if (!mPhysics)
		assert(NULL);

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(PxTolerancesScale()));
	if (!mCooking)
		assert(NULL);

	if (!PxInitExtensions(*mPhysics, mPvd))
		assert(NULL);

	PxDefaultCpuDispatcher* Dispatcher = PxDefaultCpuDispatcherCreate(1);

	PxSceneDesc desc(mPhysics->getTolerancesScale());
	desc.gravity = PxVec3(0.f, -9.8f, 0.f);
	desc.cpuDispatcher = Dispatcher;
	desc.filterShader = PxDefaultSimulationFilterShader;

	//PxCudaContextManagerDesc cudaContextManagerDesc;

	//gCudaContextManager = PxCreateCudaContextManager(*mFoundation, cudaContextManagerDesc, PxGetProfilerCallback());
	m_pScene = mPhysics->createScene(desc);

	/*m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 3);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1);*/
	/*
	PxMaterial* pMtrl = mPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *pMtrl);
	PxSphereGeometry Sphere;

	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();

	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_pScene->addActor(*groundPlane);

	//m_pScene->addActor(Sphere);
	
	//groundPlane->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	//m_pScene->addActor(*groundPlane);

	/*PxMaterial* aMaterial;
	PxScene* aScene;111
	PxRigidDynamic* aCapsuleActor = mPhysics->createRigidDynamic(PxTransform(PxVec3(0,0,0)));
	PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxShape* aCapsuleShape = PxRigidActorExt::createExclusiveShape(*aCapsuleActor,
		PxCapsuleGeometry(10, 10), *aMaterial);
	aCapsuleShape->setLocalPose(relativePose);
	PxRigidBodyExt::updateMassAndInertia(*aCapsuleActor, capsuleDensity);
	aScene->addActor(aCapsuleActor);*/

	/*PxShape* pShape;
	PxSphereGeometry sphere;
	sphere.radius = 100.f;

	PxScene* scene;
	PxSceneDesc desc(scale);
	desc.gravity = PxVec3(0.f,5.f,0.f);
	scene = mPhysics->createScene(desc);

	float mAccumulator = 0.0f;
	float mStepSize = 1.0f / 60.0f;

	mAccumulator += DT;
	if (mAccumulator >= mStepSize)
	{
		mAccumulator -= mStepSize;
		scene->simulate(mStepSize);
	}*/
	//PxMaterial mtrl;

	//mPhysics->createShape(sphere,);

	/*PxCloseExtensions();

	mPhysics->release();
	mCooking->release();
	mPvd->release();

	mFoundation->release();*/
//}

/*
	Rt->BeginDraw();
	Rt->Clear();
	for (size_t i = 0; i < m_vecText.size(); i++)
	{
		Rt->DrawTextW(
			m_vecText[i].wcsText.c_str(),
			m_vecText[i].wcsText.length(),
			textFormat,
			D2D1::RectF(m_vecText[i].vLeftTop.x, m_vecText[i].vLeftTop.y, m_vecText[i].vRightBottom.x, m_vecText[i].vRightBottom.y),
			brush
		);
	}
	Rt->EndDraw();

*/