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

VS_OUTPUT VS_Planet(VS_INPUT _Input) // Input Pos == Local Pos
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), mtxWVP);

    output.vOutTex = _Input.vTex;

    return output;
}

float4 PS_Planet(VS_OUTPUT _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;
    float4 texColor2 = (float4) 0.f;
    float UVX = _Input.vOutTex.x;
   

    _Input.vOutTex.x = g_float_0 + _Input.vOutTex.x;
    _Input.vOutTex.y = g_float_1 + _Input.vOutTex.y;

    texColor2 = g_Tex_1.Sample(g_Sample, _Input.vOutTex);

    _Input.vOutTex.y += sin(abs(_Input.vOutTex.y - g_float_2 * 0.01f) * 200.f) * 0.002f;
    //sin(abs(_Input.vOutTex.y - g_float.z * 0.01f) * 200.f) * 0.001f;

    _Input.vOutTex.x = UVX;

    texColor = g_Tex_0.Sample(g_Sample, _Input.vOutTex);

    texColor = saturate((texColor) * texColor2 * 1.5f); // 감마값때문에 색상을 변질시켜준다.
    
    return texColor;
}