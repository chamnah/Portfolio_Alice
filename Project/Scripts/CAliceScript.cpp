#include "CAliceScript.h"
#include "CScriptMgr.h"
#include "CGameObj.h"
#include "CAnimator3D.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CPlayerMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CWKnifeScript.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CMeshRender.h"
#include "CTrailScript.h"
#include "CRenderMgr.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CEffectScript.h"
#include "CPrefab.h"
#include "CParticleScript.h"
#include "CEquipScript.h"
#include "CLayer.h"
#include "CCamera.h"
#include "CFadeScript.h"

CAliceScript::CAliceScript()
	:m_fSpeed(100.f)
	, m_bAction(false)
	, m_iAttCombo(-1)
	, m_iAttComboMax(4)
	, m_fComboTime(0.f)
	, m_fCombatTime(0.f)
	, m_bNextCombo(true)
	, m_bIdle(true)
	, m_pTexTrail(nullptr)
	, m_bJump(false)
	, m_fJumpPower(200.f)
	, m_iJumpCount(0)
	, m_fLanding(0.f)
	, m_bColl(false)
	, m_bLanding(false)
	, m_bDash(false)
	, m_fMoveLength(0.f)
	, m_pSmokeObj(nullptr)
	, m_pWP1(nullptr)
	, m_pAim(nullptr)
	, m_bUp(false)
	, m_bSmall(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CALICESCRIPT);

	m_strAtt = L"c_attack";
	m_eState = ALICE_STATE::NONE;

	m_pTexTrail = CResMgr::GetInst()->Load<CTexture>(L"T_FX_VB_Trail_BGLight");

	if (m_pTexTrail == nullptr)
	{
		wstring strPath = CPathMgr::GetInst()->GetPath();
		strPath += L"Texture\\T_FX_VB_Trail_BGLight.tga";

		m_pTexTrail = CTexture::CreateTexture(strPath);
		CResMgr::GetInst()->AddResource<CTexture>(L"T_FX_VB_Trail_BGLight", m_pTexTrail);
	}

	/*CGameObj* pObj = GetOwner();
	CAnimator3D* pAnim = GetOwner()->GetAnimator3D();
	GetOwner()->GetAnimator3D()->AddAnimation(L"Run", 138, 162);*/
}

CAliceScript::~CAliceScript()
{
}

int CAliceScript::Update()
{
	if (GetOwner()->GetDead())
		return 0;

	TransForm()->SetOldPos(TransForm()->GetPos());
	Sound();

	if (TransForm()->GetPos().y < -1500)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Default");
		vector<CGameObj*>& vecObj = pLayer->GetGameObj();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetName() == L"spawn")
			{
				TransForm()->SetPos(vecObj[i]->GetTransForm()->GetPos());
				return 0;
			}
		}
	}

	if (m_bAction)
	{
		if (KEYTAB(KEY_TYPE::KEY_LBTN) && m_iAttCombo < m_iAttComboMax)
			m_bNextCombo = true;

		bool bFinish = Animator3D()->GetFinish();
		if (bFinish)
		{
			if (m_eState == ALICE_STATE::WP_THREE)
			{
				m_bAction = false;
				m_bNextCombo = false;
				return 0;
			}

			if(m_bNextCombo)
				WPOneAttckAnim();
			else
			{
				m_fComboTime += DT;
				if (m_fComboTime > 0.3f)
				{
					wchar_t szCombo[3];
					_itow_s(m_iAttCombo, szCombo, 10);
					wstring strKey = m_strAtt + szCombo;
					strKey += L"_end";
					GetOwner()->GetAnimator3D()->PlayAnim(strKey, false);
					m_iAttCombo = -1;
					m_bAction = false;
					m_strAttEnd = strKey;
				}
			}
		}

		if (KEYAWAY(KEY_TYPE::KEY_LBTN))
		{
			if (m_eState == ALICE_STATE::WP_TWO)
			{
				GetOwner()->GetAnimator3D()->PlayAnim(L"wp3_Idle", true);
				m_bAction = false;
				m_pWP1->GetAnimator3D()->PlayAnim(L"idle_pose", true);
			}
		}

		return 0;
	}
	else
	{
		if (m_eState != ALICE_STATE::NONE)
		{
			if (!Animator3D()->GetCurAnimKey(m_strAttEnd) || Animator3D()->GetFinish())
				m_bIdle = true;
			
			m_fCombatTime += DT;

			if (m_fCombatTime > 3.f)
			{
				m_fCombatTime = 0.f;
				EquipOff();
			}
		}

		if (!m_bJump)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN) && m_iAttCombo < m_iAttComboMax)
			{
				if(m_eState == ALICE_STATE::WP_ONE)
					WPOneAttckAnim();
				else if (m_eState == ALICE_STATE::WP_TWO)
					WPTwoAttckAnim();
				else if (m_eState == ALICE_STATE::WP_THREE)
					WPThreeAttckAnim();
				return 0;
			}
			else if (KEYTAB(KEY_TYPE::KEY_1))
			{
				EquipWPOne();
				m_eState = ALICE_STATE::WP_ONE;
			}
			else if (KEYTAB(KEY_TYPE::KEY_2))
			{
				EquipWPTwo();
				m_eState = ALICE_STATE::WP_TWO;
			}
			else if (KEYTAB(KEY_TYPE::KEY_3))
			{
				EquipWPThree();
				m_eState = ALICE_STATE::WP_THREE;
			}
			MoveAnim();
		}

		Vec3 vPos; 

		if (m_bDash)
		{
			vPos = TransForm()->GetPos();
			vPos += m_vMoveDir * 300.f * DT;
			m_fMoveLength += 300.f * DT;

			if (m_fMoveLength >= 300.f)
			{
				m_fMoveLength = 0.f;
				m_bDash = false;
				MeshRender()->SetView(true);
				MeshRender()->SetShadow(true);
				vector<CGameObj*>& vecChild = GetOwner()->GetChild();
				for (size_t i = 0; i < vecChild.size(); i++)
				{
					vecChild[i]->GetMeshRender()->SetView(true);
					vecChild[i]->GetMeshRender()->SetShadow(true);
				}
				CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pSmokeObj,NULL });


			}
			TransForm()->SetPos(vPos);
		}
		else
		{
			Move();

			if (m_bJump)
			{
				m_fJumpPower -= 200 * DT;
				vPos = TransForm()->GetPos();
				vPos.y += m_fJumpPower * DT;
				TransForm()->SetPos(vPos);
				wstring strType;
				if (m_iJumpCount > 0)
					strType = L"l_";

				if (m_fJumpPower <= 0)
				{
					Animator3D()->PlayAnim(strType + L"jump_fall", false);
					m_bUp = false;
				}
				else
				{
					if (Animator3D()->GetCurAnimKey(strType + L"jump_start") && Animator3D()->GetFinish())
						Animator3D()->PlayAnim(strType + L"jump_rise", false);
				}

				/*if (vPos.y <= m_fLanding)
				{
					m_bLanding = true;
					Animator3D()->PlayAnim(strType + L"jump_land", false);
					vPos.y = m_fLanding;
					m_bJump = false;
				}*/

				if (KEYTAB(KEY_TYPE::KEY_SPACE))
				{
					CResMgr::GetInst()->SoundPlay(L"sfx_alice_jump_double01", 1);
					CGameObj* pObj = CResMgr::GetInst()->Load<CPrefab>(L"Tonado")->CreateObjClone(L"Effect");
					pObj->GetMeshRender()->SetMtrl(pObj->GetMeshRender()->GetMtrl()->Clone());
					CEffectScript* pEffect = new CEffectScript;
					pEffect->SetTime(2.f);
					pEffect->SetEffectType(EFFECT_TYPE::FADEOUT);
					pObj->AddComponent(pEffect);
					pObj->GetTransForm()->SetRotation(TransForm()->GetRotation());
					Vec3 vPos = TransForm()->GetPos();
					vPos.y += 100.f;
					pObj->GetTransForm()->SetPos(vPos);
					m_bJump = true;
					m_fJumpPower = 150.f;
					Animator3D()->PlayAnim(L"l_jump_start", false);
					Vec4 vColor = {1.f,0.f,1.f,1.f};
					pObj->GetMeshRender()->SetParam(&vColor, SHADER_PARAM::MTRL);
				}
			}
			else
			{
				m_iJumpCount = 0;
				if (KEYTAB(KEY_TYPE::KEY_SPACE))
				{
					CResMgr::GetInst()->SoundPlay(L"sfx_alice_jump01",1);
					m_iJumpCount++;
					m_bJump = true;
					m_fJumpPower = 300.f;
					Animator3D()->PlayAnim(L"jump_start", false);
					m_bUp = true;
				}
				else if (CPlayerMgr::GetInst()->GetJumpPad())
				{
					CPlayerMgr::GetInst()->SetJumpPad(false);
					m_iJumpCount++;
					m_bJump = true;
					m_fJumpPower = 1200.f;
					Animator3D()->PlayAnim(L"jumppad_raise", false);
					m_bUp = true;
					CGameObj* pFade = new CGameObj;
					pFade->SetName(L"Fade");
					pFade->GetTransForm()->SetPos(Vec3(0.f, 0.f, 1.f));
					pFade->GetTransForm()->SetScale(Vec3(g_Global.fWidth, g_Global.fHeight, 1.f));
					pFade->AddComponent(new CMeshRender);
					pFade->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
					pFade->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"Fade"));
					pFade->GetMeshRender()->SetRenderType(RENDER_TYPE::FORWARD);
					Vec4 vColor = Vec4(0.f,0.f,0.f,0.f);
					pFade->GetMeshRender()->SetParam(&vColor,SHADER_PARAM::MTRL);
					pFade->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));
					pFade->AddComponent(new CFadeScript);
					CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)CSceneMgr::GetInst()->GetLayerIndex(L"UI"),(INT_PTR)pFade });

					vector<CGameObj*>& vecMenu = CSceneMgr::GetInst()->GetLayer(L"Menu")->GetGameObj();
					for (size_t i = 0; i < vecMenu.size(); i++)
					{
						CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)vecMenu[i],NULL });
					}
					CRenderMgr::GetInst()->SetText(false);

					vPos = TransForm()->GetPos();
					vPos.y += m_fJumpPower * DT;
					TransForm()->SetPos(vPos);
				}
			}
		}
	}

	m_fLanding = 0.f;
	return 0;
}

void CAliceScript::Start()
{
	//Animator3D()->AddAnimation(L"Idle", 0, 150);
	//Animator3D()->AddAnimation(L"walk", 151, 183);
	//Animator3D()->AddAnimation(L"run", 184, 206); 
	//Animator3D()->AddAnimation(L"w_Idle", 207, 357); 
	//Animator3D()->AddAnimation(L"w_walk", 358, 390); 
	//Animator3D()->AddAnimation(L"w_run", 391, 413); 
	//Animator3D()->AddAnimation(L"c_Idle", 414, 454); 
	//Animator3D()->AddAnimation(L"c_attack0", 455, 483); 
	//Animator3D()->AddAnimation(L"c_attack0_end", 484,501); 
	//Animator3D()->AddAnimation(L"c_attack1", 502, 532); 
	//Animator3D()->AddAnimation(L"c_attack1_end", 533, 548); 
	//Animator3D()->AddAnimation(L"c_attack2", 549, 577);
	//Animator3D()->AddAnimation(L"c_attack2_end", 578, 584);
	//Animator3D()->AddAnimation(L"c_attack3", 585, 618); 
	//Animator3D()->AddAnimation(L"c_attack3_end", 619, 629); 
	//Animator3D()->AddAnimation(L"c_attack4", 630, 662); 
	//Animator3D()->AddAnimation(L"c_attack4_end", 663, 673); // 674บฮลอ
	Animator3D()->PlayAnim(L"Idle", true);

	/*CMeshData* pData = CResMgr::GetInst()->Load<CMeshData>(L"SK_VorpalBlade_Lv04");
	if (nullptr == pData)
	{
		wstring strContent = CPathMgr::GetInst()->GetPath();
		strContent += L"Mesh\\SK_VorpalBlade_Lv04.fbx";
		pData = CResMgr::GetInst()->LoadFBX(strContent);
	}
	m_pWP1 = pData->instantiate();
	m_pWP1->SetName(L"Knife");
	m_pWP1->AddComponent(new CWKnifeScript);
	GetOwner()->AddChild(m_pWP1);*/
	//m_pWP1->SetShow(false);
	/*Animator3D()->AddAnimation(L"Idle", 0, 80);
	Animator3D()->AddAnimation(L"Jump_Fall", 81, 101);
	Animator3D()->AddAnimation(L"Jump_Land", 102, 105);
	Animator3D()->AddAnimation(L"Jump_Run_Fall", 106, 120);
	Animator3D()->AddAnimation(L"Jump_Run_Land", 121, 126);
	Animator3D()->AddAnimation(L"Jump_Run_Start", 127, 131);
	Animator3D()->AddAnimation(L"Jump_Start", 132, 137);
	Animator3D()->AddAnimation(L"Run", 138, 162);
	Animator3D()->AddAnimation(L"RunBreak_L", 163, 167);
	Animator3D()->AddAnimation(L"RunBreak_R", 168, 172);
	Animator3D()->AddAnimation(L"Turn90Left_NoRotation", 173, 194);
	Animator3D()->AddAnimation(L"Turn90Right_NoRotation", 195, 214);
	Animator3D()->AddAnimation(L"Walk", 215, 246);
	Animator3D()->AddAnimation(L"beGrab_Begin", 247, 285);
	Animator3D()->AddAnimation(L"beGrab_Interrupted", 286, 322);
	Animator3D()->AddAnimation(L"beGrab_Loop", 323, 383);
	Animator3D()->AddAnimation(L"Combat_Attack01", 384, 396);
	Animator3D()->AddAnimation(L"Combat_Attack01_F", 397, 416);
	Animator3D()->AddAnimation(L"Combat_Attack02", 417, 434);
	Animator3D()->AddAnimation(L"Combat_Attack02_F", 435, 454);
	Animator3D()->AddAnimation(L"Combat_Attack03", 455, 499);
	Animator3D()->AddAnimation(L"Combat_Backward", 500, 520);
	Animator3D()->AddAnimation(L"Combat_Forward", 521, 539);
	Animator3D()->AddAnimation(L"Combat_Idle", 540, 600);
	Animator3D()->AddAnimation(L"Combat_StrafLeft", 601, 621);
	Animator3D()->AddAnimation(L"Combat_StrafRight", 622, 642);
	Animator3D()->AddAnimation(L"DollBoy_LP_Attack", 643, 868);*/

	//GetOwner()->GetAnimator3D()->PlayAnim(L"run", true);
}

int CAliceScript::FinalUpdate()
{
	return 0;
}

void CAliceScript::Move()
{
	Vec3 vFrontDir = CPlayerMgr::GetInst()->GetDir(DIR_TYPE::FRONT);
	Vec3 vRightDir = CPlayerMgr::GetInst()->GetDir(DIR_TYPE::RIGHT);

	Vec3 vPos = TransForm()->GetPos();
	m_fSpeed = 200.f;
	m_eMove = MOVE_TYPE::FRONT;
	if (KEYHOLD(KEY_TYPE::KEY_W))
	{
		vPos += vFrontDir * m_fSpeed * DT;
		Vec3 vRot;
		vRot.y = CPlayerMgr::GetInst()->GetAnimRot().y + XM_PI;
		TransForm()->SetRotation(vRot);
		m_vMoveDir = vFrontDir;
		TransForm()->SetPos(vPos);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_S))
	{
		m_vMoveDir = -vFrontDir;
		vPos -= vFrontDir * m_fSpeed * DT;
		Vec3 vRot;
		vRot.y = CPlayerMgr::GetInst()->GetAnimRot().y;
		TransForm()->SetRotation(vRot);
		TransForm()->SetPos(vPos);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_A))
	{
		m_vMoveDir = -vRightDir;
		vPos -= vRightDir * m_fSpeed * DT;
		Vec3 vRot;
		vRot.y = CPlayerMgr::GetInst()->GetAnimRot().y + XM_PIDIV2;
		TransForm()->SetRotation(vRot);
		TransForm()->SetPos(vPos);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_D))
	{
		m_vMoveDir = vRightDir;
		vPos += vRightDir * m_fSpeed * DT;
		Vec3 vRot;
		vRot.y = CPlayerMgr::GetInst()->GetAnimRot().y - XM_PIDIV2;
		TransForm()->SetRotation(vRot);
		TransForm()->SetPos(vPos);
	}
	
	if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
	{
		m_bDash = true;
		MeshRender()->SetView(false);
		MeshRender()->SetShadow(false);
	 	vector<CGameObj*>& vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); i++)
		{
			vecChild[i]->GetMeshRender()->SetView(false);
			vecChild[i]->GetMeshRender()->SetShadow(false);
		}
		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"MoveParticle");
		m_pSmokeObj = pPrefab->Instantiate();
		m_pSmokeObj->GetMeshRender()->SetMtrl(m_pSmokeObj->GetMeshRender()->GetMtrl()->Clone());
		m_pSmokeObj->GetMeshRender()->GetMtrl()->SetRasterizeState(RASTERIZE_TYPE::CULL_NONE);
		m_pSmokeObj->GetTransForm()->SetPos(Vec3(0.f,100.f,0.f));
		CParticleScript* pParticle = new CParticleScript;
		m_pSmokeObj->AddComponent(pParticle);
		pParticle->SetCount(1);
		GetOwner()->AddChild(m_pSmokeObj);
	}

	CPlayerMgr::GetInst()->SetMoveDir(m_vMoveDir);
}

void CAliceScript::MoveAnim()
{
	if (m_eState == ALICE_STATE::NONE)
		strAnim = L"";
	else if(m_eState == ALICE_STATE::WP_ONE)
		strAnim = L"w_";
	else if (m_eState == ALICE_STATE::WP_TWO)
		strAnim = L"wp3_";
	else if (m_eState == ALICE_STATE::WP_THREE)
		strAnim = L"wp4_";

	if (KEYHOLD(KEY_TYPE::KEY_W))
	{
		strAnim += L"run";
		Animator3D()->PlayAnim(strAnim, true);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_S))
	{
		strAnim += L"run";
		Animator3D()->PlayAnim(strAnim, true);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_A))
	{
		strAnim += L"run";
		Animator3D()->PlayAnim(strAnim, true);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_D))
	{
		strAnim += L"run";
		Animator3D()->PlayAnim(strAnim, true);
	}
	else if (Animator3D()->GetFinish() || m_bIdle)
	{
		strAnim += L"Idle";
		Animator3D()->PlayAnim(strAnim, true);
	}
	
	static Vec3 vSmall = Collider()->GetScale();
	static Vec3 vPos = Collider()->GetPos();
	static float fAccTime = 0.f;
	if (KEYHOLD(KEY_TYPE::KEY_CTRL))
	{
		fAccTime += DT * 2;
		if (fAccTime >= 1.f)
			fAccTime = 1.f;
		float fScale = Lerf(1.f, 0.4f, fAccTime);
		TransForm()->SetScale(Vec3(fScale, fScale, fScale));
		for (size_t i = 0; i < GetOwner()->GetChild().size(); i++)
			GetOwner()->GetChild()[i]->GetTransForm()->SetScale(Vec3(fScale, fScale, fScale));
		CPlayerMgr::GetInst()->SetSmall(true);

		Collider()->SetRadius(vSmall.x * 0.4f);
		Collider()->SetScale(vSmall * 0.4f);
		Collider()->SetPos(vPos * 0.4f);

		CCamera* pCam = CRenderMgr::GetInst()->GetMainCam();
		Vec3 vCam = pCam->TransForm()->GetPos() - (pCam->TransForm()->GetPos() * fScale);
		pCam->TransForm()->SetAddPos(-vCam);
	}
	else if (KEYAWAY(KEY_TYPE::KEY_CTRL))
	{
		fAccTime = 0.f;
		TransForm()->SetScale(Vec3(1.f, 1.f, 1.f));
		for (size_t i = 0; i < GetOwner()->GetChild().size(); i++)
			GetOwner()->GetChild()[i]->GetTransForm()->SetScale(Vec3(1.f, 1.f, 1.f));
		CPlayerMgr::GetInst()->SetSmall(false);
		
		Collider()->SetRadius(vSmall.x);
		Collider()->SetScale(vSmall);
		Collider()->SetPos(vPos);
		CCamera* pCam = CRenderMgr::GetInst()->GetMainCam();
		pCam->TransForm()->SetAddPos(Vec3(0.f,0.f,0.f));
	}
}

void CAliceScript::WPOneAttckAnim()
{
	if (m_eState == ALICE_STATE::NONE)
		EquipWPOne();

	m_eState = ALICE_STATE::WP_ONE;
	m_bNextCombo = false;
	m_fComboTime = 0.f;
	m_iAttCombo++;
	wchar_t szCombo[3];
	_itow_s(m_iAttCombo, szCombo, 10);

	GetOwner()->GetAnimator3D()->PlayAnim(m_strAtt + szCombo, false);
	m_bAction = true;
	m_bIdle = false;
}

void CAliceScript::WPTwoAttckAnim()
{
	GetOwner()->GetAnimator3D()->PlayAnim(L"wp3_attack", true);
	m_pWP1->GetAnimator3D()->PlayAnim(L"fire", true);
	m_bAction = true;
	m_bIdle = false;
}

void CAliceScript::WPThreeAttckAnim()
{
	GetOwner()->GetAnimator3D()->PlayAnim(L"wp4_attack", false);
	m_pWP1->GetAnimator3D()->PlayAnim(L"fire", false);
	m_bAction = true;
	m_bIdle = false;
}

void CAliceScript::EquipOff()
{
	//m_pWP1->SetShow(false);
	m_eState = ALICE_STATE::NONE;
	m_bIdle = true;
	if (m_pWP1 != nullptr)
	{
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pWP1 });
		m_pWP1 = nullptr;

		if (m_pAim != nullptr)
		{
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pAim });
			m_pAim = nullptr;
		}
	}
}

void CAliceScript::EquipWPOne()
{
	EquipOff();

	CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"Sword");
	m_pWP1 = pPrefab->Instantiate();
	m_pWP1->SetTeamType(TEAM_TYPE::PLAYER);
	GetOwner()->AddChild(m_pWP1);
	((CWKnifeScript*)m_pWP1->GetScript()[0])->SetColl();
	
	CGameObj* pTrail = new CGameObj;
	pTrail->AddComponent(new CMeshRender);
	pTrail->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"PointMesh"));
	pTrail->GetMeshRender()->SetMtrl(CResMgr::GetInst()->Load<CMaterial>(L"Trail"));
	pTrail->GetMeshRender()->GetMtrl()->SetRasterizeState(RASTERIZE_TYPE::CULL_NONE);
	pTrail->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ONEONE));
	pTrail->GetMeshRender()->SetParam(m_pTexTrail,SHADER_PARAM::TEX_1);
	pTrail->AddComponent(new CTrailScript);
	pTrail->GetMeshRender()->SetRenderType(RENDER_TYPE::FORWARD);
	m_pWP1->AddChild(pTrail);

	UINT iUp = CPlayerMgr::GetInst()->GetWeaponUp(WEAPON_TYPE::VOPAL);

	if (iUp == 0)
		pTrail->GetMeshRender()->SetParam(&Vec4(0.f,0.f,1.f,1.f),SHADER_PARAM::MTRL);
	else if(iUp == 1)
		pTrail->GetMeshRender()->SetParam(&Vec4(1.f, 0.7f, 0.f, 1.f), SHADER_PARAM::MTRL);

	//m_pWP1->SetShow(true);

	//m_pWP1->AddChild(pTrail);

	//CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)CSceneMgr::GetInst()->GetLayerIndex(L"Default"),(INT_PTR)m_pWP1 });
}

void CAliceScript::EquipWPTwo()
{
	EquipOff();

	CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"WP_Grinder");
	m_pWP1 = pPrefab->Instantiate();
	m_pWP1->SetTeamType(TEAM_TYPE::PLAYER);
	GetOwner()->AddChild(m_pWP1);

	CEquipScript* pEquip = new CEquipScript;
	m_pWP1->AddComponent(pEquip);
	pEquip->SetEquipBone(L"Bip01-Prop1",GetOwner());
	m_pWP1->GetAnimator3D()->PlayAnim(L"idle_pose", true);

	m_pAim = CResMgr::GetInst()->Load<CPrefab>(L"Aim")->CreateObjClone(L"UI");
}

void CAliceScript::EquipWPThree()
{
	EquipOff();

	CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(L"TeapotCannonA");
	m_pWP1 = pPrefab->Instantiate();
	m_pWP1->SetTeamType(TEAM_TYPE::PLAYER);
	GetOwner()->AddChild(m_pWP1);

	CEquipScript* pEquip = new CEquipScript;
	m_pWP1->AddComponent(pEquip);
	pEquip->SetEquipBone(L"Bip01-Prop1", GetOwner());
	m_pWP1->GetAnimator3D()->PlayAnim(L"Idle", true);
}

void CAliceScript::Sound()
{
	if (Animator3D()->GetCurAnimKey(L"c_attack0") && Animator3D()->GetCurFrm() >= 8 && Animator3D()->GetCurFrm() <= 12)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash01", 1,false);
	else if (Animator3D()->GetCurAnimKey(L"c_attack1") && Animator3D()->GetCurFrm() >= 10 && Animator3D()->GetCurFrm() <= 14)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash02", 1, false);
	else if (Animator3D()->GetCurAnimKey(L"c_attack2") && Animator3D()->GetCurFrm() >= 10 && Animator3D()->GetCurFrm() <= 14)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash03", 1, false);
	else if (Animator3D()->GetCurAnimKey(L"c_attack2") && Animator3D()->GetCurFrm() >= 20 && Animator3D()->GetCurFrm() <= 24)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash04", 1, false);
	else if (Animator3D()->GetCurAnimKey(L"c_attack3") && Animator3D()->GetCurFrm() >= 10 && Animator3D()->GetCurFrm() <= 14)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash05", 1, false);
	else if (Animator3D()->GetCurAnimKey(L"c_attack4") && Animator3D()->GetCurFrm() >= 10 && Animator3D()->GetCurFrm() <= 14)
		CResMgr::GetInst()->SoundPlay(L"sfx_vorpal_slash06", 1, false);
}

void CAliceScript::CollisionEnter(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Land" && _pColl->GetColliderType() != COLLIDER_TYPE::COLL_BOX &&
		_pColl->GetColliderType() != COLLIDER_TYPE::COLL_RECT)
	{
		return;
	}

	if (_pColl->GetOwner()->GetTeamType() == TEAM_TYPE::MONSTER)
	{
		CPlayerMgr::GetInst()->SubHP(2);
		return;
	}

	Vec3 vOld = TransForm()->GetOldPos();
	Vec3 vPos = TransForm()->GetPos();
	m_vOld = vOld;
	
	m_bColl = true;
	if (_pColl->GetOwner()->GetLayerName() == L"Land")
	{
		if (_pColl->GetColliderType() == COLLIDER_TYPE::COLL_RECT)
		{
			static Vec3 arrVTX[2] = { Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f, 0.5f, 0.f) };
			Vec3 vStart = XMVector3TransformCoord(arrVTX[0], _pColl->GetWorldMtx());
			Vec3 vEnd = XMVector3TransformCoord(arrVTX[1], _pColl->GetWorldMtx());
			vPos.y = LandHeight(vStart,vEnd, vPos.z);
			TransForm()->SetPos(vPos);
			m_bLanding = true;
			m_bJump = false;
			return;
		}

		float fOtherCollY = _pColl->GetWorldPos().y;
		float fCollScaleY = _pColl->GetLandHeight();
		float fCollY = Collider()->GetPos().y;

		if (m_bUp)
			return;

		if (vOld.y + fCollY >= fOtherCollY + (fCollScaleY * 0.5f))
		{
			m_fLanding = _pColl->GetWorldPos().y + (fCollScaleY * 0.5f);
			vPos.y = m_fLanding;
			TransForm()->SetPos(vPos);
			m_bLanding = true;
			m_bJump = false;
		}
		else
		{
			vOld.y = vPos.y;
			TransForm()->SetPos(vOld);
		}
	}
	else if (_pColl->GetOwner()->GetLayerName() == L"Default")
	{
		vOld.y = vPos.y;
		TransForm()->SetOnlyPos(vOld);

		TransForm()->FinalUpdate();
		Collider()->MatrixUpdate();

		vector<CGameObj*>& vecChild = GetOwner()->GetChild();

		for (size_t i = 0; i < vecChild.size(); i++)
		{
			vecChild[i]->GetTransForm()->FinalUpdate();
		}
	}
}

void CAliceScript::Collision(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Land" && _pColl->GetColliderType() != COLLIDER_TYPE::COLL_BOX &&
		_pColl->GetColliderType() != COLLIDER_TYPE::COLL_RECT)
	{
		return;
	}

	m_bLanding = false;
	Vec3 vOld = m_vOld;
	Vec3 vPos = TransForm()->GetPos();

	if (_pColl->GetOwner()->GetLayerName() == L"Land")
	{
		if (_pColl->GetColliderType() == COLLIDER_TYPE::COLL_RECT)
		{
			static Vec3 arrVTX[2] = { Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f, 0.5f, 0.f) };
			Vec3 vStart = XMVector3TransformCoord(arrVTX[0], _pColl->GetWorldMtx());
			Vec3 vEnd = XMVector3TransformCoord(arrVTX[1], _pColl->GetWorldMtx());
			vPos.y = LandHeight(vStart, vEnd, vPos.z);
			TransForm()->SetPos(vPos);
			m_bLanding = true;
			m_bJump = false;
			return;
		}

		if (!_pColl->GetOwner()->GetChild().empty())
		{
			CCollider* pColl = _pColl->GetOwner()->GetChild()[0]->GetCollider();
			if (pColl != nullptr)
			{
				if (pColl->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
				{
					if (!CCollisionMgr::GetInst()->IsCircleColl(pColl, Collider()))
					{
						//m_fJumpPower = 0.f;
						m_bJump = true;
						return;
					}
				}
			}
		}

		if (m_bUp)
			return;

		if (vOld.y + Collider()->GetPos().y >= _pColl->GetWorldPos().y + (_pColl->GetLandHeight() * 0.5f))
		{
			m_fLanding = _pColl->GetWorldPos().y + (_pColl->GetLandHeight() * 0.5f);
			vPos.y = m_fLanding;
			TransForm()->SetPos(vPos);
			m_bLanding = true;
		}
		else
		{
			Vec3 vDir = Collider()->GetWorldPos() - _pColl->GetWorldPos();
			vDir.Normalize();
			vOld += vDir * 10 * DT;
			vOld.y = vPos.y;
			TransForm()->SetOnlyPos(vOld);
			m_vOld = vOld;
		}
	}
	else if(_pColl->GetOwner()->GetLayerName() == L"Default")
	{
		vOld.y = vPos.y;
		TransForm()->SetOnlyPos(vOld);
		TransForm()->FinalUpdate();
		Collider()->MatrixUpdate();
		
		vector<CGameObj*>& vecChild = GetOwner()->GetChild();

		for (size_t i = 0; i < vecChild.size(); i++)
		{
			vecChild[i]->GetTransForm()->FinalUpdate();
		}

		/*Vec3 vDir = Collider()->GetWorldPos() - _pColl->GetWorldPos();
		vDir.Normalize();
		vOld += vDir * 10 * DT;
		vOld.y = vPos.y;
		TransForm()->SetOnlyPos(vOld);
		m_vOld = vOld;*/
	}

	//if (vOld.y > _pColl->GetWorldPos().y + (_pColl->GetScale().y * 0.5f))
	//{
	//	m_fLanding = _pColl->GetWorldPos().y + (_pColl->GetScale().y * 0.5f);
	//	vOld.y = m_fLanding;
	//	/*Animator3D()->PlayAnim(L"jump_land", false);
	//	m_bJump = false;*/
	//}
	//else
	//	vOld.y = vPos.y;
	//TransForm()->SetPos(vOld);

	/*Vec3 vOld = TransForm()->GetOldPos();
	Vec3 vPos = TransForm()->GetPos();
	Vec3 vDir = vOld - vPos;
	vDir.Normalize();
	while (true)
	{
		vPos += 10 * vDir * DT;
		if (vOld != vPos)
			TransForm()->SetPos(vPos);
		else
			break;

		TransForm()->FinalUpdate();
		Collider()->MatrixUpdate();

		bool bColl = CCollisionMgr::GetInst()->IsObbColl(Collider(), _pColl);
		if (bColl == false)
			break;
	}*/
}

void CAliceScript::CollisionExit(CCollider * _pColl)
{
	if (_pColl->GetOwner()->GetLayerName() == L"Land" && _pColl->GetColliderType() != COLLIDER_TYPE::COLL_BOX)
		return;

	m_bColl = false;
	if (_pColl->GetOwner()->GetLayerName() == L"Land")
	{
		if (m_bJump == false)
		{
			m_fJumpPower = 0.f;
			m_bJump = true;
		}
	}
}