#pragma once
#include <CScript.h>

enum class ALICE_STATE
{
	NONE,
	WP_ONE,
	WP_TWO,
	WP_THREE,
	WP_FOUR
};

enum class MOVE_TYPE
{
	FRONT,
	BACK,
	RIGHT,
	LEFT
};

class CAliceScript :
	public CScript
{
private:
	float m_fSpeed;
	bool  m_bAction;
	wstring strAnim;
	int   m_iAttCombo;
	int   m_iAttComboMax;
	wstring m_strAtt;
	float m_fComboTime;
	float m_fCombatTime;
	ALICE_STATE m_eState;
	bool  m_bNextCombo;
	bool  m_bIdle;
	wstring m_strAttEnd;
	bool  m_bJump;
	float m_fJumpPower;
	int   m_iJumpCount;
	float m_fLanding;
	Vec3  m_vOld;
	bool  m_bColl;
	bool  m_bLanding;
	bool  m_bDash;
	MOVE_TYPE     m_eMove;
	Vec3  m_vMoveDir;
	float m_fMoveLength;
	CGameObj* m_pAim;
	bool  m_bUp;
	bool  m_bSmall;

private:
	CGameObj* m_pWP1;
	CTexture* m_pTexTrail;
	CGameObj* m_pSmokeObj;
public:
	CLONE(CAliceScript);
	virtual int Update();
	virtual void Start();
	virtual int FinalUpdate();

public:
	void Move();
	void MoveAnim();
	void WPOneAttckAnim();
	void WPTwoAttckAnim();
	void WPThreeAttckAnim();
public:
	void EquipOff();
	void EquipWPOne();
	void EquipWPTwo();
	void EquipWPThree();
	//void Gravity();
	void Sound();

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

public:
	CAliceScript();
	~CAliceScript();
};