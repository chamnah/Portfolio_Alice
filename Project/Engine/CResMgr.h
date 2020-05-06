#pragma once
class CRes;
class CComponent;

#include "global.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CPrefab.h"
#include "CMaterial.h"
#include "CSound.h"
#include "CMeshData.h"

class CResMgr
{
	SINGLETON(CResMgr)

private:
	map<wstring, CRes*> m_mapRes[(UINT)RESOURCE_TYPE::END];
	vector<CMaterial*> m_vecClone;

public:
	void DefaultRectMesh();
	void CreateDefaultMaterial();
	void GridMesh(int _iVertexX, int _iVertexZ, int _iWidth,int _iDepth);
	map<wstring, CRes*>& GetResource(RESOURCE_TYPE _eType) { return m_mapRes[(UINT)_eType]; }
	void AddClone(CMaterial* _pMtrl) { m_vecClone.push_back(_pMtrl); }
	void GetDeleteClone(CMaterial* _pMtrl);
	CGameObj* PrefabToCreateObj(wstring _wcsLayer, wstring _wcsPrefabName);
	void SoundInit();
	void SoundPlay(const wstring& _wcsSound,int _iCount,bool _bRepeat = true);
	CTexture* CreateTexture(const wstring _strKey,UINT _iWidth, UINT _iHeight, UINT _iFlag, DXGI_FORMAT _eFormat,D3D11_USAGE _eUsage = D3D11_USAGE_DEFAULT);
	CTexture* CreateTexture(const wstring _strKey, ID3D11Texture2D* _pTex2D);
	CTexture* LoadAndAddTexture(const wstring _strKey, const wstring _strFileName);
	CMeshData* LoadFBX(const wstring _strPath);
	CMeshData* LoadMeshData(const wstring _strPath);


public:
	void RemoveResource(const wstring& _wcsFind, RESOURCE_TYPE _eType);

	template<typename T>
	T* Load(const wstring& _wcsFind);

	template<typename T>
	void AddResource(const wstring& _wcsFind,CRes* _pRes);

	template<typename T>
	void ChangeResource(const wstring& _wcsFind, CRes* _pRes);

	template<typename T>
	void DeleteResource(const wstring& _wcsFind);
};

template<typename T>
inline T * CResMgr::Load(const wstring & _wcsFind)
{
	// static으로 한 이유는 type_info타입을 비교하는 용도로만 쓰인다.
	// 다른 타입이 들어오면 새로운 템플릿 함수로 생성되어 새로운 static이므로 이렇게 초기화 해도 문제가 되지 않는다.
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	static const type_info& meshdata = typeid(CMeshData);
	
	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &sound)
		eType = RESOURCE_TYPE::SOUND;
	else if (&info == &meshdata)
		eType = RESOURCE_TYPE::MESHDATA;

	map<wstring,CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter == m_mapRes[(UINT)eType].end())
		return nullptr;

	return (T*)iter->second;
}

template<typename T>
inline void CResMgr::AddResource(const wstring & _wcsFind, CRes * _pRes)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	static const type_info& meshdata = typeid(CMeshData);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &sound)
		eType = RESOURCE_TYPE::SOUND;
	else if (&info == &meshdata)
		eType = RESOURCE_TYPE::MESHDATA;

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter != m_mapRes[(UINT)eType].end())
	{
		delete _pRes;
		return;
	}
	//assert(false && L"이미 해당 리소스가 존재 합니다.");

	_pRes->SetName(_wcsFind);
	_pRes->SetPath(_pRes->GetPath());

	m_mapRes[(UINT)eType].insert(make_pair(_wcsFind,_pRes));
}

template<typename T>
inline void CResMgr::ChangeResource(const wstring & _wcsFind, CRes * _pRes)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &material)
		eType = RESOURCE_TYPE::SOUND;

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter != m_mapRes[(UINT)eType].end())
	{
		delete iter->second;
		iter->second = nullptr;
		m_mapRes[(UINT)eType].erase(iter);
	}
	//assert(false && L"이미 해당 리소스가 존재 합니다.");

	_pRes->SetName(_wcsFind);
	_pRes->SetPath(_pRes->GetPath());

	m_mapRes[(UINT)eType].insert(make_pair(_wcsFind, _pRes));
}

template<typename T>
inline void CResMgr::DeleteResource(const wstring & _wcsFind)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &material)
		eType = RESOURCE_TYPE::SOUND;

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter != m_mapRes[(UINT)eType].end())
	{
		delete iter->second;
		iter->second = nullptr;
		m_mapRes[(UINT)eType].erase(iter);
	}
}