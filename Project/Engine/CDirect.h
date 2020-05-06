#pragma once
#include "global.h"

class CConstBuffer;
class CDirect
{
	SINGLETON(CDirect)

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	map<wstring, CConstBuffer*> m_mapConstBuffer;

public:

	void CreateConstBuffer(const wstring& _wcsKey, UINT _iSize, UINT _iRegister);
	void CreateBlurTexture();
	CConstBuffer* FindConstBuffer(const wstring& _wcsKey);
	void Init(const tResolution& _res,HWND _hwnd);
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }
};