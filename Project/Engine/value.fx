struct LightColor
{
    float4 vDiff;
    float4 vSpec;
    float4 vAmb;
};

struct LightInfo
{
    LightColor color;
    float4 vPos;
    float4 vDir;
    float fDist;
    int iType;
    float fAngle;
    int iPadding;
};

cbuffer MatBuff : register(b0)
{
   row_major float4x4 mtxWorld;
   row_major float4x4 mtxView;
   row_major float4x4 mtxProj;
   row_major float4x4 mtxWV;
   row_major float4x4 mtxVP;
   row_major float4x4 mtxWVP;
   row_major float4x4 mtxInvWorld;
};

cbuffer ShaderParam : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float4 g_mtrl;

    float2 g_Vec2_0;
    float2 g_Vec2_1;
    float2 g_Vec2_2;
    float2 g_Vec2_3; // uv mapping

    float4 g_Vec4_0;
    float4 g_Vec4_1;
    float4 g_Vec4_2;
    float4 g_Vec4_3;

    row_major float4x4 g_Mtx0;
    row_major float4x4 g_Mtx1;
    row_major float4x4 g_Mtx2;
    row_major float4x4 g_Mtx3;
    row_major float4x4 g_Mtx4;
    row_major float4x4 g_Mtx5;
    row_major float4x4 g_Mtx6;

    int g_bTex0;
    int g_bTex1;
    int g_bTex2;
    int g_bTex3;
    int g_bTex4;
    int g_bTex5;
    int g_bTex6;
    int g_bTex7;
    int g_bArrTex0;
    int g_bArrTex1;
    int g_bArrTex2;
    int g_bArrTex3;

    int g_bRTex0;
    int g_bRTex1;
    int g_bRTex2;
    int g_bRTex3;
};

cbuffer AnimParam : register(b2)
{
    float2 g_fLT;
    float2 g_fSize;
    int g_bUse;
    float2 g_fOffset;
    int g_iPadding;
};

cbuffer Light : register(b3)
{
    LightInfo g_Info[10];
};

cbuffer Global : register(b4)
{
    float g_fDT;
    float g_fAccTime;
    int g_iCount;
    float g_fWidth;
    float g_fHeight;
    float3 g_vMCamPos;
};

cbuffer Target : register(b5)
{
    int g_iTarget;
    int g_iTPadding[3];
};

struct tSkinningInfo
{
    float3 vPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};
       
Texture2D g_Tex_0 : register(t0);
Texture2D g_Tex_1 : register(t1);
Texture2D g_Tex_2 : register(t2);
Texture2D g_Tex_3 : register(t3);
Texture2D g_Tex_4 : register(t4);
Texture2D g_Tex_5 : register(t5);
Texture2D g_Tex_6 : register(t6);
Texture2D g_Tex_7 : register(t7);
        
Texture2DArray g_arrTex_0 : register(t8);
Texture2DArray g_arrTex_1 : register(t9);
Texture2DArray g_arrTex_2 : register(t10);
Texture2DArray g_arrTex_3 : register(t11);


RWTexture2D<float4> g_RWTex_0 : register(u0);

Texture2D g_Anim_Tex : register(t15);
SamplerState g_Sample : register(s0);
//groupshared float4 gChache[266];

LightColor CalculateLight(float3 vNormal,float3 vPos, LightInfo _Info,out float _fLightPow)
{
    LightColor outColor = (LightColor) 0.f;
    if (_Info.iType == 0) // Dir
    {
        _Info.vDir = normalize(mul(_Info.vDir, mtxView));

       // _Info.vDir = normalize(_Info.vDir);

        _fLightPow = saturate(dot(vNormal, -_Info.vDir.xyz));
        _Info.color.vDiff *= _fLightPow;


        float3 fReflect = normalize(_Info.vDir.xyz + 2 * vNormal * dot(-_Info.vDir.xyz, vNormal));
        float3 fEye = normalize(vPos);
        float3 fSpecular = saturate(dot(-fEye, fReflect));

        _Info.color.vSpec *= float4(pow(fSpecular, 5.f), 0.f);
        outColor = _Info.color;
    }
    else if (_Info.iType == 1) // Point
    {
        _Info.vPos = mul(float4(_Info.vPos.xyz, 1.f), mtxView);
        
        float3 vLightDir = normalize(vPos - _Info.vPos.xyz);

        float fRatio = saturate( (_Info.fDist - distance(vPos.xyz, _Info.vPos.xyz)) / _Info.fDist);
      
        outColor.vDiff = _Info.color.vDiff * saturate(dot(vNormal, -vLightDir)) * fRatio;
    }
    else if (_Info.iType == 2) // Spot
    {
        _Info.vDir = normalize(mul(_Info.vDir, mtxView));
        _Info.vPos = mul(_Info.vPos, mtxView);
        float3 vLightDir = normalize(vPos - _Info.vPos.xyz);

        float fRatio = saturate((_Info.fDist - distance(vPos, _Info.vPos.xyz)) / _Info.fDist);
      
        if (dot(_Info.vDir, float4(vLightDir, 0.f)) > _Info.fAngle)
        {
            _fLightPow = saturate(dot(vNormal, -vLightDir)) * fRatio;
            outColor.vDiff = _Info.color.vDiff * _fLightPow;
        }
        else
        {
            _fLightPow = 0.f;
            outColor.vDiff = float4(0.f, 0.f, 0.f, 0.f);
        }
    }
    return outColor;
}

int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir)
{
    float3 edge[2];
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);

    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    float3 p = _vStart + t * _vDir;
    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }
        
    return 1;
}

float3 GetIntersectPlanetoLine(float3 _vertices[3], float3 _vStart, float3 _vDir)
{
    float3 vFlow1 = _vertices[1] - _vertices[0];
    float3 vFlow2 = _vertices[2] - _vertices[0];

    float3 vNormal = normalize(cross(vFlow1, vFlow2));
    float fDist = -(dot(_vertices[0], vNormal));

    float fT = (-(dot(_vStart, vNormal)) - fDist) / dot(_vDir, vNormal);

    return _vStart + (_vDir * fT);
}

// ax + by + cz + d = 0; // a b c 를 normal vector로 본다. 임의의점 xyz가 이 방정식에 대입햇을때 부등호로 알아낼수있다.
//   T = (ax + by + cz + d) - start / dir;

float CalTessLvl(float _fMinDist, float _fMaxDist, float _fMaxTess, float _fMinTess, float3 _vWorldPos)
{
    float3 vCamPos = g_Vec4_0.xyz;
    float3 vDist = vCamPos - _vWorldPos;

    vDist.y = 0.f;

    float fDist = length(vDist); // 거리를 나타낸다.

    float fMaxPower = log2(_fMaxTess);
    float fMinPower = log2(_fMinTess);

    float fRatio = saturate((fDist - _fMinDist) / (_fMaxDist - _fMinDist));
    
    return pow(2, floor(lerp(fMaxPower, fMinPower, fRatio)));
}

float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

float2 rand_2_10(in float2 uv)
{
    float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    float noiseY = sqrt(1 - noiseX * noiseX);
    return float2(noiseX, noiseY);
}

float2 rand_2_0004(in float2 uv)
{
    float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453));
    float noiseY = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return float2(noiseX, noiseY) * 0.004;
}

matrix GetBoneMatrix(int _iBoneIdx, int _iRowIdx,Texture2D _tex)
{
    matrix mtxBone =
    {
       _tex.Load(int3(_iBoneIdx * 4, _iRowIdx, 0)),
       _tex.Load(int3(_iBoneIdx * 4 + 1, _iRowIdx, 0)),
       _tex.Load(int3(_iBoneIdx * 4 + 2, _iRowIdx, 0)),
       _tex.Load(int3(_iBoneIdx * 4 + 3, _iRowIdx, 0))
    };
    return mtxBone;
}

void Skinnig(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal, inout float4 _vWeight, inout float4 _vIndices, int _iRowIdx)
{
    tSkinningInfo Info = (tSkinningInfo)0.f;

    for (int i = 0; i < 4; ++i)
    {
        if (_vWeight[i] == 0.f)
            continue;
        matrix mtxBone = GetBoneMatrix(_vIndices[i], _iRowIdx, g_Tex_7);

        Info.vPos += (mul(float4(_vPos, 1.f), mtxBone) * _vWeight[i]).xyz;
        Info.vTangent += (mul(float4(_vTangent, 0.f), mtxBone) * _vWeight[i]).xyz;
        Info.vBinormal += (mul(float4(_vBinormal, 0.f), mtxBone) * _vWeight[i]).xyz;
        Info.vNormal += (mul(float4(_vNormal, 0.f), mtxBone) * _vWeight[i]).xyz;
    }

    _vPos = Info.vPos;
    _vTangent = normalize(Info.vTangent);
    _vBinormal = normalize(Info.vBinormal);
    _vNormal = normalize(Info.vNormal);
}