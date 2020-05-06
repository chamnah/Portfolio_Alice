#pragma once
#include "CComponent.h"

struct tAnim3D
{
	int iStartFrm;
	int iEndFrm;
	int iFrmLength;
	wstring strKey;
};

class CAnimation3D;
class CAnimator3D :
	public CComponent
{
private:
	vector<tMtxAnimClip>* m_pVecAnimClip;
	vector<tMtxBone>*     m_pVecBone;
	vector<Mtx>          m_vecFinalBoneMtx;
	vector<Mtx>          m_vecWorldBoneMtx;
	vector<float>        m_vecClipTime;
	int					 m_iCurClip;
	float				 m_fCurTime;
	int                  m_iFrameCount;
	CTexture*            m_pBoneTex;
	map<wstring, tAnim3D*>   m_mapAnim3D;
	tAnim3D*             m_pCurAnim;
	map<wstring, tAnim3D*>::iterator m_NextAnim;
	bool					 m_bBegin;
	bool				 m_bRepeat;
	bool				 m_bFinish;
	int				     m_iAnimCenter;
	Vec3                 m_vCenterPos;
	bool			     m_bChange;
	tAnim3D*             m_pChangeAnim;
	int					 m_iFrmStart;
	vector<wstring>      m_vecKey;
	int					 m_iCurFrm;

public:
	CLONE(CAnimator3D);
	virtual int Update();
	void UpdateData();
	void AddAnimation(const wstring _strKey, int _iStartFrm, int _iEndFrm);
	bool AddAnimation(const wstring _strKey, int _iFrmLength);
	void DeleteAnimation(const wstring _strKey);
	void PlayAnim(wstring _strKey, bool _bRepeat = true, bool _bReset = false);
	void NextAnim();
	bool GetCurAnimKey(wstring _strKey);
	void ChangeText(wstring _strKey, wstring _strUpdate);
	void DeleteAll();
public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	void FileSave(const wstring& _strPath);
	void FileLoad(const wstring& _strPath);

public:
	void SetAnimClip(vector<tMtxAnimClip>* _pVecAnimClip);
	void SetBone(vector<tMtxBone>* _pVecBone);
	void SetBoneTex(CTexture* _pTex) { m_pBoneTex = _pTex; }
	CTexture* GetBoneTex() { return m_pBoneTex; }

	vector<Mtx>* GetWorldBoneMtx() { return  &m_vecWorldBoneMtx; }
	vector<Mtx>& GetFinalBoneMtx() { return  m_vecFinalBoneMtx; }
	Mtx* GetCenterBoneMtx() { return  &m_vecWorldBoneMtx[m_iAnimCenter]; }
	Vec3& GetCenterFrmPos() { return m_vCenterPos; }
	bool GetFinish() { return m_bFinish; }
	map<wstring, tAnim3D*>& GetMapAnim() { return m_mapAnim3D; }
	int GetCurFrm() { return m_iCurFrm; }
	tAnim3D* GetCurAnim() { return m_pCurAnim; }
	bool GetRepeat() { return m_bRepeat; }
public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _Anim);
	~CAnimator3D();
};