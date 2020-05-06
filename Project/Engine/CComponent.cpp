#include "CComponent.h"

COMPONENT_TYPE CComponent::GetType()
{
	if (m_eType == COMPONENT_TYPE::END)
		assert(false);
	return m_eType; 
}

CComponent::CComponent()
	:m_eType(COMPONENT_TYPE::END)
{
}

CComponent::CComponent(const CComponent & _Com)
	: m_eType(_Com.m_eType)
	, m_pOwnerObj(nullptr)
{
}

CComponent::~CComponent()
{
}

void CComponent::Save(FILE* _pFile)
{
	fwrite(&m_eType, sizeof(UINT), 1, _pFile); 
}