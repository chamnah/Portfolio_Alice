#pragma once
#include "CComponent.h"
class CAnimation2D;
class CAnimator2D :
	public CComponent
{
private:
	map<wstring, CAnimation2D*> m_mapAnim;
	CAnimation2D*				m_pCurAnim;
	wstring						m_wcsTemp;
	bool						m_bRepeat;
	bool						m_bPlay;
	bool						m_bReset;
	wstring						m_wcsPath;

public:
	virtual int Update();
	void UpdateData();

public:
	void AddAnimation(CTexture* _pTex, const Vec2& _vLT, const Vec2& _vSize, UINT iMaxFrm, const wstring& _wcsKey);
	CAnimation2D* AddAnimation(const wstring& _wcsPath, const wstring& _wcsKey, const Vec2& _vOffset = {}, float _fDuration = 0.02f);
	void RemoveAnimation(const wstring& _wcsKey);
	void PlayAnim(const wstring& _wcsKey, bool _bRepeat);
	void ClearData();
	bool GetAnimFinish();
	const wstring& GetCurKey();
	void AnimReset();
	void IsPlay(bool _bPlay) { m_bPlay = _bPlay; }
	void ChangeAnimation(CAnimation2D* _pAnim) { m_pCurAnim = _pAnim; }
	void SetOffset(Vec2& _vOffset);
	Vec2 GetOffset();
	void SetDurationTime(float _fDuration);
	void ClearAnimation();
	void SetReset(bool _bReset) { m_bReset = _bReset; }
	bool IsReset() { return m_bReset; }
	void SetPath(const wstring& _wcsPath) { m_wcsPath = _wcsPath; }
	UINT GetCurFrame();
	void SetMoveFrm(UINT _iFrm);
	CTexture* GetCurAnimTexture();
	void SetAnimSize(float _fSize); // X만 사이즈 변경되게끔 설계했음
	void SetAutoSize(bool _bSize);
	bool GetPlay() { return m_bPlay; }
	void SetCurAnim(CAnimation2D* _pAnim) { m_pCurAnim = _pAnim; }
	void SetCurFrmCutSize(Vec4& _vSize);

public:
	void AnimSetOwner();

	map<wstring, CAnimation2D*>& GetAnimation() { return m_mapAnim; }

	CLONE(CAnimator2D);

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CAnimator2D();
	CAnimator2D(const CAnimator2D& _Anim);
	virtual ~CAnimator2D();
};