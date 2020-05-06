#include "CMeshRender.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CTransForm.h"
#include "CMaterial.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "time.h"
#include "CCamera.h"

CMeshRender::CMeshRender()
	:m_bView(true)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
	, m_eRenderType(RENDER_TYPE::DEFFERED)
	, m_bParticle(false)
	, m_iParticleCount(0)
	, m_bShadow(true)
	, m_vUV{1.f,1.f}
{
	m_eType = COMPONENT_TYPE::MESH_RENDER;
}

CMeshRender::CMeshRender(const CMeshRender & _Other)
{
	*this = _Other;
	for (size_t i = 0; i < _Other.m_vecMtrl.size(); i++)
	{
		m_vecMtrl[i] = _Other.m_vecMtrl[i];
	}
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	TransForm()->UpdateData();

	if(Animator2D())	
		Animator2D()->UpdateData();
	else
		Animator2D()->ClearData();

	if (Animator3D())
		Animator3D()->UpdateData();

	if (!m_bView)
		return;

	if (m_pMesh != nullptr && m_vecMtrl[0] != nullptr)
	{
		if (m_pMesh->IsStreamVB() && m_pSubMtrl != nullptr)
		{
			srand(time(NULL));
			for (UINT i = (UINT)SHADER_PARAM::VEC2_0; i < (UINT)SHADER_PARAM::VEC2_END; i++)
			{
				Vec2 vRand = Vec2(rand() % 1000, rand() % 1000);
				m_pMtrl->SetParam(&vRand, (SHADER_PARAM)i);
			}
			
			float fScaleX =TransForm()->GetScale().x;
			float fScaleY = TransForm()->GetScale().y;
			m_pSubMtrl->SetParam(&fScaleX, SHADER_PARAM::FLOAT_0);
			m_pSubMtrl->SetParam(&fScaleY, SHADER_PARAM::FLOAT_1);

			m_pMesh->StreamRender(m_pMtrl, m_pSubMtrl);
		}
		else
		{
			for (size_t i = 0; i < m_vecMtrl.size(); i++)
			{
				if (m_vecMtrl[i] != nullptr)
				{
					Mtx mtxCamInvView = CRenderMgr::GetInst()->GetMainCam()->GetMtxInvView();
					m_vecMtrl[i]->SetParam(&mtxCamInvView, SHADER_PARAM::MTX_0);
					m_vecMtrl[i]->SetParam(&m_vUV, SHADER_PARAM::VEC2_3);

					m_vecMtrl[i]->UpdateData();
					m_pMesh->SetShader(m_vecMtrl[i]->GetShader());

					if(m_bParticle == true)
						m_pMesh->VertexRender(i, m_iParticleCount);
					else
						m_pMesh->Render(i);
				}
			}
		}
	}
}

void CMeshRender::Render(int _iIndex)
{
	if (!GetOwner()->GetShow())
		return;
	TransForm()->UpdateData();

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		Animator2D()->ClearData();

	if (Animator3D())
		Animator3D()->UpdateData();

	if (!m_bView)
		return;

	if (m_pMesh != nullptr && m_vecMtrl[_iIndex] != nullptr)
	{
		if (m_vecMtrl[_iIndex] != nullptr)
		{
			Mtx mtxCamInvView = CRenderMgr::GetInst()->GetMainCam()->GetMtxInvView();
			m_vecMtrl[_iIndex]->SetParam(&mtxCamInvView, SHADER_PARAM::MTX_0);
			m_vecMtrl[_iIndex]->SetParam(&m_vUV, SHADER_PARAM::VEC2_3);
			m_vecMtrl[_iIndex]->UpdateData();
			m_pMesh->SetShader(m_vecMtrl[_iIndex]->GetShader());

			if (m_bParticle == true)
				m_pMesh->VertexRender(_iIndex, m_iParticleCount);
			else
				m_pMesh->Render(_iIndex);
		}
	}
}

LONGLONG CMeshRender::GetInstID(int _iMtrlIdx)
{
	uInstID id = {m_pMesh->GetResID(),m_vecMtrl[_iMtrlIdx]->GetResID(),_iMtrlIdx };
	return id.llID;
}

void CMeshRender::ShadowRender()
{
	TransForm()->UpdateData();

	if (m_pMesh != nullptr)
	{
		CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Shadow");

		if (Animator3D() != nullptr)
		{
			CTexture* pTex = Animator3D()->GetBoneTex();
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_7);
		}
		else
			pMtrl->SetParam(nullptr, SHADER_PARAM::TEX_7);

		for (size_t i = 0; i < m_vecMtrl.size(); i++)
		{
			pMtrl->UpdateData();
			m_pMesh->SetShader(pMtrl->GetShader());
			m_pMesh->Render(i);
		}
	}
}

void CMeshRender::PointShadowRender()
{	
	TransForm()->UpdateData();

	if (m_pMesh != nullptr)
	{
		CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"PointShadow");
		
		if (Animator3D() != nullptr)
		{
			CTexture* pTex = Animator3D()->GetBoneTex();
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_7);
		}

		for (size_t i = 0; i < m_vecMtrl.size(); i++)
		{
			pMtrl->UpdateData();
			m_pMesh->SetShader(pMtrl->GetShader());
			m_pMesh->Render(i);
		}
	}
}

void CMeshRender::ParticleRender()
{

}


void CMeshRender::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_eRenderType, sizeof(UINT), 1, _pFile);
	fwrite(&m_bShadow,sizeof(bool),1,_pFile);
	bool bMesh = false;
		
	if (m_pMesh != nullptr)
		bMesh = true;

	UINT iMtrlCount = m_vecMtrl.size();
		
	fwrite(&bMesh, sizeof(bool), 1, _pFile);
		
	if (bMesh)
	{
		SaveWString(m_pMesh->GetName(), _pFile);
		SaveWString(m_pMesh->GetPath(), _pFile);
	}
	
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < iMtrlCount; i++)
	{
		if (m_vecMtrl[i] != nullptr)
		{
			SaveWString(m_vecMtrl[i]->GetName(), _pFile);

			Vec4 vMtrl = m_vecMtrl[i]->GetDiff();
			fwrite(&vMtrl, sizeof(Vec4), 1, _pFile);


			UINT iTexCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
			fwrite(&iTexCount, sizeof(UINT), 1, _pFile);
			for (UINT j = 0; j < iTexCount; ++j)
			{
				if (m_vecMtrl[i]->GetArrTexture()[j] != nullptr)
					SaveWString(m_vecMtrl[i]->GetArrTexture()[j]->GetName(), _pFile);
				else
					SaveWString(L"", _pFile);
			}

			SaveWString(CRenderMgr::GetInst()->GetBlendName(m_vecMtrl[i]->GetBlend()), _pFile);
			SaveWString(CRenderMgr::GetInst()->GetDepthStencilName(m_vecMtrl[i]->GetDepthStencil()), _pFile);
			UINT iState = (UINT)(m_vecMtrl[i]->GetRasterizeState());
			fwrite(&iState, sizeof(UINT),1,_pFile);

			iState = (UINT)(m_vecMtrl[i]->GetSamplerState());
			fwrite(&iState, sizeof(UINT), 1, _pFile);
		}
	}
	fwrite(&m_vUV, sizeof(Vec2), 1, _pFile);
}

void CMeshRender::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_eRenderType, sizeof(UINT), 1, _pFile);
	fread(&m_bShadow, sizeof(bool), 1, _pFile);

	bool bMesh = false;

	fread(&bMesh, sizeof(bool), 1, _pFile);

	if (bMesh)
	{
		wstring strKey = LoadWString(_pFile);
		wstring strPath = LoadWString(_pFile);
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(strKey);
		if (m_pMesh == nullptr)
			m_pMesh = CMeshData::LoadFile(strPath);
	}
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);

	CMaterial* pMtrl = nullptr;

	for (size_t i = 0; i < iMtrlCount; i++)
	{
		pMtrl = CResMgr::GetInst()->Load<CMaterial>(LoadWString(_pFile))->Clone();

		Vec4 vMtrl;
		fread(&vMtrl, sizeof(Vec4), 1, _pFile);
		pMtrl->SetParam(&vMtrl, SHADER_PARAM::MTRL);

		UINT iTexCount = 0;
		fread(&iTexCount, sizeof(UINT), 1, _pFile);

		for (UINT i = 0; i < iTexCount; i++)
		{
			UINT iTex = i + (UINT)SHADER_PARAM::TEX_0;
			wstring wcsTex = LoadWString(_pFile);
			if (wcsTex != L"")
				pMtrl->SetParam(CResMgr::GetInst()->Load<CTexture>(wcsTex), (SHADER_PARAM)iTex);
		}

		wstring wcsBlend = LoadWString(_pFile);
		pMtrl->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsBlend));

		wstring wcsDepthStencil = LoadWString(_pFile);
		pMtrl->SetDepthStencil(CRenderMgr::GetInst()->GetDepthStencil(wcsDepthStencil));

		UINT iState = 0;
		fread(&iState, sizeof(UINT), 1, _pFile);
		pMtrl->SetRasterizeState((RASTERIZE_TYPE)iState);

		fread(&iState, sizeof(UINT), 1, _pFile);
		pMtrl->SetSamplerState((SAMPLER_TYPE)iState);

		m_vecMtrl.push_back(pMtrl);
	}
	
	fread(&m_vUV, sizeof(Vec2), 1, _pFile);

}
void CMeshRender::SetMtrlClone(CMaterial * _pMtrl, int _iSubset)
{
	if (m_vecMtrl[_iSubset] != nullptr)
	{
		CResMgr::GetInst()->GetDeleteClone(m_vecMtrl[_iSubset]);
		m_vecMtrl[_iSubset] = nullptr;
	}
	m_vecMtrl[_iSubset] = _pMtrl->Clone();
}

void  CMeshRender::SetMtrl(CMaterial* _pMtrl, int _iSubset)
{
	m_vecMtrl[_iSubset] = _pMtrl;
}

void CMeshRender::SetSubMtrl(CMaterial* _pMtrl)
{
	m_pSubMtrl = _pMtrl;
}

void CMeshRender::SetTexture(CTexture* pTex,SHADER_PARAM _Param, int _iSubset)
{
	m_vecMtrl[_iSubset]->SetParam(pTex, _Param);

	if(pTex == nullptr)
		TransForm()->SetScale(1.f, 1.f, 1.f);
	else
		TransForm()->SetScale((float)pTex->GetWidth(),(float)pTex->GetHeight(),1.f);
}

void CMeshRender::SetParam(void* pValue, SHADER_PARAM _Param, int _iSubset)
{
	m_vecMtrl[_iSubset]->SetParam(pValue, _Param);
}

void CMeshRender::SetMesh(CMesh* _pMesh)
{
	m_pMesh = _pMesh;
	m_vecMtrl.resize(m_pMesh->GetSubsetCount());
}