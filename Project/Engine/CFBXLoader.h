#pragma once
#include "CParent.h"

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring strName;
	int     iDepth;
	int     iParent;
	FbxAMatrix mtxOffset;
	FbxAMatrix mtxTransForm;
	FbxNode*   pNode;
	vector<tKeyFrame> vecKeyFrame;
};

struct tAnimClip
{
	wstring strName;
	FbxTime tStartTime;
	FbxTime tEndTime;
	FbxLongLong llTimeLength;
	FbxTime::EMode eMode;
};

class CFBXLoader :
	public CParent
{
private:
	FbxManager*        m_pManager;
	FbxScene*          m_pScene;
	FbxImporter*       m_pImporter;
	vector<tContainer> m_vecContainer;
	vector<tBone*>     m_vecBone;
	vector<tAnimClip*>  m_vecAnimClip;
	FbxArray<FbxString*> m_arrAnimName;
	FbxNode*             m_pSocket;


public:
	void Init();
	void LoadFBX(const wstring _strPath);
	void LoadSkeleton(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightAndIndices(FbxCluster* _pCluster,int _iBoneIdx,tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster,FbxAMatrix& _mtxTransForm, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void Triangulate(FbxNode* _pNode);
	void LoadMeshDataFromNode(FbxNode* _pNode);
	void LoadMesh(FbxMesh* _pMesh);
	void LoadTangent(FbxMesh* _pMesh,tContainer& _pContainer,int _iIdx,int _iVtxOrder);
	void LoadNormal(FbxMesh* _pMesh, tContainer& _pContainer, int _iIdx, int _iVtxOrder);
	void LoadBinormal(FbxMesh* _pMesh, tContainer& _pContainer, int _iIdx, int _iVtxOrder);
	void LoadUV(FbxMesh* _pMesh, tContainer& _pContainer, int _iIdx, int _iUVIndex);
	void LoadMaterial(FbxSurfaceMaterial* _pMtrl);
	Vec4 LoadMaterialData(FbxSurfaceMaterial* _pMtrl,const char* _strMtrlName,const char* _strFactorName);
	wstring LoadMtrlTextureName(FbxSurfaceMaterial* _pMtrl, const char* _strMtrlName);
	void CreateTexture();
	void CreaetMaterial();

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransForm(FbxNode* _pNode);

	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer * _pContainer);

public:
	vector<tContainer>& GetContainer() { return m_vecContainer; }
	vector<tBone*>&     GetBone() { return m_vecBone; }
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }
public:
	CFBXLoader();
	virtual ~CFBXLoader();
};