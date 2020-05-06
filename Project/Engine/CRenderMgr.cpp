#include "CRenderMgr.h"
#include "CBlendState.h"
#include "CDepthStencilState.h"
#include "CResMgr.h"
#include "CRT.h"
#include "CMRT.h"
#include "CDirect.h"
#include "CCamera.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CSceneMgr.h"
#include "CLight.h"
#include "CFilterCom.h"
#include "CDeferredScript.h"
#include "CConstBuffer.h"
#include "CTransForm.h"
#include "KeyMgr.h"
#include "CPathMgr.h"
#include "CCore.h"

CRenderMgr::CRenderMgr()
	:m_arrBlend{}
	,m_arrDepthStencil{}
	,m_pSwap(nullptr)
	,m_pVisionFilter(nullptr)
	, m_bCollView(false)
	, m_pTextRT(nullptr)
	, m_pFactory(nullptr)
	, m_bText(true)
{
	CreateBlend();
	CreateDepthStencil();
	CreateRasterize();
	CreateSampler();
}

CRenderMgr::~CRenderMgr()
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		delete m_arrBlend[i];
		m_arrBlend[i] = nullptr;
	}

	for (UINT i = 0; i < (UINT)DEPTH_STENCIL_TYPE::END; ++i)
	{
		delete m_arrDepthStencil[i];
		m_arrDepthStencil[i] = nullptr;
	}

	for (size_t i = 0; i < (UINT)RASTERIZE_TYPE::END; i++)
	{
		if(m_arrRasterize[i] != nullptr)
		m_arrRasterize[i]->Release();
	}

	for (size_t i = 0; i < (UINT)SAMPLER_TYPE::END; i++)
	{
		if (m_arrSampler[i] != nullptr)
			m_arrSampler[i]->Release();
		
	}

	SAFE_RELEASE(m_pSwap);
	SAFE_ARRAY_DELETE(m_pRT, (UINT)RT_TYPE::END);
	SAFE_ARRAY_DELETE(m_pMRT, (UINT)MRT_TYPE::END);
	SafeMapDelete(m_mapFilter);

	SAFE_RELEASE(m_pFactory);
	SAFE_RELEASE(m_pTextRT);
}

void CRenderMgr::Init(UINT _iWidth,UINT _iHeight,HWND _hwnd)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	m_hWnd = _hwnd;

	CreateSwap();
	CreateMultiRenderTarget();
	CreateFilter();
	D2DInit(&m_pFactory,&m_pTextRT);
}

void CRenderMgr::FinalUpdate()
{
	m_pMergeFilter->Update();
	m_pMergeFilter->FinalUpdate();

}

void CRenderMgr::Render()
{
	if (KEYTAB(KEY_TYPE::KEY_ALT))
		m_bCollView = !m_bCollView;

	if (m_pMainCam == nullptr)
		return;
	UINT iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iCount; i++)
	{
		CTexture::RegisterClear(i);
	}

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"Global");

	pBuffer->UpdateData(&g_Global, sizeof(tGlobal));
	pBuffer->UpdateRegisterAll();


	m_pMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();

	for (size_t i = 0; i < m_vecCam.size(); i++)
	{
		m_vecCam[i]->SortGameObject();
		m_vecCam[i]->DeferredRender();
	}

	//m_pMRT[(UINT)MRT_TYPE::SHADOW]->OMSet();
	for (size_t i = 0; i < m_vecLight.size(); i++)
	{
		m_vecLight[i]->ShadowRender();
	}

	m_pMRT[(UINT)MRT_TYPE::OUTPUT]->OMSet();
	m_pMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	g_World.mtxView = m_pMainCam->GetMtxView();
	g_World.mtxProj = m_pMainCam->GetMtxProj();

	static int iShadowSel = 0;
	for (size_t i = 0; i < m_vecLight.size(); i++)
	{
		m_vecLight[i]->Render();
	}

	m_pMRT[(UINT)MRT_TYPE::OUTPUT]->OMSet();

	if (KEYTAB(KEY_TYPE::KEY_F2))
	{
		iShadowSel++;
		if (iShadowSel >= m_vecLight.size())
			iShadowSel = 0;
	}

	if (!m_vecLight.empty())
		m_pMergeFilter->GetFilter()->GetMaterial()->SetParam(m_vecLight[iShadowSel]->GetShadowTexture(SHADOW_RT::SHADOW_0), SHADER_PARAM::TEX_5);
	else
		m_pMergeFilter->GetFilter()->GetMaterial()->SetParam(NULL, SHADER_PARAM::TEX_5);

	m_pMergeFilter->GetFilter()->Render();

	for (size_t i = 0; i < m_vecCam.size(); i++)
	{
		m_pMRT[(UINT)MRT_TYPE::OUTPUT]->OMSet();
		m_vecCam[i]->ForwardRender();
		CONTEXT->CopyResource(m_pRT[(UINT)RT_TYPE::SWAP]->GetTexture()->GetTexture2D(), m_pRT[(UINT)RT_TYPE::OUTPUT]->GetTexture()->GetTexture2D());
		m_pMRT[(UINT)MRT_TYPE::SWAP]->OMSet();
		m_vecCam[i]->PostEffectRender();
	}
	

	if (m_bCollView)
	{
		for (size_t i = 0; i < m_vecCam.size(); i++)
			m_vecCam[i]->CollRender();
	}
	if (m_bText)
	{
		m_pTextRT->BeginDraw();
		for (size_t i = 0; i < m_vecCam.size(); i++)
			m_vecCam[i]->TextRender();
		m_pTextRT->EndDraw();
	}
	m_vecCam.clear();
	m_vecLight.clear();
}

void CRenderMgr::CreateSwap()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = m_iWidth; // 후면버퍼 너비
	sd.BufferDesc.Height = m_iHeight; // 후면버퍼 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 디스플레이 화면 갱신율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 디스플레이 화면 갱신율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 후면버퍼 픽셀 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 스캔라인모드 어떤식으로 모니터 출력할지 위에서 부터 한줄씩 할지 등등
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 비례모드

	sd.SampleDesc.Count = 1; // 멀티샘플링할때 추출할 픽셀 갯수
	sd.SampleDesc.Quality = 0; // 수준
	// 런타임중 샘플링 변경하려면 삭제하고 다시 생성하는 방법이 있다.

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼를 렌더 대상으로한다.(현재 후면버퍼를 렌더링)
	sd.BufferCount = 1; // 후면버퍼의 갯수
	sd.OutputWindow = m_hWnd;// 렌더링을 할 창의 핸들값
	sd.Windowed = true; // 창모드 true 전체화면모드 false
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//교환 효과 (디스플레이 구동기가 가장 효율적인 방법으로 선택)
	sd.Flags = 0; // 디스플레이모드 플래그 책은 잘돌아가서 0으로 선택함 DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH를 지정하면 전체화면 모드에서 디스플레이모드 자동선택 이거 실험!!


	// 스왑체인을 위한 질의 과정 스왑체인을 사용하려면 장치에 쓰인 Factory인스턴스를 이용해야한다. 그냥 생성하면 오류가 발생한다.
	IDXGIDevice* dxgiDevice = 0;
	DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	// __uuidof = 클래스명이나 인터페이스명에대한 GUID값을 리턴해준다.

	assert(dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	assert(dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory); // Factory를 얻음

	assert(dxgiFactory);

	dxgiFactory->CreateSwapChain(DEVICE, &sd, &m_pSwap);

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();
}

void CRenderMgr::CreateMultiRenderTarget()
{
	CTexture* pTex = nullptr;

	// 파이프라인에 연결할 후면버퍼 뷰를 생성
	ID3D11Texture2D* back;
	m_pSwap->GetBuffer(0, // 후면버퍼의 색인 몇번째 녀석인지 하나일땐 0
		__uuidof(ID3D11Texture2D), (void**)&back);

	pTex = CTexture::CreateTexture(back);//CResMgr::GetInst()->CreateTexture(L"SwapTarget", back);
	m_pRT[(UINT)RT_TYPE::SWAP] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 1.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pRT[(UINT)RT_TYPE::OUTPUT] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 0.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);//CResMgr::GetInst()->CreateTexture(L"ColorTarget", m_iWidth, m_iHeight,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pRT[(UINT)RT_TYPE::COLOR] = new CRT(pTex,Vec4(1.f,1.f,1.f,1.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);//CResMgr::GetInst()->CreateTexture(L"NormalTarget", m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pRT[(UINT)RT_TYPE::NORMAL] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 0.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);//CResMgr::GetInst()->CreateTexture(L"ObjSpecularTarget", m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pRT[(UINT)RT_TYPE::OBJ_SPECULAR] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 0.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);//CResMgr::GetInst()->CreateTexture(L"PositionTarget", m_iWidth, m_iHeight,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pRT[(UINT)RT_TYPE::POSITION] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 1.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);//CResMgr::GetInst()->CreateTexture(L"DiffuseTarget", m_iWidth, m_iHeight,D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pRT[(UINT)RT_TYPE::DIFFUSE] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 1.f));

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);//CResMgr::GetInst()->CreateTexture(L"SpecularTarget", m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	m_pRT[(UINT)RT_TYPE::SPECULAR] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 0.f));

	//for (UINT i = (UINT)RT_TYPE::SHADOW_0; i <= (UINT)RT_TYPE::SHADOW_5; i++)
	//{
	//	pTex = CTexture::CreateTexture(4096, 4096, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);//CResMgr::GetInst()->CreateTexture(strTarget, 4096, 4096, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);
	//	m_pRT[i] = new CRT(pTex, Vec4(1.f, 0.f, 0.f, 1.f));
	//}
	
	/*pTex = CResMgr::GetInst()->CreateTexture(L"MergeTarget", m_iWidth, m_iHeight, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pRT[(UINT)RT_TYPE::MERGE] = new CRT(pTex, Vec4(0.f, 0.f, 0.f, 1.f));*/

	pTex = CTexture::CreateTexture(m_iWidth, m_iHeight, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);//CResMgr::GetInst()->CreateTexture(L"DepthStencilTarget", m_iWidth, m_iHeight, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);

	// Clear 값은 왠만하면 0으로 해줘야한다. Deffered 단계에서 가져다가 쓸떄 그 값이 있는거 존재하는것으로 착각을해서 이상한 값이 나온다.

	m_pMRT[(UINT)MRT_TYPE::SWAP] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::SWAP]->Create(&(m_pRT[(UINT)RT_TYPE::SWAP]), nullptr, 1);

	m_pMRT[(UINT)MRT_TYPE::OUTPUT] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::OUTPUT]->Create(&(m_pRT[(UINT)RT_TYPE::OUTPUT]), pTex, 1);
	m_pMRT[(UINT)MRT_TYPE::OUTPUT]->OMSet();

	m_pMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::DEFERRED]->Create(&(m_pRT[(UINT)RT_TYPE::COLOR]),nullptr,4);
	m_pMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();

	m_pMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::LIGHT]->Create(&(m_pRT[(UINT)RT_TYPE::DIFFUSE]), nullptr, 2);
	m_pMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	//CTexture* pDepthTex = CTexture::CreateTexture(4096, 4096, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);// CResMgr::GetInst()->CreateTexture(L"ShadowDepthStencil", 4096, 4096, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);

	/*m_pMRT[(UINT)MRT_TYPE::SHADOW] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::SHADOW]->Create(&(m_pRT[(UINT)RT_TYPE::SHADOW_0]), pDepthTex, 6);*/

	/*m_pMRT[(UINT)MRT_TYPE::SHADOW] = new CMRT;
	m_pMRT[(UINT)MRT_TYPE::SHADOW]->Create(&(m_pRT[(UINT)RT_TYPE::SHADOW]), pDepthTex, 1);*/
}

void CRenderMgr::CreateBlend()
{
	D3D11_BLEND_DESC  desc;
	wstring wcsName = L"";

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	wcsName = L"Alpha";

	m_arrBlend[(UINT)BLEND_TYPE::ALPHA] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::ALPHA]->SetName(wcsName);
	
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = false;

	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wcsName = L"NONE";

	m_arrBlend[(UINT)BLEND_TYPE::NONE] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::NONE]->SetName(wcsName);
	
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;

	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wcsName = L"Lighting";

	m_arrBlend[(UINT)BLEND_TYPE::LIGHTING] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::LIGHTING]->SetName(wcsName);

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;

	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wcsName = L"ADD";
	m_arrBlend[(UINT)BLEND_TYPE::ADD] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::ADD]->SetName(wcsName);
	
	desc = {};

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wcsName = L"ONEONE";
	m_arrBlend[(UINT)BLEND_TYPE::ONEONE] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::ONEONE]->SetName(wcsName);

	desc = {};

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wcsName = L"Sub";
	m_arrBlend[(UINT)BLEND_TYPE::SUB] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)BLEND_TYPE::SUB]->SetName(wcsName);
}

void CRenderMgr::CreateDepthStencil()
{
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL]->SetName(L"LessEqualNoRender");

	desc = {};

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS]->SetName(L"Less");

	desc = {};

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::GREATER_EQUAL] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::GREATER_EQUAL]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::GREATER_EQUAL]->SetName(L"GREATER_EQUAL");

	desc = {};

	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = true;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace = desc.BackFace;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_FRONT] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_FRONT]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_FRONT]->SetName(L"FindFront");

	desc = {};

	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = true;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace = desc.FrontFace;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_BACK] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_BACK]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_BACK]->SetName(L"FindBack");

	desc = {};

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = true;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace = desc.FrontFace;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_EQUAL] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_EQUAL]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::FIND_EQUAL]->SetName(L"FindEqual");

	desc = {};
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;

	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO] = new CDepthStencilState;
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]->Create(desc);
	m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]->SetName(L"NoDepthStencil");
}

void CRenderMgr::CreateFilter()
{
	m_pMergeFilter = new CGameObj;
	m_pMergeFilter->AddComponent(new CFilterCom);
	m_pMergeFilter->AddComponent(new CDeferredScript);

	m_pMergeFilter->GetTransForm()->SetScale(Vec3(2.f,2.f,1.f));
	
	CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Merge");
	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL]);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::COLOR]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::NORMAL]->GetTexture(), SHADER_PARAM::TEX_1);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_2);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::DIFFUSE]->GetTexture(), SHADER_PARAM::TEX_3);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SPECULAR]->GetTexture(), SHADER_PARAM::TEX_4);
	//pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_0]->GetTexture(), SHADER_PARAM::TEX_5);
	//pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]);

	m_pMergeFilter->GetFilter()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pMergeFilter->GetFilter()->SetMaterial(pMtrl);

	m_mapFilter.insert(make_pair(L"Merge", m_pMergeFilter));

	CGameObj* pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);
	pFilter->GetTransForm()->SetScale(Vec3(2.f, 2.f, 1.f));

	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"DirLight");
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::NORMAL]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_1);
	//pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_0]->GetTexture(), SHADER_PARAM::TEX_2);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::OBJ_SPECULAR]->GetTexture(), SHADER_PARAM::TEX_3);
	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]);
	pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::ADD]);
	//pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::ONEONE]);
	pFilter->GetFilter()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pFilter->GetFilter()->SetMaterial(pMtrl);

	m_mapFilter.insert(make_pair(L"DirLight", pFilter));

	pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);

	//pFilter->GetTransForm()->SetScale(Vec3(2.f, 2.f, 1.f));

	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"PointLight")->Clone();
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::NORMAL]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_1);
	/*pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_0]->GetTexture(), SHADER_PARAM::TEX_2);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_1]->GetTexture(), SHADER_PARAM::TEX_3);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_2]->GetTexture(), SHADER_PARAM::TEX_4);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_3]->GetTexture(), SHADER_PARAM::TEX_5);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_4]->GetTexture(), SHADER_PARAM::TEX_6);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SHADOW_5]->GetTexture(), SHADER_PARAM::TEX_7);*/
	pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::ADD]);

	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]);
	//pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::ONEONE]);
	
	pFilter->GetFilter()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pFilter->GetFilter()->SetVolumMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pFilter->GetFilter()->SetMaterial(pMtrl);

	m_mapFilter.insert(make_pair(L"PointLight", pFilter));

	pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);

	pFilter->GetTransForm()->SetScale(Vec3(2.f, 2.f, 1.f));

	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"PointLight");
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::NORMAL]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_1);
	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::NO_NO]);
	pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::ONEONE]);
	
	pFilter->GetFilter()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pFilter->GetFilter()->SetVolumMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pFilter->GetFilter()->SetMaterial(pMtrl);

	m_mapFilter.insert(make_pair(L"SpotLight", pFilter));

	pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);

	pFilter->GetTransForm()->SetScale(Vec3(2.f, 2.f, 1.f));

	pFilter->GetFilter()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::LESS_EQUAL]);
	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Vision");
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::SWAP]->GetTexture(), SHADER_PARAM::TEX_0);
	pFilter->GetFilter()->SetMaterial(pMtrl);

	m_mapFilter.insert(make_pair(L"Vision", pFilter));
	m_pVisionFilter = pFilter;

	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Texture\\T_Scratch01.tga";
	CTexture* pTex = CTexture::CreateTexture(strPath);
	CResMgr::GetInst()->AddResource<CTexture>(L"Decal", pTex);

	pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);
	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Decal")->Clone();
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(pTex, SHADER_PARAM::TEX_1);
	pMtrl->SetDepthStencil(m_arrDepthStencil[(UINT)DEPTH_STENCIL_TYPE::GREATER_EQUAL]);
	pMtrl->SetBlend(m_arrBlend[(UINT)BLEND_TYPE::SUB]);
	pFilter->GetFilter()->SetVolumMesh(CResMgr::GetInst()->Load<CMesh>(L"CubeMesh"));
	pFilter->GetFilter()->SetMaterial(pMtrl);
	m_mapFilter.insert(make_pair(L"Decal", pFilter));

	pFilter = new CGameObj;
	pFilter->AddComponent(new CFilterCom);
	pFilter->GetFilter()->SetFilterType(FILTER_TYPE::BLUR);
	pFilter->GetFilter()->SetOutTexture(m_pRT[(UINT)RT_TYPE::OUTPUT]->GetTexture());
	pFilter->GetFilter()->Start();
	pMtrl->SetRasterizeState(RASTERIZE_TYPE::CULL_NONE);
	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Blur")->Clone();
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::OUTPUT]->GetTexture(), SHADER_PARAM::TEX_0);
	pMtrl->SetParam(m_pRT[(UINT)RT_TYPE::POSITION]->GetTexture(), SHADER_PARAM::TEX_2);
	pFilter->GetFilter()->SetVolumMesh(CResMgr::GetInst()->Load<CMesh>(L"SphereMesh"));
	pFilter->GetFilter()->SetMaterial(pMtrl);
	m_mapFilter.insert(make_pair(L"Blur", pFilter));
}

void CRenderMgr::CreateViewPort()
{
}

void CRenderMgr::CreateRasterize()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&desc, &m_arrRasterize[(UINT)RASTERIZE_TYPE::CULL_NONE]);
	m_vecRSName.push_back(L"CULL_NONE");

	m_arrRasterize[(UINT)RASTERIZE_TYPE::CULL_BACK] = nullptr;
	m_vecRSName.push_back(L"CULL_BACK");

	desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&desc, &m_arrRasterize[(UINT)RASTERIZE_TYPE::CULL_FRONT]);
	m_vecRSName.push_back(L"CULL_FRONT");

	desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&desc, &m_arrRasterize[(UINT)RASTERIZE_TYPE::WIREFRAME]);
	m_vecRSName.push_back(L"WIREFRAME");

}

void CRenderMgr::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	DEVICE->CreateSamplerState(&desc, &m_arrSampler[(UINT)SAMPLER_TYPE::CLAMP]);
	m_vecSPName.push_back(L"CLAMP");

	desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	DEVICE->CreateSamplerState(&desc, &m_arrSampler[(UINT)SAMPLER_TYPE::WRAP]);
	m_vecSPName.push_back(L"WRAP");
}

CBlendState * CRenderMgr::GetBlend(wstring & _wcsName)
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		if (m_arrBlend[i]->GetName() == _wcsName)
			return m_arrBlend[i];
	}

	return nullptr;
}

CDepthStencilState * CRenderMgr::GetDepthStencil(const wstring & _wcsName)
{
	for (UINT i = 0; i < (UINT)DEPTH_STENCIL_TYPE::END; ++i)
	{
		if (m_arrDepthStencil[i]->GetName() == _wcsName)
			return m_arrDepthStencil[i];
	}

	return nullptr;
}

wstring CRenderMgr::GetBlendName(CBlendState * _pState)
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		if (m_arrBlend[i] == _pState)
			return m_arrBlend[i]->GetName();
	}

	return L"NONE";
}

wstring CRenderMgr::GetDepthStencilName(CDepthStencilState * _pState)
{
	for (UINT i = 0; i < (UINT)DEPTH_STENCIL_TYPE::END; ++i)
	{
		if (m_arrDepthStencil[i] == _pState)
			return m_arrDepthStencil[i]->GetName();
	}
	return L"NONE";
}

void CRenderMgr::RSSetState(RASTERIZE_TYPE _eType)
{
	CONTEXT->RSSetState(m_arrRasterize[(UINT)_eType]);
}

void CRenderMgr::SPSetState(SAMPLER_TYPE _eType)
{
	CONTEXT->DSSetSamplers(0, 1, &m_arrSampler[(UINT)_eType]);
	CONTEXT->CSSetSamplers(0, 1, &m_arrSampler[(UINT)_eType]);
	CONTEXT->PSSetSamplers(0, 1, &m_arrSampler[(UINT)_eType]);
}

void CRenderMgr::Clear()
{
	for (UINT i = 0; i < (UINT)MRT_TYPE::END; i++)
	{
		m_pMRT[i]->Clear();
	}
}

CGameObj * CRenderMgr::FindFilter(const wstring & _wcsKey)
{
	map<wstring,CGameObj*>::iterator iter = m_mapFilter.find(_wcsKey);
	if (iter == m_mapFilter.end())
		assert(NULL);

	return iter->second;
}

void CRenderMgr::SetMergeFilterParam(void* _pData,SHADER_PARAM _eParam)
{
	m_pMergeFilter->GetFilter()->GetMaterial()->SetParam(_pData, _eParam);
}

RASTERIZE_TYPE CRenderMgr::GetRSState(wstring & _strName)
{
	for (size_t i = 0; i < m_vecRSName.size(); i++)
	{
		if (m_vecRSName[i] == _strName)
			return (RASTERIZE_TYPE)i;
	}

	assert(NULL);

	return RASTERIZE_TYPE();
}

SAMPLER_TYPE CRenderMgr::GetSPState(wstring & _strName)
{
	for (size_t i = 0; i < m_vecSPName.size(); i++)
	{
		if (m_vecSPName[i] == _strName)
			return (SAMPLER_TYPE)i;
	}

	assert(NULL);

	return SAMPLER_TYPE();
}

CTexture * CRenderMgr::GetRenderTargetTexture(RT_TYPE _eType)
{
	return m_pRT[(UINT)_eType]->GetTexture();
}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	m_vecCam.push_back(_pCam);

	if (_pCam->GetMainCam())
		m_pMainCam = _pCam;
}

HRESULT CRenderMgr::D2DInit(ID2D1Factory **factory, ID2D1RenderTarget **Rt)
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory);
	if (FAILED(hr))
		return hr;
	IDXGISurface* pBackBuffer = nullptr;
	m_pSwap->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	FLOAT dpiX;
	FLOAT dpiY;
	//(*factory)->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	hr = (*factory)->CreateDxgiSurfaceRenderTarget(
		pBackBuffer,&props,	Rt);
	pBackBuffer->Release();
	return hr;
}