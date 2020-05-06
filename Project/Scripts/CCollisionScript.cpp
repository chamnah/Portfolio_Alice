#include "CCollisionScript.h"
#include "CScriptMgr.h"
#include "CCollider.h"
#include "CTransForm.h"
	
CCollisionScript::CCollisionScript()
{		
	SetScriptType((UINT)SCRIPT_TYPE::CCOLLISIONSCRIPT);
}		
		
CCollisionScript::~CCollisionScript()
{		
}		
		
void CCollisionScript::CollisionEnter(CCollider * _pColl)
{		
	assert(_pColl);
	if (_pColl->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{	
		if (_pColl->GetLand())
		{
			CGameObj* pObj = GetOwner()->GetAncestor();
			Vec3 vPos = pObj->GetTransForm()->GetPos();
		
			CGameObj* pLand = _pColl->GetOwner();
			Vec3 vLandPos = pLand->GetTransForm()->GetPos();
			vLandPos.y += _pColl->GetPos().y + _pColl->GetLandHeight();
			
			if(vPos.y < vLandPos.y)
			{
				vPos.y = vLandPos.y;
				pObj->GetTransForm()->SetPos(vPos);
			}
		}
	}	
	else
	{	
		
	}	
}		
		
void CCollisionScript::Collision(CCollider * _pColl)
{		
	CollisionEnter(_pColl);
}		
		
void CCollisionScript::CollisionExit(CCollider * _pColl)
{		
}				