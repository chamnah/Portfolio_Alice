#pragma once
#include "CScript.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CScriptMgr.h"
template<typename T>
class CMgrScript : public CScript
{
private:
	static T*  m_pMgr;

public:
	static T* GetInst()
	{
		if (nullptr != m_pMgr)
			return m_pMgr;

		CGameObj* pOwner = CSceneMgr::GetInst()->FindObject(L"MgrObj");

		if (pOwner == nullptr)
		{
			pOwner = new CGameObj;
			pOwner->SetName(L"MgrObj");

			int iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
			tEvent event = tEvent{ EVENT_TYPE::CREATE_OBJ,iIdx , (INT_PTR)pOwner };
			CEventMgr::GetInst()->AddEvent(event);
		}
	
		vector<CScript*>& vecScript = pOwner->GetScript();
		m_pMgr = new T;
		
		if (!vecScript.empty())
		{
			wstring wcsScript = CScriptMgr::GetScriptName(m_pMgr);

			for (UINT i = 0; i < vecScript.size(); ++i)
			{
				if (CScriptMgr::GetScriptName(vecScript[i]) == wcsScript)
				{
					delete m_pMgr;
					m_pMgr = (T*)vecScript[i];
					return m_pMgr;
				}
			}
		}
		pOwner->AddComponent(m_pMgr); // 이 안에서 삭제도 일어날것이다. 구조상

		return m_pMgr;
	}

	CMgrScript() {}
	~CMgrScript() { m_pMgr = nullptr; }
};

template<typename T>
T* CMgrScript<T>::m_pMgr = nullptr;