#include "value.fx"

struct VS_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float2 vCenterUV : POSITION;
    float fMaxDistUV : POSITION2;
    float2 vOutUV : TEXCOORD;
    float4 vOutColor : COLOR;
};

//========================
// Distortion Shader
// AlphaBlend = false
// PostEffect = true
// g_Tex_0 : Output Target
// g_Tex_1 : Noise Texture
//========================

VS_OUTPUT VS_Distortion(VS_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vOutUV = _input.vUV;

    return output;
}

float4 PS_Distortion(VS_OUTPUT _input) : SV_Target
{
    float2 vScreenUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);
    
    // vScreenUV.y+=sin(abs((vScreenUV.x-g_fAccTime * 파장 속도) * 진폭 밀도)) * 파장 크기
    // vScreenUV.y += sin(abs((vScreenUV.x - g_fAccTime * 0.04f) * 50.f)) * 0.01f;
    // vScreenUV.y+=sin(abs((vScreenUV.x-g_fAccTime * 파장 속도) * 진폭 밀도)) * 파장 크기
    // vScreenUV.y += sin(abs((vScreenUV.x - g_fAccTime * 0.04f) * 50.f)) * 0.01f;
    float2 vDir = normalize(float2(0.5f, 0.5f) - _input.vOutUV);
  //  return g_tex_0.Sample(g_sam_0, vScreenUV); //* float4(1.5f, 1.f, 1.f, 1.f);

   // return g_tex_0.Sample(g_sam_0, vScreenUV) * float4(1.5f, 1.f, 1.f, 1.f);
    float fDist = distance(float2(0.5f, 0.5f), _input.vOutUV);

    // 왜곡 강도에 영향을 주는 중심으로부터 떨어진 비율( 중심에 가까울 수록 0에 가깝다.)
    float fRatio = (fDist / 0.5f);
    fRatio = fRatio * (0.2f + (fRatio) * 0.4f);

    vScreenUV += vDir * sin(abs((fRatio - g_fAccTime * 0.06f) * 80.f)) * 0.03f;
    
    float4 vColor = g_Tex_0.Sample(g_Sample, vScreenUV);
    vColor = 1.f - vColor;
    return vColor;
}

//float4 PS_Distortion(VS_OUTPUT _input) : SV_Target
//{
//    float2 vScreenUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);
    
//    float2 vDir = normalize(float2(0.5f, 0.5f) - _input.vOutUV);
//    float fDist = distance(float2(0.5f, 0.5f), _input.vOutUV);
 
//    vScreenUV += vDir * sin(abs((fDist - g_fAccTime * 0.04f) * 50.f)) * 0.03f;
    
//    return g_tex_0.Sample(g_sam_0, vScreenUV);
//}

VS_OUTPUT VS_DistortionSphere(VS_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vOutUV = _input.vUV;

    float4 vProj = mul(float4(0.f, 0.f, 0.f, 1.f), mtxWVP);
    vProj.xy /= vProj.z;
    output.vCenterUV = float2((vProj.x + 1.f) / 2.f, (1.f - vProj.y) / 2.f);

    float4 vEdge = mul(float4(1.f, 0.f, 0.f, 1.f), mtxWorld);
    float4 vViewTrans = mul(float4(0.f, 0.f, 0.f, 1.f), mtxView);
    vEdge.xyz += vViewTrans.xyz;
    vEdge = mul(vEdge, mtxProj);

    vEdge.xy /= vEdge.z;
    vEdge.xy = float2((vEdge.x + 1.f) / 2.f, (1.f - vEdge.y) / 2.f);

    output.fMaxDistUV = distance(output.vCenterUV, vEdge.xy);
    return output;
}

float4 PS_DistortionSphere(VS_OUTPUT _input) : SV_Target
{
    float2 vScreenUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);
        
    float2 vDir = _input.vCenterUV - vScreenUV;

    // 화면 비율 상쇄
    vDir.x = (vDir.x / g_fHeight) * g_fWidth;

    // 중심으로 부터 거리에 따른 왜곡 강도
    float fDist = length(vDir);

    // 왜곡 강도에 영향을 주는 중심으로부터 떨어진 비율( 중심에 가까울 수록 0에 가깝다.)
    float fRatio = (fDist / _input.fMaxDistUV);

    //fRatio = fRatio * (0.1f + (fRatio) * 0.4f);      
    vDir = normalize(vDir);
    
    vScreenUV += vDir * cos(abs((fRatio - g_fAccTime * 1.f) * 10.f)) * 0.03f;
   

    float4 vTargetColor = g_Tex_0.Sample(g_Sample, vScreenUV);
    //float4 vSunColor = g_Tex_1.Sample(g_Sample, vScreenUV);

    return vTargetColor /** vSunColor*/ * 2.f;
}

VS_OUTPUT VS_Blur(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWorld);
    output.vOutPos = mul(output.vOutPos, mtxProj);
    output.vOutUV = _input.vUV;

    return output;
}

float4 PS_Blur(VS_OUTPUT _input) : SV_Target
{
    float2 vScreenUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);
    float4 vOri = g_Tex_0.Sample(g_Sample, vScreenUV);   // OriTexture
    float4 vColor = g_Tex_1.Sample(g_Sample, vScreenUV); // BlurTexture
    float vDis = g_Tex_2.Sample(g_Sample, vScreenUV).z / 5000.f; // PositoinTexture

    if (vDis >= 1.f)
        vDis = 1.f;
    return lerp(vOri, vColor, float4(vDis, vDis, vDis, vDis));
}

VS_OUTPUT VS_Fade(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWorld);
    output.vOutPos = mul(output.vOutPos, mtxProj);
    output.vOutColor = _input.vColor;
    return output;
}

float4 PS_Fade(VS_OUTPUT _input) : SV_Target0
{
    float4 vColor = g_mtrl;
    return vColor;
}