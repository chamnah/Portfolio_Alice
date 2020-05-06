#pragma once
#include "CParent.h"
class CConstBuffer :
	public CParent
{
private:
	ID3D11Buffer* m_pCB;
	UINT          m_iRegister;

public:
	 static CConstBuffer* Create(UINT _iSize, UINT _iRegister);
	 void UpdateData(void* _pData, UINT _iSize);
	 void ClearData();
	 void UpdateRegister();
	 void UpdateRegisterAll();

public:
	CConstBuffer();
	~CConstBuffer();
};