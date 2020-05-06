#include "CSkirtScript.h"
#include "CPlayerMgr.h"
#include "CAnimator3D.h"
#include "CTransForm.h"
#include "CScriptMgr.h"
#include "CMeshRender.h"
#include "CMesh.h"
CSkirtScript::CSkirtScript()
	:m_iBoneIndex(0)
	, m_bOn(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CSKIRTSCRIPT);
}

CSkirtScript::~CSkirtScript()
{
}

int CSkirtScript::Update()
{
	CGameObj* pPlayer = CPlayerMgr::GetInst()->GetPlayer();

	if (pPlayer == nullptr)
		return 0;
	if (m_bOn == false)
	{
		m_bOn = true;
		vector<tMtxBone>* vecBone = pPlayer->GetMeshRender()->GetMesh()->GetBone();
		for (size_t i = 0; i < vecBone->size(); i++)
		{
			wstring strName = vecBone->at(i).strName;
			//Bip01-R-Finger4
			if (strName == L"Bip01-Pelvis")
				m_iBoneIndex = i;
		}

		/*Vec3 vRot;
		vRot.x = XMConvertToRadians(180.f);
		vRot.y = XMConvertToRadians(260.f);
		TransForm()->SetRotation(vRot);*/
		/*Vec3 vPos = TransForm()->GetPos();
		vPos.z = 3.f;
		TransForm()->SetPos(vPos);*/
	}
	else
	{
		Mtx mtxBone = pPlayer->GetAnimator3D()->GetFinalBoneMtx()[m_iBoneIndex];
		TransForm()->SetMtxOwner(mtxBone);
	
		GetOwner()->SetParent(pPlayer);
	}

	/*Mtx* pBone = GetParentOfOwner()->GetAnimator3D()->GetCenterBoneMtx();
	TransForm()->SetMtxOwner(*pBone);*/

	return 0;
}	