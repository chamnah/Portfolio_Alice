#include "CTexture.h"
#include "CDirect.h"
#include "CResMgr.h"

CTexture * CTexture::CreateTexture(const wstring& _wcsPath, UINT _iFlag, UINT _iFormat)
{
	wchar_t szExt[20] = L"";
	_wsplitpath_s(_wcsPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	CTexture* pTex = new CTexture;

	ScratchImage TempImage;
	if (_iFormat == 0) // Format을 지정하지 않으면 원래 자신의 Format을 이용한다.
	{
		if (wcscmp(szExt, L".tga") == 0)
			LoadFromTGAFile(_wcsPath.c_str(), NULL, pTex->m_image);
		else
			LoadFromWICFile(_wcsPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, NULL, pTex->m_image);
	}
	else // Format을 지정한 것으로 정한다.
	{
		if (wcscmp(szExt, L".tga") == 0)
			LoadFromTGAFile(_wcsPath.c_str(), NULL, TempImage);
		else
			LoadFromWICFile(_wcsPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, NULL, TempImage);

		Convert(TempImage.GetImages(), TempImage.GetImageCount(), TempImage.GetMetadata(),(DXGI_FORMAT)_iFormat, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT, pTex->m_image);
	}

	ID3D11Resource* pRes = nullptr;
	CreateTextureEx(DEVICE, pTex->m_image.GetImages(), pTex->m_image.GetImageCount(), pTex->m_image.GetMetadata(), D3D11_USAGE_DEFAULT, _iFlag,0,0,false, &pRes);
	if (_iFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		ID3D11ShaderResourceView* pView = nullptr;
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Format = pTex->m_image.GetMetadata().format;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2D.MostDetailedMip = 0;

		DEVICE->CreateShaderResourceView(pRes, &desc, &(pTex->m_pResView));
		assert(pTex->m_pResView);
		pTex->m_pResView->GetResource((ID3D11Resource**)&pTex->m_pTex2D);
	}

	if (_iFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		desc.Format = pTex->m_image.GetMetadata().format;

		DEVICE->CreateUnorderedAccessView(pRes, &desc, &(pTex->m_pUAView));
		assert(pTex->m_pUAView);
	}

	pRes->Release();
	pTex->SetPath(_wcsPath);
	pTex->m_pTex2D->GetDesc(&pTex->m_Desc);

	return pTex;
}

CTexture * CTexture::CreateTexture(ID3D11ShaderResourceView* _pRV)
{
	CTexture* pTex = new CTexture;
	pTex->m_pResView = _pRV;

	return pTex;
}

CTexture * CTexture::CreateTexture(CTexture * _pTex)
{
	CTexture* pTex = CreateTexture(_pTex->m_Desc.Width, _pTex->m_Desc.Height, _pTex->m_Desc.BindFlags, _pTex->m_Desc.Format, _pTex->m_Desc.Usage);
	return pTex;
}

CTexture * CTexture::CreateTexture(UINT _iWidth, UINT _iHeight, UINT _iFlag,DXGI_FORMAT _eFormat,D3D11_USAGE _eUsage)
{
	CTexture* pTex = new CTexture;

	pTex->m_Desc.Width = _iWidth;
	pTex->m_Desc.Height = _iHeight;
	pTex->m_Desc.MipLevels = 1; // 밉맵수준의 갯수 깊이버퍼 텍스쳐는 하나면 된다.
	pTex->m_Desc.ArraySize = 1; // 텍스쳐 배열의 텍스쳐갯수 깊이 버퍼는 하나면된다.
	pTex->m_Desc.Format = _eFormat; // 4.1.5
	
	// 지연 렌더링에서는 멀티샘플링을 단순히 지정해서 사용할수 없다.
	pTex->m_Desc.SampleDesc.Count = 1; // 멀티샘플링할때 추출할 픽셀 갯수
	pTex->m_Desc.SampleDesc.Quality = 0; // 수준

	pTex->m_Desc.BindFlags = _iFlag; // 자원을 파이프라인에 어떤식으로 묶을것인지 지정한다. 플래그로써 하나 이상 조합 지정 가능
	
	pTex->m_Desc.Usage = _eUsage; // 텍스쳐의 용도결정 (GPU가 읽고 쓰기를 한다. CPU 접근 불가)
	if(pTex->m_Desc.Usage == D3D11_USAGE_DYNAMIC)
		pTex->m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;// CPU가 자원에 접근하는 방식을 결정한다. 접근 못하게 하려면 0이다.
	
	pTex->m_Desc.MiscFlags = 0; // 기타 플래그들이다.

	DEVICE->CreateTexture2D(&pTex->m_Desc,0, &(pTex->m_pTex2D)); // 깊이 스텐실 텍스쳐를 생성하고 그 버퍼를 반환받는다.
	assert(pTex->m_pTex2D);

	if (_iFlag & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(pTex->m_pTex2D, 0, &(pTex->m_pDSView)); // 그 버퍼를 토대로 파이프라인과 연결할 뷰를 생성
		assert(pTex->m_pDSView);
	}
	else
	{
		if (_iFlag & D3D11_BIND_RENDER_TARGET)
		{
			DEVICE->CreateRenderTargetView(pTex->m_pTex2D, 0, &(pTex->m_pRTView));
			assert(pTex->m_pRTView);
		}

		if (_iFlag & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Format = _eFormat;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			DEVICE->CreateShaderResourceView(pTex->m_pTex2D, &desc , &(pTex->m_pResView));
			assert(pTex->m_pResView);
		}

		if (_iFlag & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Format = _eFormat;

			DEVICE->CreateUnorderedAccessView(pTex->m_pTex2D, &desc, &(pTex->m_pUAView));
			assert(pTex->m_pUAView);

		}
	}

	return pTex;
}

CTexture * CTexture::CreateTexture(ID3D11Texture2D * _pTex2D)
{
	CTexture* pTex = new CTexture;

	pTex->m_pTex2D = _pTex2D;

	D3D11_TEXTURE2D_DESC Texdesc;
	_pTex2D->GetDesc(&Texdesc);

	if (Texdesc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(pTex->m_pTex2D, 0, &(pTex->m_pDSView)); // 그 버퍼를 토대로 파이프라인과 연결할 뷰를 생성
		assert(pTex->m_pDSView);
	}
	else
	{
		if (Texdesc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			DEVICE->CreateRenderTargetView(pTex->m_pTex2D, 0, &(pTex->m_pRTView));
			assert(pTex->m_pRTView);
		}

		if (Texdesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Format = Texdesc.Format;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			DEVICE->CreateShaderResourceView(pTex->m_pTex2D, &desc, &(pTex->m_pResView));
			assert(pTex->m_pResView);
		}

		if (Texdesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Format = Texdesc.Format;

			DEVICE->CreateUnorderedAccessView(pTex->m_pTex2D,&desc,&(pTex->m_pUAView));
			assert(pTex->m_pUAView);
		}
	}
	
	pTex->m_pTex2D->GetDesc(&pTex->m_Desc);

	return pTex;
}

CTexture* CTexture::CreateTexture(vector<CTexture*>& _vecTex, UINT _iFlag)
{
	vector<D3D11_SUBRESOURCE_DATA> vecSubData;

	D3D11_TEXTURE2D_DESC oriDesc;
	_vecTex[0]->m_pTex2D->GetDesc(&oriDesc);

	D3D11_TEXTURE2D_DESC desc;
	desc = oriDesc;
	desc.ArraySize = _vecTex.size();
	desc.BindFlags = _iFlag;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SUBRESOURCE_DATA data;
	for (size_t i = 0; i < _vecTex.size(); i++)
	{
		data.pSysMem = _vecTex[i]->GetPixel();
		data.SysMemPitch = _vecTex[i]->m_image.GetImages()->rowPitch;
		data.SysMemSlicePitch = _vecTex[i]->m_image.GetImages()->slicePitch;

		vecSubData.push_back(data);
	}

	CTexture* pTex = new CTexture;
	DEVICE->CreateTexture2D(&desc,&vecSubData[0], &pTex->m_pTex2D);


	D3D11_TEXTURE2D_DESC NewDesc;
	pTex->m_pTex2D->GetDesc(&NewDesc);

	if (_iFlag & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(pTex->m_pTex2D, 0, &(pTex->m_pDSView)); // 그 버퍼를 토대로 파이프라인과 연결할 뷰를 생성
		assert(pTex->m_pDSView);
	}
	else
	{
		if (_iFlag & D3D11_BIND_RENDER_TARGET)
		{
			DEVICE->CreateRenderTargetView(pTex->m_pTex2D, 0, &(pTex->m_pRTView));
			assert(pTex->m_pRTView);
		}

		if (_iFlag & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			desc.Format = NewDesc.Format;
			desc.Texture2DArray.ArraySize = NewDesc.ArraySize;
			desc.Texture2DArray.FirstArraySlice = 0;
			desc.Texture2DArray.MipLevels = -1;
			desc.Texture2DArray.MostDetailedMip = 0;

			DEVICE->CreateShaderResourceView(pTex->m_pTex2D, &desc, &(pTex->m_pResView));
			assert(pTex->m_pResView);
			//CONTEXT->GenerateMips(pTex->m_pResView);
		}
	}

	return pTex;
}

void CTexture::SetRegisterAll(int _iIndex)
{
	CONTEXT->VSSetShaderResources(_iIndex, 1, &m_pResView); // 1번째 slot이 레지스터 번호이다. 텍스쳐 레지스터 t0 ~ ...
	CONTEXT->HSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->DSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->GSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->CSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->PSSetShaderResources(_iIndex, 1, &m_pResView);
}

void CTexture::SetRWTexRegister(int _iIndex)
{
	CONTEXT->CSSetUnorderedAccessViews(_iIndex,1,&m_pUAView, nullptr);
}

void CTexture::RegisterClear(int _iIndex)
{
	ID3D11ShaderResourceView* pView = nullptr;
	CONTEXT->VSSetShaderResources(_iIndex, 1, &pView); // 1번째 slot이 레지스터 번호이다. 텍스쳐 레지스터 t0 ~ ...
	CONTEXT->HSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->DSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->GSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->CSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->PSSetShaderResources(_iIndex, 1, &pView);
}

void CTexture::RWTexRegisterClear(int _iIndex)
{
	ID3D11UnorderedAccessView* pView = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(_iIndex, 1, &pView,nullptr);
}

void CTexture::Resize(int _iWidth, int _iHeight)
{
	SAFE_RELEASE(m_pResView);
	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pUAView);
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pTex2D);

	ID3D11Texture2D* pTex = nullptr;
	m_Desc.Width = _iWidth;
	m_Desc.Height = _iHeight;
	DEVICE->CreateTexture2D(&m_Desc,NULL,&m_pTex2D);

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_pTex2D, 0, &(m_pDSView)); // 그 버퍼를 토대로 파이프라인과 연결할 뷰를 생성
		assert(m_pDSView);
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			DEVICE->CreateRenderTargetView(m_pTex2D, 0, &(m_pRTView));
			assert(m_pRTView);
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Format = m_Desc.Format;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			DEVICE->CreateShaderResourceView(m_pTex2D, &desc, &(m_pResView));
			assert(m_pResView);
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Format = m_Desc.Format;

			DEVICE->CreateUnorderedAccessView(m_pTex2D, &desc, &(m_pUAView));
			assert(m_pUAView);
		}
	}
}

void * CTexture::GetPixel()
{
	if (CaptureTexture(DEVICE, CONTEXT, m_pTex2D, m_image))
		assert(NULL);
	return m_image.GetPixels();
}

void CTexture::Save(FILE* _pFile)
{
	// 키값
	wstring strName = GetName();

	SaveWString(GetName(), _pFile);

	// 상대경로
	SaveWString(m_wcsPath, _pFile);
}

void CTexture::Load(FILE * _pFile)
{
	wstring strKey, strPath;
	strKey = LoadWString(_pFile);

	// 상대경로
	strPath = LoadWString(_pFile);

	// 해당 키로 등록된게 없으면 로딩
	if (nullptr == CResMgr::GetInst()->Load<CTexture>(strKey))
		CResMgr::GetInst()->AddResource<CTexture>(strKey, CreateTexture(strPath));
}

void CTexture::Clear(Vec4 vColor)
{
	static CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"TexClear");

	pMtrl->SetParam(&m_Desc.Width,SHADER_PARAM::INT_0);
	pMtrl->SetParam(&m_Desc.Height, SHADER_PARAM::INT_1);
	pMtrl->SetParam(&vColor, SHADER_PARAM::VEC4_0);
	pMtrl->SetParam(this, SHADER_PARAM::RWTEX_0);

	int iGroupX = (m_Desc.Width / 32) + 1;
	int iGroupY = (m_Desc.Height / 32) + 1;

	pMtrl->Excute(iGroupX,iGroupY,1);
}

void CTexture::SetData(void * _pData, UINT _iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pTex2D,0,D3D11_MAP_WRITE_DISCARD,0, &tSub);
	memcpy(tSub.pData, _pData, _iSize);
	CONTEXT->Unmap(m_pTex2D,0);
}

CTexture::CTexture()
	:m_Desc{}
	, m_pResView(nullptr)
	, m_pRTView(nullptr)
	, m_pUAView(nullptr)
	, m_pDSView(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pResView);
	SAFE_RELEASE(m_pRTView);
	SAFE_RELEASE(m_pUAView);
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pTex2D);
	m_image.Release();
}