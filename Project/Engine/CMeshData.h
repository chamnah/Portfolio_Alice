#pragma once
#include "CRes.h"
class CMesh;
class CMeshData :
	public CRes
{
private:
	CMesh* m_pMesh;
	vector<CMaterial*> m_vecMtrl;
	wstring m_strPath;
	bool   m_bAnim;


public:
	void LoadFBX(const wstring _strPath);
	CGameObj* instantiate();

	void Save(FILE* _pFile);
	static void Load(FILE* _pFile);
	static CMesh* LoadFile(wstring _strPath);
	void SaveMeshDataFile();
	static CMeshData* LoadMeshDataFile(wstring _strPath);
	CMesh* GetMesh() { return m_pMesh; }
	void SetAnim(bool _bAnim) { m_bAnim = _bAnim; }
public:
	CMeshData();
	~CMeshData();
};