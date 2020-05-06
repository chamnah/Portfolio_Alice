#pragma once
#include "global.h"

template<typename T1,typename T2>
void SafeArrayMapDelete(map<T1,T2> _map,UINT _iSize )
{
	for (UINT i = 0; i < _iSize; ++i) 
	{
		typename map<T1, T2>::iterator iter = _map[i].begin();            
		for (; iter != _map[i].end(); ++iter)
		{
			if (iter->second == nullptr)
				continue;
			delete iter->second;
			iter->second = nullptr;
		}															
	}															
}

template<typename T1, typename T2>
void SafeMapDelete(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end();)
	{
		if (iter->second == nullptr)
			continue;

		delete iter->second;
		iter->second = nullptr;
		iter = _map.erase(iter);
	}
}

template<typename T1, typename T2>
void SafeMapRelease(map<T1, T2> _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		(iter->second)->Release();
	}
}

template<typename T1>
void SafeListDelete(list<T1> _list)
{
	typename list<T1>::iterator iter = _list.begin();
	for (; iter != _list.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
}

template<typename T1>
void SafeArrayDelete(vector<T1>& _vector)
{
	typename vector<T1>::iterator iter = _vector.begin();
	for (; iter != _vector.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}

	_vector.clear();
}

void SaveWString(const wstring& _str, FILE* _pFile);
wstring LoadWString(FILE* _pFile);
float Gradient(Vec2 _vStart, Vec2 _vEnd);
float Gradient(Vec3 _vStart, Vec3 _vEnd);

float LinearEquation(Vec2 _vStart, Vec2 _vEnd, float _fMyPosX, Vec2 _vLinePos);

float RadianToDegree(float _fAngle);
float DegreeToRadian(float _fAngle);

Vec3 GetVec2Dir(Vec3 _vPos,Vec3 _vTarget);

float Lerf(float _fStart, float _fDest, float _fRatio);
Vec3  Lerf(Vec3 _vStart, Vec3 _vDest, float _fRatio);
Vec3 DecomposeRot(Mtx& _mtxRot);
Vec3 GetIntersectPlanetoLine(Vec3* _arrPlane, Vec3 _vStart, Vec3 _vDir);
int GetSizeofFormat(DXGI_FORMAT _eFormat);
Mtx GetMatrix(FbxAMatrix& _mtx);

float LandHeight(Vec3& _vStart, Vec3& _vEnd, float _fZ);