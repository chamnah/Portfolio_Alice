#pragma once
#include "CComponent.h"
enum class POINT_DIR
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FRONT,
	BACK,
	END
};

enum class SHADOW_RT
{
	SHADOW_0,
	SHADOW_1,
	SHADOW_2,
	SHADOW_3,
	SHADOW_4,
	SHADOW_5,
	END
};

class CMRT;
class CRT;
class CLight :
	public CComponent
{
private:
	tLightInfo m_Info;
	CGameObj*  m_pFilter;
	CGameObj*  m_pCam;
	Vec3       m_vPointDir[(UINT)POINT_DIR::END];
	Vec3       m_vRotDir[(UINT)POINT_DIR::END];
	Mtx        m_mtxCamView[6];
	CTexture*  m_pShadowTex[6];
	CMRT*      m_pMRT;
	CRT*	   m_pShadowRT[(UINT)SHADOW_RT::END];
public:
	CLONE(CLight);

	virtual int Update();
	virtual int FinalUpdate();
	virtual void Render();
	void ShadowRender();

public:
	void SetDirection(Vec3& _vDir);
	Vec4& GetDirection() { return m_Info.vDir; }

	void SetLightColor(tLightColor& _Color) { m_Info.color = _Color; }
	tLightColor& GetLightColor() { return m_Info.color; }

	void SetDistance(float _fDist);
	float GetDistance() { return m_Info.fDist; }

	void SetLightType(int _iType);
	int  GetLightType() { return m_Info.iType; }

	void SetAngle(float _fAngle) { m_Info.fAngle = _fAngle; }
	float GetAngle() { return m_Info.fAngle; }

	CTexture* GetShadowTexture(SHADOW_RT _eRT);

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CLight();
	CLight(const CLight& _other);
	~CLight();
};