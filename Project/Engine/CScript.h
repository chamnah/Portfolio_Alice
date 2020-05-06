#pragma once
#include "CComponent.h"

struct ValueInfo
{
	void*   pValue;
	string strType;
	string strName;

	ValueInfo() {}
	ValueInfo(void* _pVal, string _strType, string _strName) { pValue = _pVal; strType = _strType ; strName = _strName; }
};

struct BoolInfo
{
	bool bVal;
	wstring strName;
};

struct IntInfo
{
	int iVal;
	wstring strName;
};

class CPrefab;
class CCollider;
class CScript :
	public CComponent
{
private:
	UINT m_iScriptType;
protected:
	vector<ValueInfo> m_vecVal;
	vector<BoolInfo> m_vecBool;
	vector<IntInfo> m_vecInt;
public:
	void Instantiate(CPrefab* _pPrefab, const wstring& _wcsKey, const Vec3& _vPos);
	virtual void Save(FILE* _pFile) {}
	virtual void Load(FILE* _pFile) {}

	virtual void CollisionEnter(CCollider* _pColl) {}
	virtual void Collision(CCollider* _pColl) {}
	virtual void CollisionExit(CCollider* _pColl) {}

	UINT GetScriptType() { return m_iScriptType; }
	void SetScriptType(UINT iType);
	vector<ValueInfo>& GetVecInfo() { return m_vecVal; }
	vector<BoolInfo>& GetVecBool() { return m_vecBool; }
	vector<IntInfo>& GetVecInt() { return m_vecInt; }
protected:
	wstring AnimAngleToNum(int _iDir, float _fAngle, int* _pInt = nullptr);
	wstring AnimAngleNoRot(int _iDir, float _fAngle, int* _pInt = nullptr);
	float   GetAngleDegree(Vec3& _vDest, Vec3& _vSrc);
	float   GetAngleRadian(Vec3& _vDest, Vec3& _vSrc);
	Vec3    GetDirVec3(Vec3 _vStart, Vec3 _vEnd);
	float   GetDistanceVec3(Vec3 _vStart, Vec3 _vEnd);

public:
	CScript();
	virtual ~CScript();
};