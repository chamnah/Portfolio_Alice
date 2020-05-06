#include "CEquipScript.h"
#include "CAnimator3D.h"
#include "CMeshRender.h"
#include "CMesh.h"
#include "CTransForm.h"
#include "CScriptMgr.h"

CEquipScript::CEquipScript()
	:m_iBoneIdx(-1)
	, m_pParentObj(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CEQUIPSCRIPT);
}

CEquipScript::~CEquipScript()
{
}

void CEquipScript::Start()
{
}

int CEquipScript::Update()
{
	if (m_iBoneIdx != -1)
		TransForm()->SetMtxOwner(m_pParentObj->GetAnimator3D()->GetWorldBoneMtx()->at(m_iBoneIdx));

	return 0;
}

void CEquipScript::SetEquipBone(const wstring& _strBone,CGameObj* _pParent)
{
	m_pParentObj = _pParent;
	vector<tMtxBone>* vecBone = m_pParentObj->GetMeshRender()->GetMesh()->GetBone();
	for (size_t i = 0; i < vecBone->size(); i++)
	{
		wstring strName = vecBone->at(i).strName;
		if (strName == _strBone)
			m_iBoneIdx = i;
	}
}