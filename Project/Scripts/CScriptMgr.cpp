#include "stdafx.h"
#include "CScriptMgr.h"

#include "CAliceScript.h"
#include "CBodyScript.h"
#include "CBulletScript.h"
#include "CButtonScript.h"
#include "CButtonUIScript.h"
#include "CBuyScript.h"
#include "CCameraScript.h"
#include "CCannonBallScript.h"
#include "CCardScript.h"
#include "CCarScript.h"
#include "CCollisionScript.h"
#include "CDirLightScript.h"
#include "CDistortionScript.h"
#include "CDoorScript.h"
#include "CEffectScript.h"
#include "CEquipScript.h"
#include "CFadeScript.h"
#include "CGameCameraScript.h"
#include "CGravityScript.h"
#include "CGrinderScript.h"
#include "CGruntMiniScript.h"
#include "CGruntScript.h"
#include "CHPScript.h"
#include "CItemScript.h"
#include "CJumpPadScript.h"
#include "CMonsterSpawnScript.h"
#include "CMouseScript.h"
#include "CMoveScript.h"
#include "CParticleScript.h"
#include "CPlayerScript.h"
#include "CScoreScript.h"
#include "CShrinkScript.h"
#include "CSkirtScript.h"
#include "CTeapotCannonScript.h"
#include "CTileMgrScript.h"
#include "CTitleScript.h"
#include "CTrailScript.h"
#include "CUIScript.h"
#include "CUpgradeTextScript.h"
#include "CWKnifeScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAliceScript");
	_vec.push_back(L"CBodyScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CButtonUIScript");
	_vec.push_back(L"CBuyScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CCannonBallScript");
	_vec.push_back(L"CCardScript");
	_vec.push_back(L"CCarScript");
	_vec.push_back(L"CCollisionScript");
	_vec.push_back(L"CDirLightScript");
	_vec.push_back(L"CDistortionScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CEquipScript");
	_vec.push_back(L"CFadeScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGrinderScript");
	_vec.push_back(L"CGruntMiniScript");
	_vec.push_back(L"CGruntScript");
	_vec.push_back(L"CHPScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CJumpPadScript");
	_vec.push_back(L"CMonsterSpawnScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CMoveScript");
	_vec.push_back(L"CParticleScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CScoreScript");
	_vec.push_back(L"CShrinkScript");
	_vec.push_back(L"CSkirtScript");
	_vec.push_back(L"CTeapotCannonScript");
	_vec.push_back(L"CTileMgrScript");
	_vec.push_back(L"CTitleScript");
	_vec.push_back(L"CTrailScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CUpgradeTextScript");
	_vec.push_back(L"CWKnifeScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAliceScript" == _strScriptName)
		return new CAliceScript;
	if (L"CBodyScript" == _strScriptName)
		return new CBodyScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CButtonUIScript" == _strScriptName)
		return new CButtonUIScript;
	if (L"CBuyScript" == _strScriptName)
		return new CBuyScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CCannonBallScript" == _strScriptName)
		return new CCannonBallScript;
	if (L"CCardScript" == _strScriptName)
		return new CCardScript;
	if (L"CCarScript" == _strScriptName)
		return new CCarScript;
	if (L"CCollisionScript" == _strScriptName)
		return new CCollisionScript;
	if (L"CDirLightScript" == _strScriptName)
		return new CDirLightScript;
	if (L"CDistortionScript" == _strScriptName)
		return new CDistortionScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CEquipScript" == _strScriptName)
		return new CEquipScript;
	if (L"CFadeScript" == _strScriptName)
		return new CFadeScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGrinderScript" == _strScriptName)
		return new CGrinderScript;
	if (L"CGruntMiniScript" == _strScriptName)
		return new CGruntMiniScript;
	if (L"CGruntScript" == _strScriptName)
		return new CGruntScript;
	if (L"CHPScript" == _strScriptName)
		return new CHPScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CJumpPadScript" == _strScriptName)
		return new CJumpPadScript;
	if (L"CMonsterSpawnScript" == _strScriptName)
		return new CMonsterSpawnScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CMoveScript" == _strScriptName)
		return new CMoveScript;
	if (L"CParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CScoreScript" == _strScriptName)
		return new CScoreScript;
	if (L"CShrinkScript" == _strScriptName)
		return new CShrinkScript;
	if (L"CSkirtScript" == _strScriptName)
		return new CSkirtScript;
	if (L"CTeapotCannonScript" == _strScriptName)
		return new CTeapotCannonScript;
	if (L"CTileMgrScript" == _strScriptName)
		return new CTileMgrScript;
	if (L"CTitleScript" == _strScriptName)
		return new CTitleScript;
	if (L"CTrailScript" == _strScriptName)
		return new CTrailScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	if (L"CUpgradeTextScript" == _strScriptName)
		return new CUpgradeTextScript;
	if (L"CWKnifeScript" == _strScriptName)
		return new CWKnifeScript;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CALICESCRIPT:
		return L"CAliceScript";
		break;

	case SCRIPT_TYPE::CBODYSCRIPT:
		return L"CBodyScript";
		break;

	case SCRIPT_TYPE::CBULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::CBUTTONSCRIPT:
		return L"CButtonScript";
		break;

	case SCRIPT_TYPE::CBUTTONUISCRIPT:
		return L"CButtonUIScript";
		break;

	case SCRIPT_TYPE::CBUYSCRIPT:
		return L"CBuyScript";
		break;

	case SCRIPT_TYPE::CCAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::CCANNONBALLSCRIPT:
		return L"CCannonBallScript";
		break;

	case SCRIPT_TYPE::CCARDSCRIPT:
		return L"CCardScript";
		break;

	case SCRIPT_TYPE::CCARSCRIPT:
		return L"CCarScript";
		break;

	case SCRIPT_TYPE::CCOLLISIONSCRIPT:
		return L"CCollisionScript";
		break;

	case SCRIPT_TYPE::CDIRLIGHTSCRIPT:
		return L"CDirLightScript";
		break;

	case SCRIPT_TYPE::CDISTORTIONSCRIPT:
		return L"CDistortionScript";
		break;

	case SCRIPT_TYPE::CDOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::CEFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::CEQUIPSCRIPT:
		return L"CEquipScript";
		break;

	case SCRIPT_TYPE::CFADESCRIPT:
		return L"CFadeScript";
		break;

	case SCRIPT_TYPE::CGAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::CGRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::CGRINDERSCRIPT:
		return L"CGrinderScript";
		break;

	case SCRIPT_TYPE::CGRUNTMINISCRIPT:
		return L"CGruntMiniScript";
		break;

	case SCRIPT_TYPE::CGRUNTSCRIPT:
		return L"CGruntScript";
		break;

	case SCRIPT_TYPE::CHPSCRIPT:
		return L"CHPScript";
		break;

	case SCRIPT_TYPE::CITEMSCRIPT:
		return L"CItemScript";
		break;

	case SCRIPT_TYPE::CJUMPPADSCRIPT:
		return L"CJumpPadScript";
		break;

	case SCRIPT_TYPE::CMONSTERSPAWNSCRIPT:
		return L"CMonsterSpawnScript";
		break;

	case SCRIPT_TYPE::CMOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::CMOVESCRIPT:
		return L"CMoveScript";
		break;

	case SCRIPT_TYPE::CPARTICLESCRIPT:
		return L"CParticleScript";
		break;

	case SCRIPT_TYPE::CPLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::CSCORESCRIPT:
		return L"CScoreScript";
		break;

	case SCRIPT_TYPE::CSHRINKSCRIPT:
		return L"CShrinkScript";
		break;

	case SCRIPT_TYPE::CSKIRTSCRIPT:
		return L"CSkirtScript";
		break;

	case SCRIPT_TYPE::CTEAPOTCANNONSCRIPT:
		return L"CTeapotCannonScript";
		break;

	case SCRIPT_TYPE::CTILEMGRSCRIPT:
		return L"CTileMgrScript";
		break;

	case SCRIPT_TYPE::CTITLESCRIPT:
		return L"CTitleScript";
		break;

	case SCRIPT_TYPE::CTRAILSCRIPT:
		return L"CTrailScript";
		break;

	case SCRIPT_TYPE::CUISCRIPT:
		return L"CUIScript";
		break;

	case SCRIPT_TYPE::CUPGRADETEXTSCRIPT:
		return L"CUpgradeTextScript";
		break;

	case SCRIPT_TYPE::CWKNIFESCRIPT:
		return L"CWKnifeScript";
		break;

	}
	return nullptr;
}