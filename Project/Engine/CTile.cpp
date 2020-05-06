#include "CTile.h"
#include "CMaterial.h"
#include "CMeshRender.h"

CTile::CTile()
	:m_eColl(COLL_TYPE::NOCOLL)
	, m_bClose(false)
	, m_bScale(false)
{
	m_eType = COMPONENT_TYPE::TILE;
}

CTile::~CTile()
{
}

void CTile::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);
	UINT i = (UINT)m_eColl;
	fwrite(&i, sizeof(UINT), 1, _pFile);
}

void CTile::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	UINT i = 0;
	fread(&i, sizeof(UINT), 1, _pFile);
	m_eColl = (COLL_TYPE)i;

	if (MeshRender() != nullptr)
		MeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);
}