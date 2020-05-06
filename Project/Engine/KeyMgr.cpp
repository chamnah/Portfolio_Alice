#include "KeyMgr.h"
#include "CCore.h"
#include "CCamera.h"

int g_vkIdx[(UINT)KEY_TYPE::END] =
{
	'1', // KEY_1
	'2', // KEY_2
	'3', // KEY_3
	'4', // KEY_4
	'5', // KEY_5
	'6', // KEY_6
	'7', // KEY_7
	'8', // KEY_8
	'9', // KEY_9
	'0', // KEY_0
	'Q', // KEY_Q
	'W', // KEY_W,
	'E', // KEY_E
	'R', // KEY_R,
	'T', // KEY_T,
	'Y', // KEY_Y,
	'U', // KEY_U,
	'I', // KEY_I,
	'O', // KEY_O,
	'P', // KEY_P,
	'A', // KEY_A,
	'S', // KEY_S,
	'D', // KEY_D,
	'F', // KEY_F,
	'G', // KEY_G,
	'H', // KEY_H,
	'J', // KEY_J,
	'K', // KEY_K,
	'L', // KEY_L,
	'Z', // KEY_Z,
	'X', // KEY_X,
	'C', // KEY_C,
	'V', // KEY_V,
	'B', // KEY_B,
	'N', // KEY_N,
	'M', // KEY_M,

	VK_F1,	// KEY_F1,
	VK_F2,	// KEY_F2,
	VK_F3,	// KEY_F3,
	VK_F4,	// KEY_F4,
	VK_F5,	// KEY_F5,
	VK_F6,	// KEY_F6,
	VK_F7,	// KEY_F7,
	VK_F8,	// KEY_F8,
	VK_F9,	// KEY_F9,
	VK_F10,	// KEY_F10,
	VK_F11,	// KEY_F11,
	VK_F12,	// KEY_F12,

	VK_LCONTROL,// KEY_CTRL,
	VK_LMENU,	// KEY_ALT,
	VK_LSHIFT,	// KEY_LSHIFT,
	VK_RSHIFT,	// KEY_RSHIFT,
	VK_TAB,		// KEY_TAB,
	VK_CAPITAL,	// KEY_CAP,
	VK_RETURN,	// KEY_ENTER,
	VK_ESCAPE,	// KEY_ESC,
	VK_SPACE,	// KEY_SPACE,
	VK_UP,		// KEY_UP,
	VK_DOWN,	// KEY_DOWN,
	VK_LEFT,	// KEY_LEFT,
	VK_RIGHT,	// KEY_RIGHT,
	VK_NUMPAD1,	// KEY_NUM1,
	VK_NUMPAD2,	// KEY_NUM2,
	VK_NUMPAD3,	// KEY_NUM3,
	VK_NUMPAD4,	// KEY_NUM4,
	VK_NUMPAD5,	// KEY_NUM5,
	VK_NUMPAD6,	// KEY_NUM6,
	VK_NUMPAD7,	// KEY_NUM7,
	VK_NUMPAD8,	// KEY_NUM8,
	VK_NUMPAD9,	// KEY_NUM9,
	VK_NUMPAD0,	// KEY_NUM0,	

	VK_LBUTTON,// KEY_LBTN
	VK_RBUTTON,// KEY_RBTN
};

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY_TYPE::END; ++i)
	{
		m_vecKey.push_back(tKey{ (KEY_TYPE)i , KEY_STATE::NONE });
	}
}

void CKeyMgr::update()
{
	bool bFocus = true;// CCore::GetInst()->GetWndFocus();

	for (UINT i = 0; i < m_vecKey.size(); ++i)
	{
		// 키가 눌렸는지 체크
		if (bFocus && GetAsyncKeyState(g_vkIdx[(UINT)m_vecKey[i].eType]) & 0x8000)
		{
			// 키가 눌렸는데, 이전에 TAB(최초 누름) 상태였다면...
			if (KEY_STATE::TAB == m_vecKey[i].eState)
			{
				// 막 눌린상태 -> HOLD 유지
				m_vecKey[i].eState = KEY_STATE::HOLD;
			}
			else if (KEY_STATE::HOLD != m_vecKey[i].eState)
			{
				// Away, None -> Tab
				m_vecKey[i].eState = KEY_STATE::TAB;
			}
		}
		else
		{
			// AWAY -> NONE
			// HOLD, TAB -> AWAY
			// NONE -> NONE
			if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
			else if (KEY_STATE::NONE != m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
		}
	}

	// 마우스 현재 좌표 계산
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(m_hWnd, &ptPos);

	m_vWinMousePos = Vec3(ptPos.x, ptPos.y, 0.f);

	//m_vMousePos.x = ((+2.f * ptPos.x) / (CCore::GetInst()->GetResolution().x) - 1.f);
	//m_vMousePos.y = (-2.f * ptPos.y) / (CCore::GetInst()->GetResolution().y + 1.f); // NDC 상에서 좌표
	RECT rt = {};
	GetClientRect(m_hWnd, &rt);

	POINT vClient = { rt.right - rt.left,rt.bottom - rt.top };

	m_vMousePos.x = (ptPos.x - vClient.x * 0.5f) * (CCore::GetInst()->GetResolution().x / vClient.x);
	m_vMousePos.y = (vClient.y * 0.5f - ptPos.y) * (CCore::GetInst()->GetResolution().y / vClient.y);


	m_mtxMouse = XMMatrixIdentity();
	m_mtxMouse.Translation(m_vMousePos);

	//m_vMousePos.x = ((+2.f * ptPos.x) / CCore::GetInst()->GetResolution().x - 1.f) * CCore::GetInst()->GetResolution().x * 0.5f; //NDC를 이용한 클라이언트 좌표 만들기
	//m_vMousePos.y = ((-2.f * ptPos.y) / CCore::GetInst()->GetResolution().y + 1.f) * CCore::GetInst()->GetResolution().y * 0.5f;



	/*m_vMousePos = XMVector3TransformCoord(m_vMousePos,g_World.mtxView);
	m_vMousePos = XMVector3TransformCoord(m_vMousePos, g_World.mtxProj); 연습*/

	//m_vMousePos.x = ((+2.f * ptPos.x) / CCore::GetInst()->GetResolution().x - 1.f);
	//m_vMousePos.y = ((-2.f * ptPos.y) / CCore::GetInst()->GetResolution().y + 1.f);


	//m_vMousePos.x = (float)ptPos.x - (CCore::GetInst()->GetResolution().x / 2.f);
	//m_vMousePos.y = -((float)ptPos.y - (CCore::GetInst()->GetResolution().y / 2.f));
}

const Vec3& CKeyMgr::GetDXMousePos() // 스크린에서의 좌표를 반환시킨다.
{
	return m_vMousePos; 
}