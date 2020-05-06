#include "value.fx"

struct VS_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float3 vNormal : NORMAL;
    float2 vTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float3 vOutNormal : NORMAL;
    float4 vOutViewPos : POSITION;
    float2 vOutTex : TEXCOORD;
};

VS_OUTPUT VS_Light(VS_INPUT _Input) // Input Pos == Local Pos
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), mtxWVP);
    output.vOutColor = _Input.vColor;

    output.vOutNormal = normalize(mul(float4(_Input.vNormal, 0.f), mtxWV));

    output.vOutTex = _Input.vTex;

    output.vOutViewPos = mul(float4(_Input.vPos, 1.f), mtxWV); // 현재 카메라에 방향을 알기위해서

    return output;
}

float4 PS_Light(VS_OUTPUT _Input) : SV_Target
{
   // LightInfo Info;
    //Info.vDir = float4(1.f, -1.f, 1.f, 0.f);
    //Info.iType = 0;
    //Info.color.vDiff = float4(1.f, 0.3f, 0.3f, 0.f);
    //Info.color.vAmb =  float4(0.1f, 0.1f, 0.1f, 0.f);
    //Info.color.vSpec = float4(0.4f, 0.4f, 0.4f, 0.f);

    //Info.iType = 1;
    //Info.color.vDiff = float4(1.f, 1.0f, 1.0f, 0.f);
    //Info.color.vAmb = float4(0.1f, 0.1f, 0.1f, 0.f);
    //Info.color.vSpec = float4(0.4f, 0.4f, 0.4f, 0.f);
    //Info.vPos = float4(0.f, 0.f, 0.f, 1.f);
    //Info.fDist = 20000.f;

    LightColor color;
    float4 texColor = g_Tex_0.Sample(g_Sample, _Input.vOutTex);
    float4 outColor = (float4) 0.f;
    float fLightPow = 0.f;
    for (int i = 0; i < g_iCount; ++i)
    {
        color = CalculateLight(_Input.vOutNormal, _Input.vOutViewPos.xyz, g_Info[i], fLightPow);
        outColor += texColor * color.vDiff + color.vSpec + color.vAmb;
    }

    return outColor;
}