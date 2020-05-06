#include "CShader.h"
#include "CDirect.h"
#include "CPathMgr.h"

CShader::CShader()
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVSInstBlob);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pErrBlob);

	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSInst);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pCS);
}

void CShader::CreateVertexShader(const string& _strFunc, const string& _strVersion,const wstring& _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		assert(false && "CreateVertexShader Error");
}

void CShader::CreateVertexInstanceShader(const string & _strFunc, const string & _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pVSInstBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSInstBlob->GetBufferPointer(), m_pVSInstBlob->GetBufferSize(), NULL, &m_pVSInst)))
		assert(false && "CreateVertexInstancingShader Error");
}

void CShader::CreateHullShader(const string & _strFunc, const string & _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pHSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(), m_pHSBlob->GetBufferSize(), NULL, &m_pHS)))
		assert(false && "CreateHullShader Error");
}

void CShader::CreateDomainShader(const string & _strFunc, const string & _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pDSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(), m_pDSBlob->GetBufferSize(), NULL, &m_pDS)))
		assert(false && "CreateDomainShader Error");
}

void CShader::CreatePixelShader(const string& _strFunc, const string& _strVersion,const wstring& _wcsFileName)
{

	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pPSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
		assert(false && "CreatePixelShader Error");
}

void CShader::CreateGeoMetryShader(const string & _strFunc, const string & _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pGSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(), NULL, &m_pGS)))
		assert(false && "CreateGeometryShader Error");
}

void CShader::CreateStreamGeoMetryShader(const string & _strFunc, const string & _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pGSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	D3D11_SO_DECLARATION_ENTRY entry[8] =
	{
		{0,"POSITION",0,0,3,0}
		,{0,"COLOR",0,0,4,0}
		,{0,"TEXCOORD",0,0,2,0}
		,{0,"NORMAL",0,0,3,0}
		,{0,"TANGENT",0,0,3,0}
		,{0,"BINORMAL",0,0,3,0}
		,{0,"AGE",0,0,1,0}
		,{0,"TYPE",0,0,1,0}
	};

	UINT istride[] = { sizeof(VTX) };
	DEVICE->CreateGeometryShaderWithStreamOutput(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(), entry, _countof(entry), istride, _countof(istride), D3D11_SO_NO_RASTERIZED_STREAM, NULL, &m_pGS);
	int i = 0;
}

void CShader::CreateComputeShader(const string& _strFunc, const string& _strVersion, const wstring & _wcsFileName)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\";
	strPath += _wcsFileName;

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pCSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateComputeShader(m_pCSBlob->GetBufferPointer(), m_pCSBlob->GetBufferSize(), NULL, &m_pCS)))
		assert(false && "CreateComputeShader Error");
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVS, NULL, NULL);
	CONTEXT->HSSetShader(m_pHS, NULL, NULL);
	CONTEXT->DSSetShader(m_pDS, NULL, NULL);
	CONTEXT->PSSetShader(m_pPS, NULL, NULL);
	CONTEXT->GSSetShader(m_pGS, NULL, NULL);
	CONTEXT->CSSetShader(m_pCS, NULL, NULL);
}

void CShader::UpdateDataInstance()
{
	CONTEXT->VSSetShader(m_pVSInst, NULL, NULL);
	CONTEXT->HSSetShader(m_pHS, NULL, NULL);
	CONTEXT->DSSetShader(m_pDS, NULL, NULL);
	CONTEXT->PSSetShader(m_pPS, NULL, NULL);
	CONTEXT->GSSetShader(m_pGS, NULL, NULL);
	CONTEXT->CSSetShader(m_pCS, NULL, NULL);
}