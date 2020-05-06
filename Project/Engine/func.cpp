#include "func.h"

void SaveWString(const wstring & _str, FILE * _pFile)
{
	assert(!(255 < _str.length()));

	BYTE length = (BYTE)_str.length();
	fwrite(&length, sizeof(BYTE), 1, _pFile);
	fwrite(_str.c_str(), sizeof(wchar_t), length, _pFile);
}

wstring LoadWString(FILE * _pFile)
{
	wchar_t wcsBuff[150] = {};
	BYTE length = 0;
	wmemset(wcsBuff, 0, 150);
	fread(&length, sizeof(BYTE), 1, _pFile);
	fread(wcsBuff,sizeof(wchar_t),length,_pFile);

	return wcsBuff;
}

float Gradient(Vec2 _vStart, Vec2 _vEnd)
{
	float fGradient = (_vEnd.y - _vStart.y) / (_vEnd.x - _vStart.x);
	return fGradient;
}

float Gradient(Vec3 _vStart, Vec3 _vEnd)
{
	float fGradient = (_vEnd.y - _vStart.y) / (_vEnd.x - _vStart.x);
	return fGradient;
}

float LinearEquation(Vec2 _vStart, Vec2 _vEnd, float _fMyPosX, Vec2 _vLinePos)
{
	float fGra = Gradient(_vStart, _vEnd);
	float fY = -(fGra * _vLinePos.x) + _vLinePos.y; // Y절편 구하기 b = -ax + y
	float fOut = fGra * _fMyPosX + fY; // y = ax + b;

	return fOut;
}

float RadianToDegree(float _fAngle)
{
	return _fAngle * 180.f / XM_PI;
}

float DegreeToRadian(float _fAngle)
{
	return _fAngle * XM_PI / 180.f;
}

Vec3 GetVec2Dir(Vec3 _vPos, Vec3 _vTarget)
{
	_vPos.z = 0.f;
	_vTarget.z = 0.f;
	Vec3 vDir = _vTarget - _vPos;
	vDir.Normalize();

	return vDir;
}

float Lerf(float _fStart, float _fDest, float _fRatio)
{
	return _fStart + (_fDest - _fStart) * _fRatio;
}

Vec3 Lerf(Vec3 _vStart, Vec3 _vDest, float _fRatio)
{
	Vec3 vLerf;
	vLerf.x = Lerf(_vStart.x, _vDest.x, _fRatio);
	vLerf.y = Lerf(_vStart.y, _vDest.y, _fRatio);

	return vLerf;
}

bool closeEnough(const float& a, const float& b
	, const float& epsilon = std::numeric_limits<float>::epsilon())
{
	return (epsilon > std::abs(a - b));
}

Vec3 DecomposeRot(Mtx& _mtxRot)
{
	// _mat 을 분해 후 다시 행렬 만들기	
	Vec4 vMat[4];
	XMStoreFloat4(&vMat[0], _mtxRot.Right());   // 1
	XMStoreFloat4(&vMat[1], _mtxRot.Up());      // 2
	XMStoreFloat4(&vMat[2], _mtxRot.Front());   // 3
	XMStoreFloat4(&vMat[3], _mtxRot.Translation());//4

	Vec3 vNewRot;
	if (closeEnough(vMat[0].z, -1.0f)) {
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2(vMat[1].x, vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else if (closeEnough(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-vMat[1].x, -vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asin(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cos(y1), vMat[2].z / cos(y1));
		float x2 = atan2f(vMat[1].z / cos(y2), vMat[2].z / cos(y2));

		float z1 = atan2f(vMat[0].y / cos(y1), vMat[0].x / cos(y1));
		float z2 = atan2f(vMat[0].y / cos(y2), vMat[0].x / cos(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vec3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vec3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}

Vec3 GetIntersectPlanetoLine(Vec3 * _arrPlane, Vec3 _vStart, Vec3 _vDir)
{
	Vec3 vFlow0 = _arrPlane[1] - _arrPlane[0];
	Vec3 vFlow1 = _arrPlane[2] - _arrPlane[0];

	Vec3 vNormal = vFlow0.Cross(vFlow1);
	vNormal.Normalize();

	float fDist =  -(_arrPlane[0].Dot(vNormal));

	float fT = -(_vStart.Dot(vNormal)) - fDist / _vDir.Dot(vNormal);

	return _vStart + (_vDir * fT);
}

// 직선의 방정식과 평면 방정식을 이용한 교점 구하기
// 직선의 방정식 = x= x0 + lt; y = y0 + mt; z = z0 + nt  방향벡터(l,m,n), 시작점 벡터(x0,y0,z0),t(변화량)
// 평면 방정식 ax + by + cz +d = 0;
// a(x0 + lt) + b(y0 + mt) + c(z0 + nt) + d = 0; => ax0 + alt + by0 + bmt + cz0 + cnt + d = 0
// t(al,bm,cn) + ax0 + by0 + cz0 + d = 0; => t = (-(ax0 + by0 + cz0) - d) / (al,bm,cn)


int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

Mtx GetMatrix(FbxAMatrix & _mtx)
{
	Mtx mtx = {};
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			mtx.m[i][j] = _mtx.Get(i,j);
		}
	}
	return mtx;
}

float LandHeight(Vec3 & _vStart, Vec3 & _vEnd, float _fZ)
{
	float fGra = (_vEnd.y - _vStart.y) / (_vEnd.z - _vStart.z);

	float Y = -(fGra * _vStart.z) + _vStart.y; // Y절편 구하기
	float Height = fGra * _fZ + Y; // 최종 방정식

	return Height;
}
