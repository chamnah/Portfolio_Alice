#include "value.fx"

#define VS_ORI 0
#define VS_COPY 1

struct VS_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float3 vNormal : NORMAL;
    float2 vTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vPos : SV_POSITION;
    //float4 vColor : COLOR;
    //float3 vNormal : NORMAL;
    float2 vTex : TEXCOORD;
};

struct GS_OUTPUT
{
    float4 vPos : SV_POSITION;
};

VS_INPUT VS_Particle(VS_INPUT _input)
{
    return _input;
}

[maxvertexcount(4)]
void GS_Particle(point VS_INPUT _ptIn[1], inout TriangleStream<VS_OUTPUT> _ptIOut)
{
    VS_OUTPUT output = (VS_OUTPUT)0.f;
    float4 vRect[4]; // point 순서 중요, 삼각형 띠 형식이라서 0번부터 삼각형 그리고 1번부터 삼각형 그림
    vRect[0] = float4(-0.5f, -0.5f, 0.f, 1.f);
    vRect[1] = float4(-0.5f, 0.5f, 0.f, 1.f);
    vRect[2] = float4(0.5f, -0.5f, 0.f, 1.f);
    vRect[3] = float4(0.5f, 0.5f, 0.f, 1.f);

    float2 vTex[4];
    vTex[0] = float2(0.f,1.f);
    vTex[1] = float2(0.f,0.f);
    vTex[2] = float2(1.f,1.f);
    vTex[3] = float2(1.f,0.f);

   for (int i = 0; i < 4; ++i)
   {
       output.vPos = mul(vRect[i], mtxWVP);
        output.vTex = vTex[i];
       _ptIOut.Append(output);
   }
}

float4 PS_Particle(VS_OUTPUT _input) : SV_Target0
{
    //if (g_bTex0 != 0)
    //{
    //   return g_Tex_0.Sample(g_Sample,_input.vTex);
    //}
    //else
   return float4(1.f, 0.f, 0.f, 0.f);
}

struct RAIN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vTex : TEXCOORD;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float fAge : AGE;
    uint iType : TYPE;
};

struct RAIN_OUT
{
    float4 vPos : SV_POSITION;
};

RAIN VS_AddRain(RAIN _input)
{
    return _input;
}

// g_Vec2 랜덤한값을 xyzw에 각각 넣어서 보냈다.
// g_floatX 사이즈X
// g_floatY 사이즈Y
[maxvertexcount(4)]
void GS_AddRain(point RAIN _In[1], inout PointStream<RAIN> _out)
{
    //_out.Append(_In[0]);
    
    _In[0].fAge += g_fDT;
    if (_In[0].iType == VS_COPY) // 복사되어서 나온 파티클이라면
    {
        if (_In[0].fAge < 3.f) // 3초동안 유지후 지나면 삭제 Append 안하면 폐기된다.
        {
            _In[0].vPos.y -= 30 * g_fDT;
            _out.Append(_In[0]);
        }
    }
    else
    {
        if (_In[0].fAge > 0.002f)
        {
            
            _In[0].fAge = 0.f;
            _In[0].vPos = mul(float4(0.f,0.f,0.f,1.f), mtxWorld).xyz;

            float2 arrRandom[3];
            arrRandom[0] = float2(g_Vec2_0.x, g_Vec2_0.y);
            arrRandom[1] = float2(g_Vec2_1.x, g_Vec2_1.y);
            arrRandom[2] = float2(g_Vec2_2.x, g_Vec2_2.y);

            for (int i = 0; i < 3; ++i)
            {
                RAIN rain = (RAIN) 0.f;
                rain.fAge = 0.f;   
                rain.iType = VS_COPY;
                rain.vPos.x += arrRandom[i].x;
                rain.vPos.y += 20.f;
                rain.vPos.z += arrRandom[i].y;
                _out.Append(rain);
            }
            _out.Append(_In[0]);
        }
    }
}

RAIN VS_DrawRain(RAIN _input)
{
    _input.vPos.y -= 30 * g_fDT;
    return _input;
}

[maxvertexcount(4)]
void GS_DrawRain(point RAIN _In[1], inout TriangleStream<RAIN_OUT> _out)
{
    RAIN_OUT output = (RAIN_OUT)0.f;
    float4 vRect[4]; // point 순서 중요, 삼각형 띠 형식이라서 0번부터 삼각형 그리고 1번부터 삼각형 그림
    vRect[0] = float4(-0.5f * g_float_0, -0.5f * g_float_1, 0.f, 1.f) + float4(_In[0].vPos, 0.f);
    vRect[1] = float4(-0.5f * g_float_0, 0.5f * g_float_1, 0.f, 1.f) + float4(_In[0].vPos, 0.f);
    vRect[2] = float4(0.5f * g_float_0, -0.5f * g_float_1, 0.f, 1.f) + float4(_In[0].vPos, 0.f);
    vRect[3] = float4(0.5f * g_float_0, 0.5f * g_float_1, 0.f, 1.f) + float4(_In[0].vPos, 0.f);

    //vRect[0] = float4(-0.5f, -0.5f, 0.f, 1.f);
    //vRect[1] = float4(-0.5f, 0.5f, 0.f, 1.f);
    //vRect[2] = float4(0.5f, -0.5f, 0.f, 1.f);
    //vRect[3] = float4(0.5f, 0.5f, 0.f, 1.f);

    for (int i = 0; i < 4;++i)
    {
        output.vPos = mul(vRect[i], mtxVP);
        _out.Append(output);
    }
}

float4 PS_DrawRain(RAIN_OUT _out) : SV_Target0
{
    return float4(1.f, 0.f, 0.f, 0.f);
}

struct Trail
{
    float3 vPos : POSITION;
};