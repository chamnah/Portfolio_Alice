#pragma once
#include "CParent.h"

class CTexture;
class CGameObj;

struct tAnim
{
	CTexture* pTex;
	Vec2 vLT;
	Vec2 vSize;
	Vec2 vOffset;
	float fDuration;
};

class CAnimation2D :
	public CParent
{
private:
	vector<tAnim> m_vecAnim;
	CGameObj*     m_pOwner;
	float         m_fAccTime;
	UINT	      m_iCurFrm;
	bool		  m_bFinish;
	wstring       m_wcsPath;
	bool          m_bAutoSize;

public:
	static CAnimation2D* Create(CTexture* _pTex, const Vec2& _vLT, const Vec2& _vSize, UINT iMaxFrm, const wstring& _wcsKey);
	static CAnimation2D* Create(const wstring& _wcsFolderPath, const wstring& _wcsKey, const Vec2& _vOffset = {}, float _fDuration = 0.05f);

	void SetOwner(CGameObj* _pOwner) { m_pOwner = _pOwner; }
	void SetDurationTime(float _fDuration);
	UINT GetFrame() { return m_iCurFrm; }
	void SetAutoSize(bool _bSize) { m_bAutoSize = _bSize; }

	wstring& GetPath() { return m_wcsPath; }

	int Update();
	void UpdateData();
	void SetMoveFrm(UINT _iFrm);
	bool GetFinish() { return m_bFinish; }
	void SetCurFrmCutSize(Vec4& _vSize);

	vector<tAnim>& GetAllFrame() { return m_vecAnim; }
	tAnim& GetCurFrame() { return m_vecAnim[m_iCurFrm]; }
public:
	CAnimation2D();
	~CAnimation2D();
};