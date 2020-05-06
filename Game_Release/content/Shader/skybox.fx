#include "value.fx"

struct VS_INPUT
{
    float3 vPos : POSITION;
    float2 vTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float2 vOutTex : TEXCOORD;
};

VS_OUTPUT VS_SkyBox(VS_INPUT _Input) // Input Pos == Local Pos
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 0.f), mtxWVP);
    //output.vOutPos = mul(output.vOutPos, mtxProj);
    output.vOutPos.z = output.vOutPos.w;

    output.vOutTex = _Input.vTex;

    return output;
}

float4 PS_SkyBox(VS_OUTPUT _Input) : SV_Target0
{
    float4 texColor = (float4)0.f;

    if (g_bTex0.x == 1)
        texColor = g_Tex_0.Sample(g_Sample, _Input.vOutTex);
    
    return texColor;
}