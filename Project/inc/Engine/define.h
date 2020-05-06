#pragma once

#define SINGLETON(ClassName) public: static ClassName* GetInst(){ \
								static ClassName mgr; \
									return &mgr; }\
							private: ClassName();\
							~ClassName();

#define SAFE_RELEASE(p) if(p != nullptr) { p->Release(); }
#define SAFE_DELETE(p) if(p != nullptr) { delete p; p = nullptr; }

#define SAFE_ARRAY_DELETE(p,size) for(int i = 0; i < size; ++i){ SAFE_DELETE(p[i]) }
#define SAFE_ARRAY_MAP_DELETE(p, size) for (UINT i = 0; i < size; ++i) { \
	map<wstring, CRes*>::iterator iter = p[i].begin();            \
	for (; iter != p[i].end();){								\
		delete iter->second;										\
		iter = p[i].erase(iter);								\
	}															\
	}															\

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#define MAX_LAYER_COUNT 32

#define DT CTimeMgr::GetInst()->DeltaTime()

#define KEY(type, state) CKeyMgr::GetInst()->GetKeyState(type, state)
#define KEYHOLD(type) CKeyMgr::GetInst()->GetKeyState(type, KEY_STATE::HOLD)
#define KEYAWAY(type) CKeyMgr::GetInst()->GetKeyState(type, KEY_STATE::AWAY)
#define KEYTAB(type) CKeyMgr::GetInst()->GetKeyState(type, KEY_STATE::TAB)

#define DEVICE CDirect::GetInst()->GetDevice()
#define CONTEXT CDirect::GetInst()->GetContext()

#define CLONE(type) type* Clone(){ return new type(*this); }

#define RIGHT_UP 0x0001
#define LEFT_UP 0x0002
#define RIGHT_DOWN 0x0004
#define LEFT_DOWN 0x0008

#define ONE    0x0001
#define TWO    0x0002

#define GET_VALUE_NAME(a,b) b = #a

#define ORI_SIZEX 1010
#define ORI_SIZEY 500

#define TEXT_RT CRenderMgr::GetInst()->GetTextRT()
enum class KEY_TYPE
{
	KEY_1 = 0,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_CTRL,
	KEY_ALT,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_TAB,
	KEY_CAP,
	KEY_ENTER,
	KEY_ESC,
	KEY_SPACE,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_NUM0,

	KEY_LBTN,
	KEY_RBTN,

	END,
};

enum class KEY_STATE
{
	TAB,	// 최초 눌린 시점
	HOLD,   // 누르고 있는 중
	AWAY,   // 키를 막 뗀 시점
	NONE,   // 안눌림	
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESH_RENDER,
	CAMERA,
	COLLIDER,
	ANIMATION_2D,
	ANIMATION_3D,
	GRID,
	INCIDENT,
	LIGHT,
	FILTER,
	TERRAIN,
	TILE,
	TEXT,
	END,
	SCRIPT,
};

enum class COLL_TYPE
{
	NOCOLL,
	COLL,
	DRAWCOLL,
	NONE
};

enum class COLLIDER_TYPE
{
	COLL_RECT,
	COLL_BOX,
	COLL_CIRCLE_2D,
	COLL_CIRCLE_3D,
	COLL_ISO,
	COLL_HEXAGON
};

enum class RESOURCE_TYPE
{
	MESH,
	TEXTURE,
	PREFAB,
	MATERIAL,
	SOUND,
	MESHDATA,
	END
};

enum class BLEND_TYPE
{
	NONE,
	ALPHA,
	ADD,
	LIGHTING,
	ONEONE,
	SUB,
	END
};

enum class SAMPLER_TYPE
{
	CLAMP,
	WRAP,
	END
};

enum class DEPTH_STENCIL_TYPE
{
	LESS ,
	LESS_EQUAL,
	FIND_FRONT,
	FIND_BACK,
	FIND_EQUAL,
	NO_NO,
	GREATER_EQUAL,
	END
};

enum class RASTERIZE_TYPE
{
	CULL_NONE,
	CULL_BACK,
	CULL_FRONT,
	WIREFRAME,
	END
};

enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_END,
	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_END,
	MTRL,
	MTRL_END,
	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_END,
	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_END,
	MTX_0,
	MTX_1,
	MTX_2,
	MTX_3,
	MTX_4,
	MTX_5,
	MTX_6,
	MTX_END,
	
	BOOL_0,
	BOOL_1,
	BOOL_2,
	BOOL_3,
	BOOL_4,
	BOOL_5,
	BOOL_6,
	BOOL_7,
	BOOL_8,
	BOOL_9,
	BOOL_10,
	BOOL_11,
	BOOL_END,

	RBOOL_0,
	RBOOL_1,
	RBOOL_2,
	RBOOL_3,
	RBOOL_END,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	ARRTEX_0,
	TEX_END,

	RWTEX_0,
	RWTEX_1,
	RWTEX_2,
	RWTEX_3,
	RWTEX_END
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	FRONT_LEFT,
	END
};

enum class EVENT_TYPE
{
	CREATE_OBJ,
	DELETE_OBJ,
	ADD_CHILD,
	DISCONNECT_PARENT,
	CHANGE_LAYER,
	CHANGE_SCENE
};

enum class TEAM_TYPE
{
	NONE,
	MONSTER,
	PLAYER
};

enum class LIGHT_TYPE
{
	DIR,
	POINT,
	SPOT
};

enum class RT_TYPE
{
	SWAP,
	OUTPUT,
	COLOR,
	NORMAL,
	POSITION,
	OBJ_SPECULAR,
	DIFFUSE,
	SPECULAR,
	/*SHADOW_0,
	SHADOW_1,
	SHADOW_2,
	SHADOW_3,
	SHADOW_4,
	SHADOW_5,*/
	END
};

enum class MRT_TYPE
{
	SWAP,
	OUTPUT,
	DEFERRED,
	LIGHT,
	//SHADOW,
	END
};

enum class BUTTON_COLL
{
	BOX,
	HEXAGON,
	END
};

enum class BRUSH_TYPE
{
	HEIGHT,
	WEIGHT
};

enum class EFFECT_TYPE
{
	FADEOUT,
	BILLBOARD,
	TIMEOUT
};

enum class RENDER_TYPE
{
	DEFFERED,
	FORWARD,
	POSTEFFECT,
	NO_INST,
	END
};

enum class FILTER_TYPE
{
	NONE,
	BLUR
};

enum class WEAPON_TYPE
{
	VOPAL,
	GRINDER,
	TEAPOT,
	END
};

enum class ITEM_TYPE
{
	SCORE,
	BOTTLE
};