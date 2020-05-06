#pragma once
#include "CParent.h"
class CShader :
	public CParent
{
private:
	ID3DBlob* m_pVSBlob;
	ID3DBlob* m_pVSInstBlob;
	ID3DBlob* m_pHSBlob;
	ID3DBlob* m_pDSBlob;
	ID3DBlob* m_pPSBlob;
	ID3DBlob* m_pGSBlob;
	ID3DBlob* m_pCSBlob;
	ID3DBlob* m_pErrBlob;

	ID3D11VertexShader* m_pVS;
	ID3D11VertexShader* m_pVSInst;
	ID3D11HullShader* m_pHS;
	ID3D11DomainShader* m_pDS;
	ID3D11PixelShader* m_pPS;
	ID3D11GeometryShader* m_pGS;
	ID3D11ComputeShader * m_pCS;

	
public:
	ID3DBlob* GetVSBlob() { return m_pVSBlob; }
	ID3DBlob* GetVSInstBlob() { return m_pVSInstBlob; }

	void CreateVertexShader(const string& _strFunc, const string& _strVersion,const wstring& _wcsFileName);
	void CreateVertexInstanceShader(const string& _strFunc, const string& _strVersion, const wstring& _wcsFileName);
	void CreateHullShader(const string& _strFunc, const string& _strVersion, const wstring& _wcsFileName);
	void CreateDomainShader(const string& _strFunc, const string& _strVersion, const wstring& _wcsFileName);
	void CreatePixelShader(const string& _strFunc, const string& _strVersion, const wstring& _wcsFileName);
	void CreateGeoMetryShader(const string& _strFunc, const string& _strVersion, const wstring & _wcsFileName);
	void CreateStreamGeoMetryShader(const string& _strFunc, const string& _strVersion, const wstring & _wcsFileName);
	void CreateComputeShader(const string& _strFunc, const string& _strVersion, const wstring & _wcsFileName);

	void UpdateData();
	void UpdateDataInstance();

public:
	CShader();
	~CShader();
};