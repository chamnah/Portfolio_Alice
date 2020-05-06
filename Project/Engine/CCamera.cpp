#include "CCamera.h"
#include "CLayer.h"
#include "CSceneMgr.h"
#include "CCore.h"
#include "CTransForm.h"
#include "CFrustum.h"
#include "CMeshRender.h"
#include "CRenderMgr.h"
#include "KeyMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CDirect.h"
#include "CCollider.h"
#include "CInstBuffer.h"
#include "CAnimator3D.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CRenderMgr.h"
#include "CTexture.h"
#include "TimeMgr.h"
#include "CText.h"

CCamera::CCamera()
	:m_bPerspective(true)
	,m_fNear(1.f)
	,m_fFar(10000.f)
	,m_fFOV(XM_PI / 4.f)
	,m_fScale(1.0f)
	, m_iLayerMask(0xffff)
	, m_bRegister(true)
	, m_bMainCam(false)
	, m_Ray{}
	, m_bRender(true)
{
	m_eType = COMPONENT_TYPE::CAMERA;
	m_pFrustum = new CFrustum;
	bInst = false;
}

CCamera::~CCamera()
{
	delete m_pFrustum;
	m_pFrustum = nullptr;
}

void CCamera::Init()
{
}

int CCamera::Update()
{
	return 0;
}

int CCamera::LateUpdate()
{
	return 0;
}

int CCamera::FinalUpdate()
{
	CalcView();
	m_mtxInvView = m_mtxView.Invert();

	if (m_bPerspective)
		m_mtxProj = XMMatrixPerspectiveFovLH(m_fFOV, CCore::GetInst()->GetResolution().x / CCore::GetInst()->GetResolution().y, m_fNear, m_fFar);
	else
	{
		float fHalfWidth = CCore::GetInst()->GetResolution().x * 0.5f * m_fScale;
		float fHalfHeight = CCore::GetInst()->GetResolution().y * 0.5f * m_fScale;

		if (0 != fHalfWidth && 0 != fHalfHeight)
			m_mtxProj = XMMatrixOrthographicOffCenterLH(-fHalfWidth , fHalfWidth,  -fHalfHeight, fHalfHeight, m_fNear, m_fFar);
	}

	g_World.mtxProj = m_mtxProj;

	m_pFrustum->FinalUpdate();

	if(m_bRegister)
		CRenderMgr::GetInst()->RegisterCamera(this);

	CalcRay();
	return 0;
}

void CCamera::Render(CLayer** _pLayer)
{
	UpdateRender();

	UINT iLayerCount = CSceneMgr::GetInst()->GetCurLayerCount();

	for (UINT i = 0; i < iLayerCount; ++i)
	{
		if (_pLayer[i] != NULL && m_iLayerMask & (1 << i))
		{
			vector<CGameObj*>& vecAll = _pLayer[i]->GetAllObj();
			for (size_t i = 0; i < vecAll.size(); i++)
			{
				Vec3 vPos = vecAll[i]->GetTransForm()->GetPos();
				if (m_pFrustum->CheckCullingRange(vPos, vecAll[i]->GetTransForm()->GetScale().x))
				{
					vecAll[i]->Render();
				}
			}
			vecAll.clear();
		}
	}
}

void CCamera::DeferredRender()
{
	g_World.mtxView = m_mtxView;
	g_World.mtxProj = m_mtxProj;
	
	/*for (size_t i = 0; i < m_vecDeferred.size(); i++)
		m_vecDeferred[i]->Render();*/
	for (auto& mapIter : m_mapInstSingle)
		mapIter.second.clear();

	if (KEYTAB(KEY_TYPE::KEY_I))
	{
		bInst = !bInst;
		if(!bInst)
			CTimeMgr::GetInst()->GetInstObj()->GetText()->SetText(L"Instancing On");
		else
			CTimeMgr::GetInst()->GetInstObj()->GetText()->SetText(L"Instancing Off");
	}

	for (auto& mapInst : m_mapInstGroupD)
	{
		if (mapInst.second.empty())
			continue;
		
		CGameObj* pObj = mapInst.second[0].pObj;
		int iSize = 3;
		if (pObj->GetAnimator3D() != nullptr)
			iSize = 10;

		if (bInst || mapInst.second.size() <= iSize) // 똑같은 녀석이 특정 갯수 만큼 있다면 개별 렌더링을 한다.
		{
			int iCount = mapInst.second.size();
			for (size_t i = 0; i < iCount; i++)
			{
				map<wstring, vector<tInstObj>>::iterator iter = m_mapInstSingle.find(mapInst.second[i].pObj->GetLayerName());
				if (iter == m_mapInstSingle.end())
					m_mapInstSingle.insert(make_pair(mapInst.second[i].pObj->GetLayerName(), vector<tInstObj>{mapInst.second[i]}));
				else
					iter->second.push_back(mapInst.second[i]);
			}
			continue;
		}
		
		CMesh* pMesh = pObj->GetMeshRender()->GetMesh();
		int iMtrlIdx = mapInst.second[0].iMtrlIdx;
		CMaterial* pMtrl = pObj->GetMeshRender()->GetMtrl(iMtrlIdx);

		int iRowIdx = 0;
		CInstBuffer::GetInst()->Clear();
		int iCount = mapInst.second.size();
		for (size_t i = 0; i < iCount; i++)
		{
			InstData data;
			data.mtxWorld = mapInst.second[i].pObj->GetTransForm()->GetWorldMtx();
			data.mtxWV = data.mtxWorld * m_mtxView;
			data.mtxVP = m_mtxView * m_mtxProj;
			data.mtxWVP = data.mtxWorld * m_mtxView * m_mtxProj;
			data.vUV = mapInst.second[i].pObj->GetMeshRender()->GetUV();

			if (pObj->GetAnimator3D() != nullptr)
			{
				data.iRowIdx = iRowIdx++;
				CInstBuffer::GetInst()->AddBoneMtx((pObj->GetAnimator3D()->GetFinalBoneMtx()));
			}
			else
				data.iRowIdx = -1;

			CInstBuffer::GetInst()->AddInstData(data);
		}

		if (pObj->GetAnimator3D() != nullptr)
		{
			CInstBuffer::GetInst()->SetData(pObj->GetAnimator3D()->GetBoneTex());
			pMtrl->SetParam(pObj->GetAnimator3D()->GetBoneTex(), SHADER_PARAM::TEX_7);
		}
		else
			CInstBuffer::GetInst()->SetData(nullptr);

		pMtrl->SetParam(&pObj->GetMeshRender()->GetUV(),SHADER_PARAM::VEC2_3);
		pMtrl->UpdateDataInstancing();
		pMesh->SetShader(pMtrl->GetShader(),true);
		pMesh->InstancingRender(iMtrlIdx);
	}

	for (auto& pair : m_mapInstSingle)
	{
		if (pair.second.empty())
			continue;

		for (size_t i = 0; i < pair.second.size(); i++)
		{
			pair.second[i].pObj->GetMeshRender()->Render(pair.second[i].iMtrlIdx);
		}
	}
}

void CCamera::ForwardRender()
{
	g_World.mtxView = m_mtxView;
	g_World.mtxProj = m_mtxProj;

	/*for (size_t i = 0; i < m_vecForward.size(); i++)
		m_vecForward[i]->Render();*/

	for (auto& mapIter : m_mapInstSingle)
		mapIter.second.clear();

	for (auto& mapInst : m_mapInstGroupF)
	{
		if (mapInst.second.empty())
			continue;

		if (true) // 똑같은 녀석이 특정 갯수 만큼 있다면 개별 렌더링을 한다.
		{
			int iCount = mapInst.second.size();
			for (size_t i = 0; i < iCount; i++)
			{
				map<wstring, vector<tInstObj>>::iterator iter = m_mapInstSingle.find(mapInst.second[i].pObj->GetLayerName());
				if (iter == m_mapInstSingle.end())
				{
					m_mapInstSingle.insert(make_pair(mapInst.second[i].pObj->GetLayerName(), vector<tInstObj>{mapInst.second[i]}));
				}
				else
					iter->second.push_back(mapInst.second[i]);
			}
			continue;
		}

		CGameObj* pObj = mapInst.second[0].pObj;
		CMesh* pMesh = pObj->GetMeshRender()->GetMesh();
	 	int iMtrlIdx = mapInst.second[0].iMtrlIdx;
		CMaterial* pMtrl = pObj->GetMeshRender()->GetMtrl(iMtrlIdx);

		int iRowIdx = 0;
		CInstBuffer::GetInst()->Clear();
		int iCount = mapInst.second.size();
		for (size_t i = 0; i < iCount; i++)
		{
			InstData data;
			data.mtxWorld = mapInst.second[i].pObj->GetTransForm()->GetWorldMtx();
			data.mtxWV = data.mtxWorld * m_mtxView;
			data.mtxVP = m_mtxView * m_mtxProj;
			data.mtxWVP = data.mtxWorld * m_mtxView * m_mtxProj;
			data.iRowIdx = -1;

			CInstBuffer::GetInst()->AddInstData(data);
		}

		CInstBuffer::GetInst()->SetData(nullptr);

		pMtrl->UpdateDataInstancing();
		pMesh->SetShader(pMtrl->GetShader(), true);
		pMesh->InstancingRender(iMtrlIdx);
	}

	for (size_t i = 0; i < m_vecForward.size(); i++)
	{
		m_vecForward[i]->Render();
	}

	for (auto& pair : m_mapInstSingle)
	{
		if (pair.second.empty())
			continue;
		for (size_t i = 0; i < pair.second.size(); i++)
			pair.second[i].pObj->GetMeshRender()->Render(pair.second[i].iMtrlIdx);
	}
}

void CCamera::PostEffectRender()
{
	g_World.mtxView = m_mtxView;
	g_World.mtxProj = m_mtxProj;

	/*for (size_t i = 0; i < m_vecForward.size(); i++)
		m_vecForward[i]->Render();*/

	for (auto& mapIter : m_mapInstSingle)
		mapIter.second.clear();

	for (auto& mapInst : m_mapInstGroupP)
	{
		if (mapInst.second.empty())
			continue;

		if (true) // 똑같은 녀석이 특정 갯수 만큼 있다면 개별 렌더링을 한다.
		{
			int iCount = mapInst.second.size();
			for (size_t i = 0; i < iCount; i++)
			{
				map<wstring, vector<tInstObj>>::iterator iter = m_mapInstSingle.find(mapInst.second[i].pObj->GetLayerName());
				if (iter == m_mapInstSingle.end())
					m_mapInstSingle.insert(make_pair(mapInst.second[i].pObj->GetLayerName(), vector<tInstObj>{mapInst.second[i]}));
				else
					iter->second.push_back(mapInst.second[i]);
			}
			continue;
		}

		CGameObj* pObj = mapInst.second[0].pObj;
		CMesh* pMesh = pObj->GetMeshRender()->GetMesh();
		int iMtrlIdx = mapInst.second[0].iMtrlIdx;
		CMaterial* pMtrl = pObj->GetMeshRender()->GetMtrl(iMtrlIdx);

		int iRowIdx = 0;
		CInstBuffer::GetInst()->Clear();
		int iCount = mapInst.second.size();
		for (size_t i = 0; i < iCount; i++)
		{
			InstData data;
			data.mtxWorld = mapInst.second[i].pObj->GetTransForm()->GetWorldMtx();
			data.mtxWV = data.mtxWorld * m_mtxView;
			data.mtxVP = m_mtxView * m_mtxProj;
			data.mtxWVP = data.mtxWorld * m_mtxView * m_mtxProj;
			data.iRowIdx = -1;

			CInstBuffer::GetInst()->AddInstData(data);
		}

		CInstBuffer::GetInst()->SetData(nullptr);

		pMtrl->UpdateDataInstancing();
		pMesh->SetShader(pMtrl->GetShader(), true);
		pMesh->InstancingRender(iMtrlIdx);

		CONTEXT->CopyResource(CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT)->GetTexture2D(),
			CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::SWAP)->GetTexture2D());
	}

	for (size_t i = 0; i < m_vecFilter.size(); i++)
	{
		m_vecFilter[i]->Render();

		CONTEXT->CopyResource(CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT)->GetTexture2D(),
			CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::SWAP)->GetTexture2D());
	}

	for (auto& pair : m_mapInstSingle)
	{
		if (pair.second.empty())
			continue;

		for (size_t i = 0; i < pair.second.size(); i++)
		{
			pair.second[i].pObj->GetMeshRender()->Render(pair.second[i].iMtrlIdx);
			
			CONTEXT->CopyResource(CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT)->GetTexture2D(),
				CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::SWAP)->GetTexture2D());
		}
	}

	
}

void CCamera::ShadowRender()
{
	g_World.mtxProj = m_mtxProj;
	g_World.mtxView = m_mtxView;

	for (size_t i = 0; i < m_vecShadow.size(); i++)
		m_vecShadow[i]->GetMeshRender()->ShadowRender();
}

void CCamera::PointShadowRender()
{
	g_World.mtxProj = m_mtxProj;
	g_World.mtxView = m_mtxView;
	for (size_t i = 0; i < m_vecShadow.size(); i++)
		m_vecShadow[i]->GetMeshRender()->PointShadowRender();
}

void CCamera::CollRender()
{
	g_World.mtxView = m_mtxView;
	g_World.mtxProj = m_mtxProj;
	
	for (size_t i = 0; i < m_vecColl.size(); i++)
		m_vecColl[i]->Render();
}

void CCamera::TextRender()
{
	for (size_t i = 0; i < m_vecText.size(); i++)
	{
		m_vecText[i]->Render();
	}
}

void CCamera::UpdateRender()
{
}

void CCamera::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_bPerspective, sizeof(bool), 1, _pFile);
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _pFile);
	fwrite(&m_bMainCam, sizeof(bool), 1, _pFile);
}

void CCamera::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_bPerspective, sizeof(bool), 1, _pFile);
	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);
	fread(&m_iLayerMask, sizeof(UINT), 1, _pFile);
	fread(&m_bMainCam, sizeof(bool), 1, _pFile);
}

void CCamera::CheckLayerMask(UINT _iMask)
{
	m_iLayerMask ^= (1 << _iMask);
}

// 인스턴싱 할때에 재질이 몇번째인지 계산을 해서 렌더링한다.
void CCamera::SortGameObject()
{	
	RenderClear();

	/*m_vecDeferred.clear();
	m_vecForward.clear();*/
	m_vecColl.clear();
	m_vecText.clear();
	CLayer* pLayer = nullptr;
	UINT iCount = CSceneMgr::GetInst()->GetCurLayerCount();

	for (UINT i = 0; i < iCount; i++)
	{
		pLayer = CSceneMgr::GetInst()->GetLayer(i);
		
		if (pLayer == nullptr || !(m_iLayerMask & (1 << i)))
			continue;

		if (pLayer->GetName() == L"Camera" || pLayer->GetName() == L"Light")
			continue;

		vector<CGameObj*>&vecObj = pLayer->GetAllObj();

		//for (size_t j = 0; j < vecObj.size(); j++)
		//{
		//	if (vecObj[j]->GetMeshRender() != nullptr)
		//	{
		//		if (vecObj[j]->GetMeshRender()->GetDeferred())
		//			m_vecDeferred.push_back(vecObj[j]);
		//		else
		//			m_vecForward.push_back(vecObj[j]);
		//	}
		//
		//	if (vecObj[j]->GetCollider() != nullptr)
		//	{
		//		m_vecColl.push_back(vecObj[j]->GetCollider());
		//	}
		//}
	
		map<LONGLONG, vector<tInstObj>>* mapInst = nullptr;
		for (size_t j = 0; j < vecObj.size(); j++)
		{
			CCollider* pColl = vecObj[j]->GetCollider();
			if (pColl != nullptr)
			{
				m_vecColl.push_back(pColl);
				
				if (vecObj[j]->GetLayerName() != L"Player")
				{
					if (!m_pFrustum->CheckCullingCollider(pColl))
						continue;
				}
			}
			/*else
			{
				if (!m_pFrustum->CheckCullingPoint(vecObj[j]->GetTransForm()->GetPos()))
					continue;
			}*/

			if (vecObj[j]->GetMeshRender() != nullptr)
			{
				if (vecObj[j]->GetMeshRender()->GetRenderType() == RENDER_TYPE::DEFFERED)
					mapInst = &m_mapInstGroupD;
				else if (vecObj[j]->GetMeshRender()->GetRenderType() == RENDER_TYPE::FORWARD)
					mapInst = &m_mapInstGroupF;
				else if (vecObj[j]->GetMeshRender()->GetRenderType() == RENDER_TYPE::POSTEFFECT)
					mapInst = &m_mapInstGroupP;
				else if (vecObj[j]->GetMeshRender()->GetRenderType() == RENDER_TYPE::NO_INST)
				{
					m_vecForward.push_back(vecObj[j]);
					continue;
				}
				UINT iMtrlCount = vecObj[j]->GetMeshRender()->GetVecMtrl().size();
				for (UINT k = 0; k < iMtrlCount; k++)
				{
					if (vecObj[j]->GetMeshRender()->GetVecMtrl()[k] == nullptr || vecObj[j]->GetMeshRender()->GetMesh() == nullptr)
						continue;

					LONGLONG llID = vecObj[j]->GetMeshRender()->GetInstID(k);
					map<LONGLONG, vector<tInstObj>>::iterator iter = mapInst->find(llID);
					if (iter == mapInst->end())
						mapInst->insert(make_pair(llID, vector<tInstObj>{tInstObj{ vecObj[j] ,k }}));
					else
						iter->second.push_back(tInstObj{ vecObj[j] ,k });
				}
			}

			if (vecObj[j]->GetFilter() != nullptr)
			{
				m_vecFilter.push_back(vecObj[j]);
			}

			if (vecObj[j]->GetText() != nullptr)
			{
				m_vecText.push_back(vecObj[j]);
			}
		}
			/*if (vecObj[j]->GetFilter() != nullptr)
			{
				mapInst = &m_mapInstGroupF;
				m_vecForward.push_back(vecObj[j]);
			}*/
			//if(m_pFrustum->CheckCullingPoint(vecObj[i]->GetTransForm()->GetPos()/*, vecObj[i]->GetTransForm()->GetScale().x*/))
			//{
			//	vecObj[i]->SetFrustum(true);
			//	if (vecObj[i]->GetMeshRender()->GetDeferred())
			//		m_vecDeferred.push_back(vecObj[i]);
			//	else
			//		m_vecForward.push_back(vecObj[i]);
			//}
			//else
			//{
			//	vecObj[i]->SetFrustum(false);
			//}
	}
}

void CCamera::SortShadowObject()
{
	m_vecShadow.clear();

	UINT iCount = CSceneMgr::GetInst()->GetCurLayerCount();
	for (UINT i = 0; i < iCount; i++)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(i);

		if (pLayer == nullptr || !(m_iLayerMask & (1 << i)))
			continue;

		vector<CGameObj*>& vecObj = pLayer->GetAllObj();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetMeshRender() != nullptr)
			{
				if (vecObj[i]->GetMeshRender()->GetShadow())
				{
					m_vecShadow.push_back(vecObj[i]);
				}
			}
		}
	}
}

void CCamera::CalcView()
{
	Vec3 vPos = -TransForm()->GetWorldMtx().Translation();

	m_mtxView = XMMatrixIdentity();

	m_mtxView.Right(TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
	m_mtxView.Up(TransForm()->GetWorldDir(DIR_TYPE::UP));
	m_mtxView.Backward(TransForm()->GetWorldDir(DIR_TYPE::FRONT));

	m_mtxView = m_mtxView.Transpose(); // x , y, z 로의 축에대한 내적을 위해 전치행렬로 만들어 준다.

	m_mtxView._41 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
	m_mtxView._42 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::UP));
	m_mtxView._43 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::FRONT));

	g_World.mtxView = m_mtxView;//XMMatrixTranslation(m_vViewPos.x, m_vViewPos.y, m_vViewPos.z) * XMMatrixTranspose(XMMatrixRotationY(XMConvertToRadians(TransForm()->GetRotaion().x)));
}

void CCamera::CalcRay()
{
	D3D11_VIEWPORT viewport = {};
	UINT iNum = 1;
	CONTEXT->RSGetViewports(&iNum, &viewport);
	Vec3& vMousePos = CKeyMgr::GetInst()->GetWindowMousePos();
	
	m_Ray.vStart = TransForm()->GetWorldPos();
	m_Ray.vDir.x = (((vMousePos.x - viewport.TopLeftX) * 2.f / viewport.Width) - 1.f) / m_mtxProj._11;
	m_Ray.vDir.y = -(((vMousePos.y - viewport.TopLeftY) * 2.f / viewport.Height) - 1.f) / m_mtxProj._22;
	m_Ray.vDir.z = 1.f;

	m_Ray.vDir = XMVector3TransformNormal(m_Ray.vDir,m_mtxInvView);
	m_Ray.vDir.Normalize();
}

bool cmpView::operator()(tInstObj& t, tInstObj& u)
{
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCam();
	Vec3 vCamPos = pCam->TransForm()->GetPos();
	return Vec3::Distance(t.pObj->GetTransForm()->GetPos(), vCamPos) < Vec3::Distance(u.pObj->GetTransForm()->GetPos(), vCamPos);
}

void CCamera::RenderClear()
{
	for (auto& mapIter : m_mapInstGroupD)
		mapIter.second.clear();

	for (auto& mapIter : m_mapInstGroupF)
		mapIter.second.clear();

	for (auto& mapIter : m_mapInstGroupP)
		mapIter.second.clear();

	m_vecFilter.clear();
	m_vecForward.clear();
}