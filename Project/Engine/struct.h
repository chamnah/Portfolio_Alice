#pragma once

#include "define.h"

struct tResolution
{
	float x, y;

	tResolution()
		:x(0.f),y(0.f)
	{}
	tResolution(float _fX,float _fY)
		:x(_fX),y(_fY)
	{}
};

struct TranformMatrix
{
	Mtx     mtxWorld;
	Mtx     mtxView;
	Mtx     mtxProj;
	Mtx     mtxWV;
	Mtx     mtxVP;
	Mtx     mtxWVP;
	Mtx     mtxInvWorld;
};

// 정점 구조체
struct VTX
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2    vTex;

	Vec3    vNormal;
	Vec3    vTangent;
	Vec3    vBinormal;
	Vec4    vWeight;
	Vec4    vIndices;

	float   fAge;
	UINT    iType;

}; // 총 80byte 16 * 5

struct InstData
{
	Mtx     mtxWorld;
	Mtx     mtxWV;
	Mtx     mtxVP;
	Mtx     mtxWVP;
	int     iRowIdx;
	Vec2    vUV;
};

struct IDX
{
	UINT first, second, third;
};

struct ShaderParam
{
	int		arrInt[4];
	float	arrFloat[4];
	Vec4    arrMtrl;
	Vec2    arrVec2[4];
	Vec4    arrVec4[4];
	Mtx     arrMtx[7];
	int     arrBool[12];
	int     arrRBool[4];
};

struct tAnimParam
{
	Vec2 g_fLT;
	Vec2 g_fSize;
	int   g_iUse;
	Vec2 g_vOffset;
	int g_iPadding[1];
};

struct tEvent
{
	EVENT_TYPE eType;
	INT_PTR    pDesc;
	INT_PTR    pSrc;
};

struct TileInfo
{
	POINT ptPos;
	int   iDir;
};

struct EventObj
{
	Vec3 vPos;
	Vec3 vRot;
	wstring wcsName;

	EventObj()
	{}
	EventObj(Vec3 _vPos,Vec3 _vRot, wstring _wcsName)
		:vPos(_vPos),vRot(_vRot), wcsName(_wcsName)
	{}
};

struct TextInfo
{
	wstring wcsText;
	Vec2    vLeftTop;
	Vec2    vRightBottom;
};

struct TURNTILE
{
	Vec3 vPos;
	int  iIdx;
};

struct tLightColor
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
};

struct tLightInfo
{
	tLightColor color;
	Vec4 vPos;
	Vec4 vDir;
	float fDist;
	int iType;
	float fAngle;
	int  iPadding;
};

struct tGlobal
{
	float fDT;
	float fAccTime;
	int   iCount;
	float fWidth;
	float fHeight;
	int   iPadding[3];
};

struct tTarget
{
	int iTarget;
	int iPadding[3];
};

struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

struct tMtrl
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmiv;
};

struct tFbxMaterial
{
	tMtrl mtrl;
	wstring strName;
	wstring strDiff;
	wstring strSpec;
	wstring strAmb;
	wstring strEmiv;
	wstring strNormal;
};

struct tSkining
{
	int iBoneIdx; // 정점이 영향을 받는 Boneid
	double iWeight;  // 영향을 받는 정도

};

struct tContainer
{
	wstring strName;
	vector<Vec3> vecVtxPos;
	vector<Vec3> vecVtxTangent;
	vector<Vec3> vecVtxNormal;
	vector<Vec3> vecVtxBinormal;
	vector<Vec2> vecVtxUV;
	vector<Vec4> vecWeight;
	vector<Vec4> vecIndices;

	vector<vector<UINT>> vecIdx;
	vector<tFbxMaterial> vecMtrl;

	bool bAnimaion;
	vector<vector<tSkining>> vecSkin;

	void Resize(UINT _iSize)
	{
		vecVtxPos.resize(_iSize);
		vecVtxTangent.resize(_iSize);
		vecVtxNormal.resize(_iSize);
		vecVtxBinormal.resize(_iSize);
		vecVtxUV.resize(_iSize);
		vecWeight.resize(_iSize);
		vecIndices.resize(_iSize);
		vecSkin.resize(_iSize);
	}
};

struct tMtxKeyFrm
{
	float fTime;
	int   iFrm;
	Vec3  vTranslate;
	Vec3  vScale;
	Vec4  vQRot;
};

struct tMtxBone
{
	wstring strName;
	int     iDepth;
	int     iParent;
	Mtx     OffsetMtx;
	Mtx     TransForm;
	vector<tMtxKeyFrm> vecKeyFrm;
};

struct tMtxAnimClip
{
	wstring strName;
	double dStartTime;
	double dEndTime;
	double dTimeLength;
	int    iStartFrm;
	int    iEndFrm;
	int    iFrmLength;
	FbxTime::EMode eMode;
};