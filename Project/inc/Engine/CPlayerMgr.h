#pragma once
class CAliceScript;
class CHPScript;
class CGameObj;
class CPlayerMgr
{
private:
	CGameObj* m_pPlayer;
	CAliceScript* m_pPlayerScript;
	CHPScript* m_pHP_UI;
	Vec3      m_vDir[(UINT)DIR_TYPE::END];
	Vec3      m_vWorldDir[(UINT)DIR_TYPE::END];
	Vec3      m_vAnimRot;
	int		  m_iHP;
	bool	  m_bUIReset;
	Vec3      m_vMoveDir;
	UINT      m_iTooth;
	wchar_t   m_strTooth[10];
	bool      m_bMenu;
	CGameObj* m_pMenu;
	UINT	  m_iWeaponUp[(UINT)WEAPON_TYPE::END];
	Vec2      m_vPreResol;
	bool	  m_bEvent;
	bool      m_bDoor;
	Vec2     m_vOriSize;
	WEAPON_TYPE m_eWeapon;
	bool      m_bTooth;
	wstring   m_strPrice;
	bool      m_bSmall;
	bool      m_bJumpPad;
public:
	SINGLETON(CPlayerMgr);
public:
	void Init();
	void Update();

public:
	void SetPlayer(CGameObj* _pObj) { m_pPlayer = _pObj; }
	CGameObj* GetPlayer() { return m_pPlayer; }
	void SetHP_UI(CHPScript* _pObj) { m_pHP_UI = _pObj; }
	CHPScript* GetHP_UI() { return m_pHP_UI; }
	void SetPlayerScript(CAliceScript* _pObj) { m_pPlayerScript = _pObj; }
	CAliceScript* GetPlayerScript() { return m_pPlayerScript; }
	void SetHP(int _iHP) { m_iHP = _iHP; }
	int GetHP() { return m_iHP; }
	void SubHP(int _iSub) { m_iHP -= _iSub; m_bUIReset = true;}
	void SetAnimRot(Vec3& _vRot) { m_vAnimRot = _vRot; }
	const Vec3& GetAnimRot() { return m_vAnimRot; }
	void SetDir(DIR_TYPE _eType, Vec3& _vDir) { m_vWorldDir[(UINT)_eType] = _vDir; }
	const Vec3& GetDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }
	void SetHP_UI_Reset(bool _bReset) { m_bUIReset = _bReset; }
	bool GetHP_UI_Reset() { return m_bUIReset; }
	void SetMoveDir(Vec3& _vDir) { m_vMoveDir = _vDir; }
	Vec3& GetMoveDir() { return m_vMoveDir; }
	void SetTooth(int _iTooth) { m_iTooth = _iTooth; }
	void AddTooth(int _iTooth) { m_iTooth += _iTooth; }
	void SubTooth(int _iTooth) { m_iTooth -= _iTooth; }
	int  GetTooth() { return m_iTooth; }
	void UIPos(CGameObj* _pChild);
	void SetEvent(bool _bEvent) { m_bEvent = _bEvent; }
	bool GetEvent() { return m_bEvent; }
	bool GetDoor() { return m_bDoor; }
	WEAPON_TYPE GetWeaponType() { return m_eWeapon; }
	void SetWeaponType(WEAPON_TYPE _eType) { m_eWeapon = _eType; }
	void AddWeaponUp(UINT _iUp, WEAPON_TYPE _eType) { m_iWeaponUp[(UINT)_eType] += _iUp; }
	UINT GetWeaponUp(WEAPON_TYPE _eType) { return m_iWeaponUp[(UINT)_eType]; }
	bool GetMenu() { return m_bMenu; }
	void SetPrice(const wstring& _strPrice) { m_strPrice = _strPrice; }
	wstring& GetPrice() { return m_strPrice; }
	void SetSmall(bool _bSmall) { m_bSmall = _bSmall; }
	bool GetSmall() { return m_bSmall; }
	void SetJumpPad(bool _bJump) { m_bJumpPad = _bJump; }
	bool GetJumpPad() { return m_bJumpPad; }
};