#include "CScript.h"
#include "CPrefab.h"
#include "CGameObj.h"
#include "CTransForm.h"
#include "CSceneMgr.h"

void CScript::SetScriptType(UINT iType)
{
	m_iScriptType = iType;
}

wstring CScript::AnimAngleToNum(int _iDir, float _fAngle,int* _pInt)
{
	float fGap = 360.f / (float)_iDir;
	wstring wcsNum = L"";

	if (_fAngle < fGap / 2.f || _fAngle >= 360.f - (fGap / 2.f))
	{
		TransForm()->SetRotation(Vec3(0.f, 0.f, 0.f));
		wcsNum = L"00";
		return wcsNum;
	}

	int iGap = int(fGap * 1000);
	float fAngle = _fAngle * 1000.f;

	int iAngle = int(fAngle) - iGap / 2;
	int iAnimNum = int(iAngle) / iGap;
	iAnimNum += 1;

	int iDir = _iDir / 4;

	if (iAnimNum > iDir && iDir * 3 > iAnimNum) // 좌우 반전을 위한 계산 , 이 영역에 안들어가면 계산 안한다.
	{
		if (iDir * 2 >= iAnimNum) // 16 -> 4 보다 크면 32 -> 8 보다 크면
		{
			iAnimNum = iDir * 2 - iAnimNum;
		}
		else
		{
			iAnimNum = ((iDir * 3 - iAnimNum) * 2) + iAnimNum;
		}
		TransForm()->SetRotation(Vec3(0.f, XM_PI, 0.f));
	}
	else
		TransForm()->SetRotation(Vec3(0.f, 0.f, 0.f));

	if (_pInt != nullptr)
		*_pInt = iAnimNum;

	wchar_t szNum[50] = L"";
	_itow_s(iAnimNum, szNum, 10);

	wcsNum = szNum;
	if (iAnimNum < 10)
		wcsNum = L"0" + wcsNum;

	return wcsNum;
}

wstring CScript::AnimAngleNoRot(int _iDir, float _fAngle, int * _pInt)
{
	float fGap = 360.f / (float)_iDir;
	wstring wcsNum = L"";

	if (_fAngle < fGap / 2.f || _fAngle >= 360.f - (fGap / 2.f))
	{
		TransForm()->SetRotation(Vec3(0.f, 0.f, 0.f));
		wcsNum = L"00";
		return wcsNum;
	}

	int iGap = int(fGap * 1000);
	float fAngle = _fAngle * 1000.f;

	int iAngle = int(fAngle) - iGap / 2;
	int iAnimNum = int(iAngle) / iGap;
	iAnimNum += 1;

	if (_pInt != nullptr)
		*_pInt = iAnimNum;

	wchar_t szNum[50] = L"";
	_itow_s(iAnimNum, szNum, 10);

	wcsNum = szNum;
	if (iAnimNum < 10)
		wcsNum = L"0" + wcsNum;

	return wcsNum;
}


float CScript::GetAngleDegree(Vec3 & _vDest, Vec3 & _vSrc)
{

	float fX = _vSrc.x - _vDest.x;
	float fY = _vSrc.y - _vDest.y;

	float fAngle = atan2f(fY, fX);
	fAngle = RadianToDegree(fAngle);
	fAngle += 180.f;

	return fAngle;
}

float CScript::GetAngleRadian(Vec3 & _vDest, Vec3 & _vSrc)
{

	float fX = _vSrc.x - _vDest.x;
	float fY = _vSrc.y - _vDest.y;

	float fAngle = atan2f(fY, fX);
	fAngle += XM_PI;
	return fAngle;
}

Vec3 CScript::GetDirVec3(Vec3 _vStart, Vec3 _vEnd)
{
	_vStart.z = 0.f;
	_vEnd.z = 0.f;
	Vec3 vDir = _vEnd - _vStart;
	vDir.Normalize();
	return vDir;
}

float CScript::GetDistanceVec3(Vec3 _vStart, Vec3 _vEnd)
{
	_vStart.z = 0.f;
	_vEnd.z = 0.f;
	
	return Vec3::Distance(_vStart, _vEnd);;
}

CScript::CScript()
{
	m_eType = COMPONENT_TYPE::SCRIPT;
}

CScript::~CScript()
{
}

void CScript::Instantiate(CPrefab* _pPrefab, const wstring& _wcsKey, const Vec3& _vPos)
{
	CGameObj* pObj = _pPrefab->Instantiate();

 	static Vec3 vPos = _vPos;

	vPos.x += 10;

	pObj->GetTransForm()->SetPos(vPos);

	CSceneMgr::GetInst()->AddObject(_wcsKey,pObj);
}