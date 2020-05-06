#pragma once
#include "CComponent.h"
class CTransForm :
	public CComponent
{
private:
	Vec3       m_vScale;
	Vec3	   m_vPos;
	Vec3       m_vOldPos;
	Vec3       m_vRot;
	Mtx		   m_mtxWorld;
	Mtx        m_mtxInvWorld;
	Mtx        m_mtxParent;
	Mtx        m_mtxOwner;
	Mtx        m_mtxRT;
	Mtx		   m_mtxMinusRT;
	Mtx		   m_mtxRot;
	Mtx        m_mtxView;
	Vec3       m_vAddPos;
	Vec3       m_vAddRot;

	Vec3       m_vLocalDir[(UINT)DIR_TYPE::END];
	Vec3       m_vWorldDir[(UINT)DIR_TYPE::END];

public:
	int Update();
	int LateUpdate();
	int FinalUpdate();
	void UpdateData();

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

	CLONE(CTransForm)

public:
	void SetPos(const Vec3& _vPos) { m_vPos = _vPos; }
	void SetPos(float _fPosX, float _fPosY, float _fPosZ) { m_vPos.x = _fPosX; m_vPos.y = _fPosY; m_vPos.z = _fPosZ; }
	void SetOnlyPos(const Vec3& _vPos) { m_vPos = _vPos; }
	const Vec3& GetPos() { return m_vPos; }
	void SetScale(const Vec3& _vScale) { m_vScale = _vScale; }
	void SetScale(float _fScaleX, float _fScaleY, float _fScaleZ) { m_vScale.x = _fScaleX; m_vScale.y = _fScaleY; m_vScale.z = _fScaleZ; }
	Vec3& GetScale() { return m_vScale; }
	void SetRotation(const Vec3& _vRot) { m_vRot = _vRot; }
	const Vec3& GetRotation() { return m_vRot; }
	void SetParentMtx(const Mtx& _mtxParent) { m_mtxParent = _mtxParent; }
	const Mtx& GetWorldMtx() { return m_mtxWorld; }
	const Mtx& GetRTMtx() { return m_mtxRT; }
	const Mtx& GetMinusRTMtx(){ return m_mtxMinusRT;}
	void SetAddPos(Vec3 _vPos) { m_vAddPos = _vPos; }
	Vec3& GetAddPos() { return m_vAddPos; }
	inline void SetInvParentPos(Vec3& _vWorldPos) { m_vPos = XMVector3TransformCoord(_vWorldPos, m_mtxParent.Invert()); }
	Mtx GetMtxWV() { return (m_mtxWorld * m_mtxView); }
	void SetMtxOwner(const Mtx& _mtxOwner) { m_mtxOwner = _mtxOwner; }
	Mtx& GetMtxOwner() { return m_mtxOwner; }
	Mtx& GetMtxRotation() { return m_mtxRot; }
	void SetAddRot(Vec3 _vRot) { m_vAddRot = _vRot; }

	Vec3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	Vec3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }

	Mtx& GetMtxInvWorld() { return m_mtxInvWorld; }
	void SetLook(Vec3 & _vDir);
	void SetLookAddRot(Vec3 & _vDir);
	
	void SetOldPos(const Vec3& _vPos) { m_vOldPos = _vPos; }
	Vec3& GetOldPos() { return m_vOldPos; }
	Vec3 GetWorldPos() { return XMVector3TransformCoord(Vec3::Zero, m_mtxWorld); }

	void RotationMatrix(const Mtx& _mtxRot);
	Vec4 RotationMtxToQuaternion();
public:
	CTransForm();
	virtual ~CTransForm();
};