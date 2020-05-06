#include "CRes.h"

static UINT g_id = 0;

CRes::CRes()
	:m_iID(g_id++)
{
}

CRes::~CRes()
{
}
