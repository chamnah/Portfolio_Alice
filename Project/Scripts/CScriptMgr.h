#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	CALICESCRIPT,
	CBODYSCRIPT,
	CBULLETSCRIPT,
	CBUTTONSCRIPT,
	CBUTTONUISCRIPT,
	CBUYSCRIPT,
	CCAMERASCRIPT,
	CCANNONBALLSCRIPT,
	CCARDSCRIPT,
	CCARSCRIPT,
	CCOLLISIONSCRIPT,
	CDIRLIGHTSCRIPT,
	CDISTORTIONSCRIPT,
	CDOORSCRIPT,
	CEFFECTSCRIPT,
	CEQUIPSCRIPT,
	CFADESCRIPT,
	CGAMECAMERASCRIPT,
	CGRAVITYSCRIPT,
	CGRINDERSCRIPT,
	CGRUNTMINISCRIPT,
	CGRUNTSCRIPT,
	CHPSCRIPT,
	CITEMSCRIPT,
	CJUMPPADSCRIPT,
	CMONSTERSPAWNSCRIPT,
	CMOUSESCRIPT,
	CMOVESCRIPT,
	CPARTICLESCRIPT,
	CPLAYERSCRIPT,
	CSCORESCRIPT,
	CSHRINKSCRIPT,
	CSKIRTSCRIPT,
	CTEAPOTCANNONSCRIPT,
	CTILEMGRSCRIPT,
	CTITLESCRIPT,
	CTRAILSCRIPT,
	CUISCRIPT,
	CUPGRADETEXTSCRIPT,
	CWKNIFESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
