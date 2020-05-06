#include "CMeshData.h"
#include "CFBXLoader.h"
#include "CMesh.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"
#include "CPathMgr.h"

CMeshData::CMeshData()
	: m_bAnim(true)
{
}

CMeshData::~CMeshData()
{
}

void CMeshData::LoadFBX(const wstring _strPath)
{
	SetName(CPathMgr::GetInst()->GetFileName(_strPath));
	m_strPath = _strPath;

	CFBXLoader loader;
	loader.Init();
	loader.LoadFBX(_strPath);

	m_pMesh = CMesh::CreateFromContainer(&loader);
	m_pMesh->SetName(CPathMgr::GetInst()->GetFileName(_strPath));
	m_pMesh->SetPath(_strPath);

	CResMgr::GetInst()->AddResource<CMesh>(GetName(), m_pMesh);

	vector<tContainer>& Container = loader.GetContainer();
	wstring strKey;
	CTexture* pTex = nullptr;
	// 모든 컨테이너를 돌면서 그 속해있는 서브셋 갯수만큼 또 돌면서 Mtrl을 보관한다.
	for (size_t i = 0; i < Container.size(); i++)
	{
		for (size_t j = 0; j < Container[i].vecMtrl.size(); j++)
		{
			CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(Container[i].vecMtrl[j].strName)->Clone();

			strKey = CPathMgr::GetInst()->GetFileName(Container[i].vecMtrl[j].strDiff);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_0);

			strKey = CPathMgr::GetInst()->GetFileName(Container[i].vecMtrl[j].strNormal);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_1);

			strKey = CPathMgr::GetInst()->GetFileName(Container[i].vecMtrl[j].strSpec);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_2);

			strKey = CPathMgr::GetInst()->GetFileName(Container[i].vecMtrl[j].strAmb);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_3); // 마스킹용 텍스쳐를 ambient에 넣어놓았다.

			//pMtrl->SetParam(&Container[i].vecMtrl[j].mtrl.vDiff, SHADER_PARAM::MTRL);

			if (pMtrl != nullptr)
				m_vecMtrl.push_back(pMtrl);
		}
	} // 매터리얼 계수도 추가 해줘야한다.
}

CGameObj * CMeshData::instantiate()
{
	CGameObj* pObj = new CGameObj;
	pObj->AddComponent(new CMeshRender);
	pObj->GetMeshRender()->SetMesh(m_pMesh);

	for (size_t i = 0; i < m_pMesh->GetSubsetCount(); i++)
	{
		pObj->GetMeshRender()->SetMtrl(m_vecMtrl[i],i);
	}

	if (!m_pMesh->IsAnimation() || !m_bAnim)
		return pObj;
	
	wstring strBoneTex = m_pMesh->GetName();
	strBoneTex += L"BoneTex";
	CTexture* pBoneTex = CTexture::CreateTexture(m_pMesh->GetBone()->size() * 4, 1, D3D10_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC); //CResMgr::GetInst()->CreateTexture(strBoneTex, m_pMesh->GetBone()->size() * 4, 1, D3D10_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DYNAMIC);
	m_pMesh->SetBoneTex(pBoneTex);

	pObj->AddComponent(new CAnimator3D);
	pObj->GetAnimator3D()->SetBoneTex(pBoneTex);
	pObj->GetAnimator3D()->SetBone(m_pMesh->GetBone());
	pObj->GetAnimator3D()->SetAnimClip(m_pMesh->GetAnimClip());

	return pObj;
}

void CMeshData::Save(FILE * _pFile)
{
	SaveWString(GetName(),_pFile);
	SaveWString(m_strPath, _pFile);
}

void CMeshData::SaveMeshDataFile()
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Mesh\\MeshData\\";
	strPath += GetName() + L".mdat";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");
	m_pMesh->SaveMeshData(pFile);

	UINT iMtrlSize = m_vecMtrl.size();
	fwrite(&iMtrlSize, sizeof(UINT), 1, pFile);

	for (size_t i = 0; i < iMtrlSize; i++)
		m_vecMtrl[i]->SaveMeshData(pFile);

	fclose(pFile);
}

CMeshData* CMeshData::LoadMeshDataFile(wstring _strPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile,_strPath.c_str(),L"rb");
	CMesh* pMesh = CMesh::LoadMeshData(pFile);
	pMesh->SetName(CPathMgr::GetInst()->GetFileName(_strPath));
	pMesh->SetPath(_strPath);

	UINT iMtrlSize = 0;
	fread(&iMtrlSize, sizeof(UINT), 1, pFile);

	vector<CMaterial*> vecMtrl;
	for (size_t i = 0; i < iMtrlSize; i++)
	{
		vecMtrl.push_back(CMaterial::LoadMeshData(pFile));
	}
	fclose(pFile);

	CMeshData* pData = new CMeshData;
	pData->m_strPath = _strPath;
	pData->m_pMesh = pMesh;
	pData->m_vecMtrl = vecMtrl;
	pData->SetName(CPathMgr::GetInst()->GetFileName(_strPath));
	return pData;
}

void CMeshData::Load(FILE * _pFile)
{
	wstring strKey = LoadWString(_pFile);
	wstring strPath = LoadWString(_pFile);

	if (nullptr == CResMgr::GetInst()->Load<CMeshData>(strKey))
	{
		wstring strExt = CPathMgr::GetInst()->GetFileExtension(strPath);
		if(strExt == L".fbx")
			CResMgr::GetInst()->LoadFBX(strPath);
		else if(strExt == L".mdat")
			CResMgr::GetInst()->LoadMeshData(strPath);
	}
}

CMesh * CMeshData::LoadFile(wstring _strPath)
{
	wstring strExt = CPathMgr::GetInst()->GetFileExtension(_strPath);
	CMeshData* pData = nullptr;
	if (strExt == L".fbx")
		pData = CResMgr::GetInst()->LoadFBX(_strPath);
	else if (strExt == L".mdat")
		pData = CResMgr::GetInst()->LoadMeshData(_strPath);
	else
		return nullptr;
	return pData->GetMesh();
}
