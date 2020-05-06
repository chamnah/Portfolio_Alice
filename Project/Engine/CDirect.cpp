#include "CDirect.h"
#include "CConstBuffer.h"
#include "CStateMgr.h"
CDirect::CDirect()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
{
}

CDirect::~CDirect()
{
	SafeMapDelete(m_mapConstBuffer);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);

//#if defined(DEBUG) | defined(_DEBUG)
//	ID3D11Debug* pDebug = nullptr;
//	m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
//
//	SAFE_RELEASE(m_pDevice);
//	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//	SAFE_RELEASE(pDebug);
//#endif
}

void CDirect::CreateConstBuffer(const wstring& _wcsKey,UINT _iSize,UINT _iRegister)
{
	CConstBuffer* pBuffer = CConstBuffer::Create(_iSize, _iRegister);
	
	m_mapConstBuffer.insert(make_pair(_wcsKey,pBuffer));
}

void CDirect::CreateBlurTexture()
{
	DXGI_SAMPLE_DESC sd;
	sd.Quality = 0;
	sd.Count = 1;

	D3D11_TEXTURE2D_DESC desc;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.SampleDesc = sd;
}

CConstBuffer * CDirect::FindConstBuffer(const wstring & _wcsKey)
{
	map<wstring,CConstBuffer*>::iterator iter = m_mapConstBuffer.find(_wcsKey);

	if (iter == m_mapConstBuffer.end())
		assert(false);

	return iter->second;
}

void CDirect::Init(const tResolution& _res, HWND _hwnd)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;// ( 디버그 모드에서 VC++ 출력창 디버그 메시지 출력 )
#endif
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL eLevel;
	HRESULT hr = D3D11CreateDevice(
		0, // 디스플레이 어댑터 설정 ( 0 == 기본어댑터)
		D3D_DRIVER_TYPE_HARDWARE, // 출력장치선택 (그래픽을 하드웨어로 출력)
		0, // 소프트웨어 구동기 지정 없으면 0
		iFlag, // 추가적인 플래그
		0, // 지원되는 기능 수준점검 D3D_FEATURE_LEVEL형식 원소 배열을 넣으면 그중에서 점검( 0 == 지원되는 최고수준기능 선택) 
		0, // 지원 배열원소 갯수 배열에 0 넣었으면 여기도 0
		D3D11_SDK_VERSION, //D3D11을 사용하기 위함
		&m_pDevice,
		&eLevel,
		&m_pContext);

	if (FAILED(hr))
		assert(NULL);

	// 각 변수에 어떤 값이 들어갔는지 확인해보자

	UINT Quality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Quality);
	assert(Quality > 0);

	/*D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	   
	ID3D11SamplerState* pSS;
	m_pDevice->CreateSamplerState(&desc, &pSS);

	m_pContext->DSSetSamplers(0, 1, &pSS);
	m_pContext->CSSetSamplers(0, 1, &pSS);
	m_pContext->PSSetSamplers(0,1,&pSS);
	pSS->Release();*/
}