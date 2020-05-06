#pragma once
#include <CScript.h>
class CEffectScript :
	public CScript
{
private:
	float m_fDeathTime;
	float m_fAccTime;
	bool  m_bDeath;
	EFFECT_TYPE m_eType;
	vector<Vec3> m_vecDir;


public:
	virtual int Update();
	CLONE(CEffectScript);

public:
	void SetTime(float _fTime) { m_fDeathTime = _fTime; m_fAccTime = m_fDeathTime; }
	void SetEffectType(EFFECT_TYPE _eType) { m_eType = _eType; }
	void SetFirework(int _iHoriz, int _iVertical);
public:
	CEffectScript();
	~CEffectScript();
};
