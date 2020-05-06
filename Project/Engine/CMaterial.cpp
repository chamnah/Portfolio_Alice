#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderMgr.h"
#include "CResMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CBlendState.h"
#include "CDepthStencilState.h"
#include "CRenderMgr.h"

CMaterial::CMaterial()
	:m_pShader(nullptr)
	, m_Param{}
	, m_arrTexture{}
	, m_pBlend(nullptr)
	, m_pDepthStencil(nullptr)
	, m_eRSType(RASTERIZE_TYPE::CULL_BACK)
	, m_eTopol(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_eSPType(SAMPLER_TYPE::WRAP)
{
	m_Param.arrMtrl = Vec4(1.f, 1.f, 1.f, 1.f);
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	m_pShader->UpdateData();

	if (m_pBlend != nullptr)
		m_pBlend->SetShader();
	else
		m_pBlend->DefaultSetShader();

	if (m_pDepthStencil != nullptr)
		m_pDepthStencil->SetRef();
	else
		m_pDepthStencil->DefaultSetRef();

	CRenderMgr::GetInst()->RSSetState(m_eRSType);
	CRenderMgr::GetInst()->SPSetState(m_eSPType);

	UINT iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;

	for (UINT i = 0; i < iCount; i++)
	{
		CTexture::RegisterClear(i);
	}

	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_arrTexture[i] != nullptr) // 널이 아닐때만 들어가므로 이미 들어 있는 녀석에 영향이 생긴다.
		{
			m_Param.arrBool[i] = true;
			m_arrTexture[i]->SetRegisterAll(i);
		}
		else
		{
			m_Param.arrBool[i] = false;
		}
	}

	iCount = (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0;

	for (UINT i = 0; i < iCount; i++)
	{
		CTexture::RWTexRegisterClear(i);
	}

	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_arrRWTexture[i] != nullptr) // 널이 아닐때만 들어가므로 이미 들어 있는 녀석에 영향이 생긴다.
		{
			m_Param.arrRBool[i] = true;
			m_arrRWTexture[i]->SetRWTexRegister(i);
		}
		else
			m_Param.arrRBool[i] = false;
	}

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"ShaderParam");
	pBuffer->UpdateData(&m_Param, sizeof(ShaderParam));
	pBuffer->UpdateRegisterAll();

	CONTEXT->IASetPrimitiveTopology(m_eTopol);
}

void CMaterial::UpdateDataInstancing()
{
	m_pShader->UpdateDataInstance();

	if (m_pBlend != nullptr)
		m_pBlend->SetShader();
	else
		m_pBlend->DefaultSetShader();

	if (m_pDepthStencil != nullptr)
		m_pDepthStencil->SetRef();
	else
		m_pDepthStencil->DefaultSetRef();

	CRenderMgr::GetInst()->RSSetState(m_eRSType);

	UINT iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;

	for (UINT i = 0; i < iCount; i++)
	{
		CTexture::RegisterClear(i);
	}

	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_arrTexture[i] != nullptr) // 널이 아닐때만 들어가므로 이미 들어 있는 녀석에 영향이 생긴다.
		{
			m_Param.arrBool[i] = true;
			m_arrTexture[i]->SetRegisterAll(i);
		}
		else
		{
			m_Param.arrBool[i] = false;
		}
	}

	iCount = (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0;

	for (UINT i = 0; i < iCount; i++)
	{
		CTexture::RWTexRegisterClear(i);
	}

	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_arrRWTexture[i] != nullptr) // 널이 아닐때만 들어가므로 이미 들어 있는 녀석에 영향이 생긴다.
		{
			m_Param.arrRBool[i] = true;
			m_arrRWTexture[i]->SetRWTexRegister(i);
		}
		else
			m_Param.arrRBool[i] = false;
	}

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"ShaderParam");
	pBuffer->UpdateData(&m_Param, sizeof(ShaderParam));
	pBuffer->UpdateRegisterAll();

	CONTEXT->IASetPrimitiveTopology(m_eTopol);
}

void CMaterial::SetParam(void * _pData, SHADER_PARAM _eType)
{
	switch (_eType)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_Param.arrInt[(UINT)_eType - (UINT)SHADER_PARAM::INT_0] = *((int*)_pData);
		break;

	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_Param.arrFloat[(UINT)_eType - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)_pData);
		break;

	case SHADER_PARAM::MTRL:
		m_Param.arrMtrl = *((Vec4*)_pData);
		break;

	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_Param.arrVec2[(UINT)_eType - (UINT)SHADER_PARAM::VEC2_0] = *((Vec2*)_pData);
		break;

	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_Param.arrVec4[(UINT)_eType - (UINT)SHADER_PARAM::VEC4_0] = *((Vec4*)_pData);
		break;

	case SHADER_PARAM::MTX_0:
	case SHADER_PARAM::MTX_1:
	case SHADER_PARAM::MTX_2:
	case SHADER_PARAM::MTX_3:
		m_Param.arrMtx[(UINT)_eType - (UINT)SHADER_PARAM::MTX_0] = *((Mtx*)_pData);
		break;

	case SHADER_PARAM::BOOL_0:
	case SHADER_PARAM::BOOL_1:
	case SHADER_PARAM::BOOL_2:
	case SHADER_PARAM::BOOL_3:
	case SHADER_PARAM::BOOL_4:
	case SHADER_PARAM::BOOL_5:
	case SHADER_PARAM::BOOL_6:
	case SHADER_PARAM::BOOL_7:
	case SHADER_PARAM::BOOL_8:
	case SHADER_PARAM::BOOL_9:
	case SHADER_PARAM::BOOL_10:
	case SHADER_PARAM::BOOL_11:
		m_Param.arrBool[(UINT)_eType - (UINT)SHADER_PARAM::BOOL_0] = *((bool*)_pData);
		break;

	case SHADER_PARAM::RBOOL_0:
	case SHADER_PARAM::RBOOL_1:
	case SHADER_PARAM::RBOOL_2:
	case SHADER_PARAM::RBOOL_3:
		m_Param.arrRBool[(UINT)_eType - (UINT)SHADER_PARAM::RBOOL_0] = *((bool*)_pData);
		break;

	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
	case SHADER_PARAM::TEX_4:
	case SHADER_PARAM::TEX_5:
	case SHADER_PARAM::TEX_6:
	case SHADER_PARAM::TEX_7:
	case SHADER_PARAM::ARRTEX_0:
		m_arrTexture[(UINT)_eType - (UINT)SHADER_PARAM::TEX_0] = (CTexture*)_pData;
		break;

	case SHADER_PARAM::RWTEX_0:
	case SHADER_PARAM::RWTEX_1:
	case SHADER_PARAM::RWTEX_2:
	case SHADER_PARAM::RWTEX_3:
		m_arrRWTexture[(UINT)_eType - (UINT)SHADER_PARAM::RWTEX_0] = (CTexture*)_pData;
		break;
	}
}

CMaterial * CMaterial::Clone()
{
	CMaterial* pClone = new CMaterial(*this);
	CResMgr::GetInst()->AddClone(pClone);
	return pClone;
}

void CMaterial::Excute(UINT _iX, UINT _iY, UINT _iZ)
{
	UpdateData();
	CONTEXT->Dispatch(_iX,_iY,_iZ);
	UAViewClear();
}

void CMaterial::UAViewClear()
{
	UINT iCount = (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0;

	for (UINT i = 0; i < iCount; i++)
		CTexture::RWTexRegisterClear(i);
}

void CMaterial::SaveMeshData(FILE* _pFile)
{
	SaveWString(GetName(), _pFile);
	
	if(m_pShader != nullptr)
	SaveWString(m_pShader->GetName(),_pFile);

	fwrite(&m_Param, sizeof(ShaderParam), 1, _pFile);

	UINT i = 0;
	for (; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; i++)
	{
		if (m_arrTexture[i] != nullptr)
		{
			fwrite(&i, sizeof(UINT), 1, _pFile);
			SaveWString(m_arrTexture[i]->GetName(), _pFile);
			SaveWString(m_arrTexture[i]->GetPath(), _pFile);
		}
	}

	fwrite(&i,sizeof(UINT),1,_pFile);

	i = 0;
	for (; i < (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0; i++)
	{
		if (m_arrRWTexture[i] != nullptr)
		{
			fwrite(&i, sizeof(UINT), 1, _pFile);
			SaveWString(m_arrRWTexture[i]->GetName(), _pFile);
			SaveWString(m_arrRWTexture[i]->GetPath(), _pFile);
		}
	}

	fwrite(&i, sizeof(UINT), 1, _pFile);
}

CMaterial * CMaterial::LoadMeshData(FILE * _pFile)
{
	wstring strKey = LoadWString(_pFile);
	wstring strShader =	LoadWString(_pFile);

	ShaderParam tParam = {};
	fread(&tParam, sizeof(ShaderParam), 1, _pFile);

	CTexture* arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0] = {};
	int iIndex = 0;
	fread(&iIndex, sizeof(UINT), 1, _pFile);
	while (iIndex != (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0)
	{
		wstring strKey = LoadWString(_pFile);
		wstring strPath = LoadWString(_pFile);
		
		arrTex[iIndex] = CResMgr::GetInst()->Load<CTexture>(strKey);
		if (arrTex[iIndex] == nullptr)
		{
			arrTex[iIndex] = CTexture::CreateTexture(strPath);
			CResMgr::GetInst()->AddResource<CTexture>(strKey, arrTex[iIndex]);
		}

		fread(&iIndex, sizeof(UINT), 1, _pFile);
	}

	CTexture* arrRWTex[(UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0] = {};
	iIndex = 0;
	fread(&iIndex, sizeof(UINT), 1, _pFile);
	while (iIndex != (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0)
	{
		wstring strKey = LoadWString(_pFile);
		wstring strPath = LoadWString(_pFile);

		arrRWTex[iIndex] = CResMgr::GetInst()->Load<CTexture>(strKey);
		if (arrRWTex[iIndex] == nullptr)
		{
			arrRWTex[iIndex] = CTexture::CreateTexture(strPath);
			CResMgr::GetInst()->AddResource<CTexture>(strKey, arrRWTex[iIndex]);
		}

		fread(&iIndex, sizeof(UINT), 1, _pFile);
	}

	CMaterial* pMtrl = nullptr;
	pMtrl = CResMgr::GetInst()->Load<CMaterial>(strKey);
	if (pMtrl == nullptr)
	{
		pMtrl = new CMaterial;
		CResMgr::GetInst()->AddResource<CMaterial>(strKey, pMtrl);
	}
	else
		pMtrl = pMtrl->Clone();

	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(strShader));
	pMtrl->m_Param = tParam;

	memcpy(pMtrl->m_arrTexture, arrTex,sizeof(void*) * ((UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0));
	memcpy(pMtrl->m_arrRWTexture, arrRWTex, sizeof(void*) * ((UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0));

	return pMtrl;
}