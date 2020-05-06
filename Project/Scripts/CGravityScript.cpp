#include "CGravityScript.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CTerrain.h"

CGravityScript::CGravityScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CGRAVITYSCRIPT);
}

CGravityScript::~CGravityScript()
{
}

int CGravityScript::Update()
{
	Vec3 vPos = TransForm()->GetPos();
	vPos.y -= DT;
	TransForm()->SetPos(vPos);
	return 0;
}

void CGravityScript::CollisionEnter(CCollider * _pColl)
{
	if (CSceneMgr::GetInst()->GetLayerName(_pColl->GetLayerID()) == L"Terrain")
	{
		_pColl->GetOwner()->GetTerrain();
	}
}