#pragma once
#include <CScript.h>
class CGizmoScript :
	public CScript
{
private:
	DIR_TYPE m_eDirType;
	Vec3     m_vDir[3];
	Vec3     m_vOldMousePos;
	Vec3     m_vCurMousePos;

public:
	CLONE(CGizmoScript);

	int Update();

public:
	void SetDirType(DIR_TYPE _eType) { m_eDirType = _eType; }

public:
	CGizmoScript();
	virtual ~CGizmoScript();
};