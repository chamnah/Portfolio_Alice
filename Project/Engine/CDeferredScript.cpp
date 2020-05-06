#include "CDeferredScript.h"
#include "KeyMgr.h"
#include "CFilterCom.h"
#include "CMaterial.h"

CDeferredScript::CDeferredScript()
	:m_iSelect(6)
	, m_iMax(6)
{
}

CDeferredScript::~CDeferredScript()
{
}

int CDeferredScript::Update()
{
	if (KEYTAB(KEY_TYPE::KEY_F1))
	{
		m_iSelect++;
		if (m_iSelect > m_iMax)
			m_iSelect = 0;
	}

	Filter()->GetMaterial()->SetParam(&m_iSelect,SHADER_PARAM::INT_0);
	return 0;
}