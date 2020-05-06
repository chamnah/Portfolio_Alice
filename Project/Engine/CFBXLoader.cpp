#include "CFBXLoader.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CShaderMgr.h"
#include <algorithm>
CFBXLoader::CFBXLoader()
{
}

CFBXLoader::~CFBXLoader()
{
	m_pScene->Destroy();
	m_pManager->Destroy();

	SafeArrayDelete(m_vecAnimClip);
	SafeArrayDelete(m_vecBone);
	for (size_t i = 0; i < m_arrAnimName.Size(); i++)
	{
		SAFE_DELETE(m_arrAnimName[i]);
	}
}

void CFBXLoader::Init()
{
	m_pManager = FbxManager::Create();
	if (m_pManager == nullptr)
		assert(NULL);

	FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIOSettings);

	m_pScene = FbxScene::Create(m_pManager, "");
	if (m_pScene == nullptr)
		assert(NULL);
}

void CFBXLoader::LoadFBX(const wstring _strPath)
{
	m_pImporter = FbxImporter::Create(m_pManager,"");

	string strPath(_strPath.begin(), _strPath.end());
	if (!m_pImporter->Initialize(strPath.c_str(), -1, m_pManager->GetIOSettings()))
		assert(NULL);

	m_pImporter->Import(m_pScene);

	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max); // 여기서 Max는 3d Max 기준 좌표계를 의미하는거 같다.
	m_pScene->FillAnimStackNameArray(m_arrAnimName);
	LoadAnimationClip();
	LoadSkeleton(m_pScene->GetRootNode(),0,0,-1);
	Triangulate(m_pScene->GetRootNode());
	m_vecContainer.clear();


	LoadMeshDataFromNode(m_pScene->GetRootNode());
	m_pImporter->Destroy();

	CreateTexture();
	//CreaetMaterial();
}

void CFBXLoader::LoadSkeleton(FbxNode * _pNode,int _iDepth,int _iIdx,int _iParentIdx)
{
	FbxNodeAttribute* pAtt = _pNode->GetNodeAttribute();
	if (pAtt && pAtt->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		tBone* pBone = new tBone;
		string strName = _pNode->GetName();
		pBone->strName = wstring(strName.begin(), strName.end());
		pBone->iDepth = _iDepth++;
		pBone->iParent = _iParentIdx;
		pBone->pNode = _pNode;
		m_vecBone.push_back(pBone);

		if (strName == "Bip01-Prop1")
		{
			m_pSocket = _pNode;
		}
	}

	for (size_t i = 0; i < _pNode->GetChildCount(); i++)
	{
		LoadSkeleton(_pNode->GetChild(i), _iDepth, m_vecBone.size(), _iIdx);
	}
}

void CFBXLoader::LoadAnimationClip()
{
	for (size_t i = 0; i < m_arrAnimName.GetCount(); i++)
	{
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());

		if (!pAnimStack)
			continue;

		tAnimClip* pAnimClip = new tAnimClip;
		string strName = pAnimStack->GetName();
		pAnimClip->strName = wstring(strName.begin(), strName.end());
		
		FbxTakeInfo* pInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());
		pAnimClip->tStartTime = pInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pInfo->mLocalTimeSpan.GetStop();
		pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) - pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);

		m_vecAnimClip.push_back(pAnimClip);
	}
}

void CFBXLoader::LoadAnimationData(FbxMesh* _pMesh,tContainer* _pContainer)
{
	int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (iSkinCount <= 0 || m_vecAnimClip.empty())
		return;

	_pContainer->bAnimaion = true;

	for (size_t i = 0; i < iSkinCount; i++)
	{
		FbxSkin* pSkin = ((FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin));

		if (pSkin == nullptr)
			continue;

		FbxSkin::EType eType = pSkin->GetSkinningType();

		if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
		{
			int iClusterCount = pSkin->GetClusterCount();

			for (size_t j = 0; j < iClusterCount; j++)
			{
				FbxCluster* pCluster = pSkin->GetCluster(j);

				if (!pCluster->GetLink())
					continue;

				int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());

				FbxAMatrix mtxMesh = GetTransForm(_pMesh->GetNode());

				LoadWeightAndIndices(pCluster,iBoneIdx,_pContainer);
				LoadOffsetMatrix(pCluster, mtxMesh, iBoneIdx, _pContainer);
				LoadKeyframeTransform(_pMesh->GetNode(), pCluster, mtxMesh, iBoneIdx, _pContainer);
			}
		}
	}

	CheckWeightAndIndices(_pMesh,_pContainer);
	
	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxLongLong llStartFrame = m_vecAnimClip[0]->tStartTime.GetFrameCount(eTimeMode);
	FbxLongLong llEndFrame = m_vecAnimClip[0]->tEndTime.GetFrameCount(eTimeMode);

	for (size_t i = 0; i < m_vecBone.size(); i++)
	{
		if (!m_vecBone[i]->vecKeyFrame.empty())
			continue;

		for (FbxLongLong j = llStartFrame; j <= llEndFrame; ++j)
		{
			tKeyFrame tFrame = {};
			FbxTime   tTime = 0.;
			tTime.SetFrame(j, eTimeMode);

			FbxAMatrix mtxMesh = GetTransForm(_pMesh->GetNode());
			FbxAMatrix matFromNode = _pMesh->GetNode()->EvaluateGlobalTransform(tTime) * mtxMesh;
			FbxAMatrix matCurTrans = matFromNode.Inverse() * m_vecBone[i]->pNode->EvaluateGlobalTransform(tTime);
			matCurTrans = matReflect * matCurTrans * matReflect;

			tFrame.dTime = tTime.GetSecondDouble();
			tFrame.matTransform = matCurTrans;

			m_vecBone[i]->vecKeyFrame.push_back(tFrame);
		}
	}
}

void CFBXLoader::LoadWeightAndIndices(FbxCluster * _pCluster, int _iBoneIdx, tContainer * _pContainer)
{
	int iIdicesCount = _pCluster->GetControlPointIndicesCount(); // 각 관절이 영향을 주는 정점에 갯수
	for (size_t i = 0; i < iIdicesCount; i++)
	{
		tSkining tSkin = {};
		tSkin.iBoneIdx = _iBoneIdx; // 관절
		tSkin.iWeight = _pCluster->GetControlPointWeights()[i]; // 관절에 영향받는 정도
		int iVtxId = _pCluster->GetControlPointIndices()[i]; // 정점 번호
		_pContainer->vecSkin[iVtxId].push_back(tSkin);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster * _pCluster, FbxAMatrix& _mtxTransForm, int _iBoneIdx, tContainer * _pContainer)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;

	_pCluster->GetTransformMatrix(matClusterTrans);
	_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;

	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _mtxTransForm;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[_iBoneIdx]->mtxOffset = matOffset;
}

void CFBXLoader::LoadKeyframeTransform(FbxNode * _pNode, FbxCluster * _pCluster, const FbxAMatrix & _mtxNode, int _iBoneIdx, tContainer * _pContainer)
{
	if (m_vecAnimClip.empty())
		return;


	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	m_vecBone[_iBoneIdx]->mtxTransForm = _mtxNode;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxLongLong llStartFrame = m_vecAnimClip[0]->tStartTime.GetFrameCount(eTimeMode);
	FbxLongLong llEndFrame = m_vecAnimClip[0]->tEndTime.GetFrameCount(eTimeMode);

	for (FbxLongLong i = llStartFrame; i <= llEndFrame; ++i)
	{
		tKeyFrame tFrame = {};
		FbxTime   tTime = 0.;

		tTime.SetFrame(i, eTimeMode);


		FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _mtxNode;
		FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
		matCurTrans = matReflect * matCurTrans * matReflect;

		tFrame.dTime = tTime.GetSecondDouble();
		tFrame.matTransform = matCurTrans;

		m_vecBone[_iBoneIdx]->vecKeyFrame.push_back(tFrame);
	}
}

void CFBXLoader::Triangulate(FbxNode * _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
		pAttr->GetAttributeType() == FbxNodeAttribute::ePatch))
	{
		FbxGeometryConverter convert(m_pManager);
		convert.Triangulate(pAttr,true);
	}

	int iChildCount = _pNode->GetChildCount();
	for (int i = 0; i < iChildCount; i++)
	{
		Triangulate(_pNode->GetChild(i));
	}
}

void CFBXLoader::LoadMeshDataFromNode(FbxNode * _pNode) // Mesh와 Material 정보 읽어오기
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();
	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = _pNode->GetMesh();
		if (pMesh != nullptr)
			LoadMesh(pMesh);
	}

	int iMtrlCount = _pNode->GetMaterialCount(); // Material SubSet 갯수
	for (int i = 0; i < iMtrlCount; i++)
	{
		FbxSurfaceMaterial* pMtrl = _pNode->GetMaterial(i);
		LoadMaterial(pMtrl);
	}

	int iChildCount = _pNode->GetChildCount();
	for (int i = 0; i < iChildCount; i++)
		LoadMeshDataFromNode(_pNode->GetChild(i));
}

void CFBXLoader::LoadMesh(FbxMesh * _pMesh)
{
	m_vecContainer.push_back(tContainer{});
	tContainer& container = m_vecContainer.back();

	string strName = _pMesh->GetName();
	container.strName = wstring(strName.begin(),strName.end());

	UINT iVtxCount = _pMesh->GetControlPointsCount();
	container.Resize(iVtxCount);
	FbxVector4* pVtxPos = _pMesh->GetControlPoints();

	for (UINT i = 0; i < iVtxCount; i++) // Max에서 뽑은 Fbx좌표계는 directx 좌표계와 y와 z가 반대로 들어있다.
	{
		container.vecVtxPos[i].x =  pVtxPos[i].mData[0];
		container.vecVtxPos[i].y = pVtxPos[i].mData[2];
		container.vecVtxPos[i].z = pVtxPos[i].mData[1];
	}

	int iPolyCount = _pMesh->GetPolygonCount();
	int iSubSetCount = _pMesh->GetNode()->GetMaterialCount(); // 하나의 노드(Container 하나당이라고도 할 수 있음)에서의 SubSet 갯수 
	container.vecIdx.resize(iSubSetCount);

	FbxGeometryElementMaterial* pMtrl = _pMesh->GetElementMaterial();

	int iPolyVtxCount = _pMesh->GetPolygonSize(0); // 폴리곤을 이루는 정점 갯수
	if (3 != iPolyVtxCount) // 삼각형이 아니라는 의미
		assert(NULL);

	UINT iPolyIdx[3] = {}; // 삼각형을 이루는 폴리곤 인덱스
	int  iVertexCount = 0;

	for (int i = 0; i < iPolyCount; i++)
	{
		for (int j = 0; j < iPolyVtxCount; j++)
		{
			iPolyIdx[j] = _pMesh->GetPolygonVertex(i,j); // i번째 폴리곤에 j번째 정점이 몇번 인덱스인가

			LoadTangent(_pMesh, container, iPolyIdx[j], iVertexCount);
			LoadNormal(_pMesh, container, iPolyIdx[j], iVertexCount);
			LoadBinormal(_pMesh, container, iPolyIdx[j], iVertexCount);
			LoadUV(_pMesh, container, iPolyIdx[j], _pMesh->GetTextureUVIndex(i,j));
			++iVertexCount;
		}

		int iSubIdx = pMtrl->GetIndexArray().GetAt(i); // 현재 폴리곤인덱스 녀석은 어떤 SubSet에 속해 있는지
		container.vecIdx[iSubIdx].push_back(iPolyIdx[0]);
		container.vecIdx[iSubIdx].push_back(iPolyIdx[2]);
		container.vecIdx[iSubIdx].push_back(iPolyIdx[1]); // 이렇게 넣어야 시계방향으로 인덱스가 감긴다.
	}

	LoadAnimationData(_pMesh, &container);
}

void CFBXLoader::LoadTangent(FbxMesh * _pMesh, tContainer&  _pContainer, int _iIdx, int _iVtxOrder)
{
	int iTangentCount =_pMesh->GetElementTangentCount();
	if (iTangentCount != 1)
		assert(NULL);

	FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();

	int iTangentIdx = 0;
	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect) // 참조 방식
			iTangentIdx = _iVtxOrder;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect) 
			iTangentIdx = _iIdx;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 TangentData = pTangent->GetDirectArray().GetAt(iTangentIdx);
	_pContainer.vecVtxTangent[_iIdx].x = TangentData.mData[0];
	_pContainer.vecVtxTangent[_iIdx].y = TangentData.mData[2];
	_pContainer.vecVtxTangent[_iIdx].z = TangentData.mData[1];

	// Tangent가 담겨있는게 0번째 폴리곤에 있는 정점에 값부터 차례대로 쭈우욱 들어있다.
	// 그래서 그 정점에 인덱스를 넘겨줘서 그 인덱스 번째로 정점을 옮겨주고 있다.
}

void CFBXLoader::LoadNormal(FbxMesh * _pMesh, tContainer & _pContainer, int _iIdx, int _iVtxOrder)
{
	int iNormalCount = _pMesh->GetElementNormalCount();
	if (iNormalCount != 1)
		assert(NULL);

	FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();

	int iNormalIdx = 0;
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect) // 참조 방식
			iNormalIdx = _iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 NormalData = pNormal->GetDirectArray().GetAt(iNormalIdx);
	_pContainer.vecVtxNormal[_iIdx].x = NormalData.mData[0];
	_pContainer.vecVtxNormal[_iIdx].y = NormalData.mData[2];
	_pContainer.vecVtxNormal[_iIdx].z = NormalData.mData[1];
}

void CFBXLoader::LoadBinormal(FbxMesh * _pMesh, tContainer & _pContainer, int _iIdx, int _iVtxOrder)
{
	int iBinormalCount = _pMesh->GetElementBinormalCount();
	if (iBinormalCount != 1)
		assert(NULL);

	FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();

	int iBinormalIdx = 0;
	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect) // 참조 방식
			iBinormalIdx = _iVtxOrder;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iIdx;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 BinormalData = pBinormal->GetDirectArray().GetAt(iBinormalIdx);
	_pContainer.vecVtxBinormal[_iIdx].x = BinormalData.mData[0];
	_pContainer.vecVtxBinormal[_iIdx].y = BinormalData.mData[2];
	_pContainer.vecVtxBinormal[_iIdx].z = BinormalData.mData[1];
}

void CFBXLoader::LoadUV(FbxMesh * _pMesh, tContainer & _pContainer, int _iIdx, int _iUVIndex)
{
	FbxGeometryElementUV* pUV = _pMesh->GetElementUV();
	FbxVector2 UV = pUV->GetDirectArray().GetAt(_iUVIndex);

	_pContainer.vecVtxUV[_iIdx].x = (float)UV.mData[0];
	_pContainer.vecVtxUV[_iIdx].y = 1.f - (float)UV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial * _pMtrl)
{
	tFbxMaterial mtrlInfo = {};

	string strName = _pMtrl->GetName();
	mtrlInfo.strName = L"3d_Deferred";
	mtrlInfo.mtrl.vDiff = LoadMaterialData(_pMtrl,FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	mtrlInfo.mtrl.vAmb = LoadMaterialData(_pMtrl, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	mtrlInfo.mtrl.vSpec = LoadMaterialData(_pMtrl, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	mtrlInfo.mtrl.vEmiv = LoadMaterialData(_pMtrl, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	mtrlInfo.strDiff = LoadMtrlTextureName(_pMtrl, FbxSurfaceMaterial::sDiffuse);
	mtrlInfo.strAmb = LoadMtrlTextureName(_pMtrl, FbxSurfaceMaterial::sAmbient);
	mtrlInfo.strSpec = LoadMtrlTextureName(_pMtrl, FbxSurfaceMaterial::sSpecular);
	mtrlInfo.strEmiv = LoadMtrlTextureName(_pMtrl, FbxSurfaceMaterial::sEmissive);
	mtrlInfo.strNormal = LoadMtrlTextureName(_pMtrl, FbxSurfaceMaterial::sNormalMap);

	m_vecContainer.back().vecMtrl.push_back(mtrlInfo);
}

Vec4 CFBXLoader::LoadMaterialData(FbxSurfaceMaterial * _pMtrl, const char * _strMtrlName, const char * _strFactorName)
{
	FbxProperty mtrlProperty = _pMtrl->FindProperty(_strMtrlName);
	FbxProperty mtrlFactorProperty = _pMtrl->FindProperty(_strFactorName);

	FbxDouble3 vMtrl = {};
	FbxDouble  vFactor = 0.f;
	if (mtrlProperty.IsValid() && mtrlFactorProperty.IsValid())
	{
		vMtrl = mtrlProperty.Get<FbxDouble3>();
		vFactor = mtrlFactorProperty.Get<FbxDouble>();
	}

	Vec4 vReturn(vMtrl.mData[0] * vFactor, vMtrl.mData[1] * vFactor, vMtrl.mData[2] * vFactor, vFactor);
	return vReturn;
}

wstring CFBXLoader::LoadMtrlTextureName(FbxSurfaceMaterial * _pMtrl, const char * _strMtrlName)
{
	string strName;
	FbxProperty MtrlPro = _pMtrl->FindProperty(_strMtrlName);

	if (MtrlPro.IsValid())
	{
		int SrcCount = MtrlPro.GetSrcObjectCount();
		if (SrcCount > 0)
		{
			FbxFileTexture* Texture = MtrlPro.GetSrcObject<FbxFileTexture>(0);
			if (Texture != NULL)
				strName = Texture->GetFileName();
		}
	}

	return wstring(strName.begin(), strName.end());
}

void CFBXLoader::CreateTexture()
{
	for (size_t i = 0; i < m_vecContainer.size(); i++)
	{
		for (size_t j = 0; j < m_vecContainer[i].vecMtrl.size(); j++)
		{
			wstring strDiff = m_vecContainer[i].vecMtrl[j].strDiff;
			if(strDiff != L"")
				CResMgr::GetInst()->AddResource<CTexture>(CPathMgr::GetInst()->GetFileName(strDiff), CTexture::CreateTexture(strDiff));

			wstring strAmb = m_vecContainer[i].vecMtrl[j].strAmb;
			if (strAmb != L"")
				CResMgr::GetInst()->AddResource<CTexture>(CPathMgr::GetInst()->GetFileName(strAmb), CTexture::CreateTexture(strAmb));

			wstring strSpec = m_vecContainer[i].vecMtrl[j].strSpec;
			if (strSpec != L"")
				CResMgr::GetInst()->AddResource<CTexture>(CPathMgr::GetInst()->GetFileName(strSpec), CTexture::CreateTexture(strSpec));

			wstring strEmiv = m_vecContainer[i].vecMtrl[j].strEmiv;
			if (strEmiv != L"")
				CResMgr::GetInst()->AddResource<CTexture>(CPathMgr::GetInst()->GetFileName(strEmiv), CTexture::CreateTexture(strEmiv));

			wstring strNormal = m_vecContainer[i].vecMtrl[j].strNormal;
			if (strNormal != L"")
				CResMgr::GetInst()->AddResource<CTexture>(CPathMgr::GetInst()->GetFileName(strNormal), CTexture::CreateTexture(strNormal));
		}
	}
}

void CFBXLoader::CreaetMaterial()
{
	wstring strKey;
	CTexture* pTex = nullptr;
	for (size_t i = 0; i < m_vecContainer.size(); i++)
	{
		for (size_t j = 0; j < m_vecContainer[i].vecMtrl.size(); j++)
		{
			CMaterial* pMtrl = new CMaterial;
			pMtrl->SetName(m_vecContainer[i].vecMtrl[j].strName);
			pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"3d_Deferred"));
			
			strKey = CPathMgr::GetInst()->GetFileName(m_vecContainer[i].vecMtrl[j].strDiff);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if(pTex != nullptr)
				pMtrl->SetParam(pTex,SHADER_PARAM::TEX_0);

			strKey = CPathMgr::GetInst()->GetFileName(m_vecContainer[i].vecMtrl[j].strNormal);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_1);

			strKey = CPathMgr::GetInst()->GetFileName(m_vecContainer[i].vecMtrl[j].strSpec);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_2);

			strKey = CPathMgr::GetInst()->GetFileName(m_vecContainer[i].vecMtrl[j].strAmb);
			pTex = CResMgr::GetInst()->Load<CTexture>(strKey);
			if (pTex != nullptr)
				pMtrl->SetParam(pTex, SHADER_PARAM::TEX_3); // 마스킹용 텍스쳐를 ambient에 넣어놓았다.

			CResMgr::GetInst()->AddResource<CMaterial>(pMtrl->GetName(), pMtrl);
		}
	}
}

int CFBXLoader::FindBoneIndex(string _strBoneName)
{
	wstring strName = wstring(_strBoneName.begin(), _strBoneName.end());
	for (size_t i = 0; i < m_vecBone.size(); i++)
	{
		if (m_vecBone[i]->strName == strName)
			return i;
	}

	assert(NULL);
	return 0;
}

FbxAMatrix CFBXLoader::GetTransForm(FbxNode * _pNode)
{
	FbxVector4 vTran = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 vRot = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 vScal = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vTran,vRot,vScal);
}

void CFBXLoader::CheckWeightAndIndices(FbxMesh * _pMesh, tContainer * _pContainer)
{
	vector<vector<tSkining>>::iterator iter = _pContainer->vecSkin.begin();

	int iVtxIdx = 0;
	for (; iter != _pContainer->vecSkin.end(); ++iter, ++iVtxIdx)
	{
		// 정점들 순회하면서 본에대한 정보를 가지고 있다면
		if (iter->size() > 0)
		{
			std::sort(iter->begin(), iter->end(), [](tSkining& _tFirst, tSkining& _tSecond) {return _tFirst.iWeight > _tSecond.iWeight; });

			if (iter->size() > 4)
				iter->erase(iter->begin() + 4, iter->end());

			double dWeight = 0.;
			for (size_t i = 0; i < iter->size(); i++)
				dWeight += ((*iter)[i]).iWeight;

			if (dWeight > 1.)
			{
				double revision = (1 - dWeight) / iter->size();
				for (size_t i = 0; i < iter->size(); i++)
					(*iter)[i].iWeight += revision;
			}
		}

		Vec4 vWeight = {};
		Vec4 vIndices = {};
		for (size_t i = 0; i < iter->size(); ++i)
		{
			vWeight[i] = (*iter)[i].iWeight;
			vIndices[i] = (*iter)[i].iBoneIdx;
		}


		memcpy(&_pContainer->vecWeight[iVtxIdx],&vWeight, sizeof(Vec4));
		memcpy(&_pContainer->vecIndices[iVtxIdx], &vIndices, sizeof(Vec4));
	}
}