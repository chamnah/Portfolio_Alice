#include "CButtonUIScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "CCollisionMgr.h"
#include "CScriptMgr.h"

CButtonUIScript::CButtonUIScript()
	:m_eColl(BUTTON_COLL::BOX)
{
	m_vRectColl[0] = Vec3(-0.5f,0.5f,0.f);
	m_vRectColl[1] = Vec3(0.5f, -0.5f, 0.f);
	SetScriptType((UINT)SCRIPT_TYPE::CBUTTONUISCRIPT);
}

CButtonUIScript::~CButtonUIScript()
{
}

int CButtonUIScript::Update()
{
	if (MouseCollision())
	{
		if(KEYTAB(KEY_TYPE::KEY_LBTN))
		{
			int i = 0;
		}
	}
	return 0;
}

bool CButtonUIScript::MouseCollision()
{
	Vec3 vMousePos = CKeyMgr::GetInst()->GetDXMousePos();
	static Vec3 vRect[2];
	static Vec3 vCenter;

	vRect[0] = XMVector3TransformCoord(m_vRectColl[0], TransForm()->GetMtxWV());
	vRect[1] = XMVector3TransformCoord(m_vRectColl[1], TransForm()->GetMtxWV());

	vCenter = XMVector3TransformCoord(Vec3::Zero, TransForm()->GetMtxWV());

	if (vRect[0].x <= vMousePos.x && vMousePos.x <= vRect[1].x &&
		vRect[1].y <= vMousePos.y && vMousePos.y <= vRect[0].y) // �簢�� ������ �浹�� �̷�� ���ٸ�
	{
		if (m_eColl == BUTTON_COLL::HEXAGON) // ���� ��ư�̾��ٸ�
		{
			float fHalfUp = ((vRect[0].y - vCenter.y) * 0.5f) + vCenter.y;
			float fHalfDown = ((vRect[1].y - vCenter.y) * 0.5f) + vCenter.y;

			Vec2 vStart;
			Vec2 vEnd;
			Vec2 vTargetPos;

			if (vMousePos.y > fHalfUp) // �� �簢�����̿� ���� ���� ���ٸ�
			{
				if (vCenter.x < vMousePos.x) // ���콺�� �����ʿ� �־��ٸ�
				{
					vStart = Vec2(vCenter.x, vRect[0].y);
					vEnd = Vec2(vRect[1].x, fHalfUp);
					vTargetPos = Vec2(vCenter.x, vRect[0].y);
				}
				else // �����̳� ��� �ִٸ�
				{
					vStart = Vec2(vRect[0].x, fHalfUp);
					vEnd = Vec2(vCenter.x, vRect[0].y);
					vTargetPos = Vec2(vCenter.x, vRect[0].y);
				}
				
				float fLinear = LinearEquation(vStart, vEnd, vMousePos.x, vTargetPos);

				if (fLinear < vMousePos.y)
					return false;
				else
					return true;

			}
			else if (vMousePos.y < fHalfDown) // ���� ���� �Ʒ��� �ִٸ�
			{
				if (vCenter.x < vMousePos.x) // ���콺�� �����ʿ� �־��ٸ�
				{
					vStart = Vec2(vCenter.x, vRect[1].y);
					vEnd = Vec2(vRect[1].x, fHalfDown);
					vTargetPos = Vec2(vCenter.x, vRect[1].y);
				}
				else // �����̳� ��� �ִٸ�
				{
					vStart = Vec2(vRect[0].x, fHalfDown);
					vEnd = Vec2(vCenter.x, vRect[1].y);
					vTargetPos = Vec2(vCenter.x, vRect[1].y);
				}

				float fLinear = LinearEquation(vStart, vEnd, vMousePos.x, vTargetPos);

				if (fLinear > vMousePos.y)
					return false;
				else
					return true;
			}
			else
			{
				return true;
			}
		} // �Ϲ� ��ư�̾��ٸ� ����� �簢�� ��ư�� �־ else�� �ϸ� �þ�� �� �߰��ؾߵ�
		else
			return true;
	}

	return false;
}