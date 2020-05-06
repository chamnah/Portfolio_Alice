#include "value.fx"
struct VS_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    float3 vTangent : TANGENT;
    float2 vTex : TEXCOORD;

};

struct VS_OUTPUT
{
    float3 vPos : POSITION;
    float3 vWorld : POSITION1;
    float4 vColor : COLOR;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    float3 vTangent : TANGENT;
    float2 vTex : TEXCOORD0;
    float2 vRange : TEXCOORD1;
};

struct PatchInfo
{
    float arrEdge[3] : SV_TessFactor;
    float fInner : SV_InsideTessFactor;
};

PatchInfo PatchFunc(InputPatch<VS_OUTPUT,3> _input,int _iIdx : SV_PrimitiveID)
{
    PatchInfo info = (PatchInfo) 0.f;
    float3 vStdPos = (_input[0].vWorld.xyz + _input[2].vWorld.xyz) / 2.f; // 중심 위치 
    float fXStep = (_input[1].vWorld.x - _input[0].vWorld.x) / 2.f;
    float fZStep = (_input[1].vWorld.z - _input[2].vWorld.z) / 2.f;

    float3 vXPos = float3(vStdPos.x + fXStep, vStdPos.y, vStdPos.z);
    float3 vZPos = float3(vStdPos.x, vStdPos.y, vStdPos.z + fZStep);
    
   /* float fTess = CalTessLvl(g_Vec2_3.x, g_Vec2_3.y, 16, 1, vStdPos);
    float fXTess = CalTessLvl(g_Vec2_3.x, g_Vec2_3.y, 16, 1, vXPos);
    float fZTess = CalTessLvl(g_Vec2_3.x, g_Vec2_3.y, 16, 1, vZPos);*/

    float fTess = CalTessLvl(g_float_2, g_float_3, 16, 1, vStdPos);
    float fXTess = CalTessLvl(g_float_2, g_float_3, 16, 1, vXPos);
    float fZTess = CalTessLvl(g_float_2, g_float_3, 16, 1, vZPos);
       
    info.arrEdge[0] = fXTess; // 마주보고 있는 엣지 갯수[0]번째 정점이 바라보고 있는 엣지
    info.arrEdge[1] = fTess;
    info.arrEdge[2] = fZTess;
    info.fInner = fTess;

    //PatchInfo info = (PatchInfo) 0.f;
    //info.arrEdge[0] = 16.f;
    //info.arrEdge[1] = 16.f;
    //info.arrEdge[2] = 16.f;
    //info.fInner = 16.f;

    return info;
}

VS_OUTPUT VS_Tess(VS_INPUT _input)
{
    VS_OUTPUT output;

    output.vPos = _input.vPos;
    output.vColor = _input.vColor;
    output.vNormal = _input.vNormal;
    output.vBinormal = _input.vBinormal;
    output.vTangent = _input.vTangent;
    output.vTex = _input.vTex;
    output.vWorld = mul(float4(_input.vPos, 1.f), mtxWorld);
    output.vRange = g_Vec2_3;
    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")] //테셀레이터에서 삼각형을 시계방향 생성한다 .
[outputcontrolpoints(3)]
[patchconstantfunc("PatchFunc")]
[maxtessfactor(32.0)]
VS_OUTPUT HS_Tess(InputPatch<VS_OUTPUT,3> _input,int _iIdx : SV_OutputControlPointID)
{
    return _input[_iIdx];
}

struct DS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vViewPos : POSITION;
    float4 vViewNormal : NORMAL;
    float4 vViewBinormal : BINORMAL;
    float4 vViewTangent : TANGENT;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// UV에는 3개의 삼각형정점에 상대적인 위치에대한 각 비율을 알려준다.
// outputpatch로 삼각형 정점들을 가져와야하고 모든 정점이 호출이 될때 input에는 항상 3개의 삼각형 정점 값이 들어온다. 
// hull셰이더 에서 만든 삼각형의 엣지 정보도 넘어온다.

// g_float0 = 하나의 TEXEL 가로 UV의 X값
// g_float1 = 하나의 TEXEL 가로 UV의 Y값
// g_vec2_0 = BrushUVPos; 터레인 전체를 하나의 0~1사이로 판단한 UV
// g_vec2_1 = BrushUVSize; 터레인 전체를 하나의 0~1사이로 판단한 UV
// g_vec2_2 = FaceCount
// g_Tex_0 = Height Map;
// g_Tex_1 = BrushTexture;
// g_Tex_2 = Weight Map;
// g_Tex_3 = Diffuse Map;
// g_arrTex_0 = Weight Texture 배열

[domain("tri")]
DS_OUTPUT DS_Tess(PatchInfo tessfactor, OutputPatch<VS_OUTPUT, 3> _input, float3 _InputUV : SV_DOMAINLOCATION) // 테셀레이터를 거친 모든 정점들이 호출이 되어진다. 
{
    DS_OUTPUT output = (DS_OUTPUT) 0.f;
    VS_INPUT vtxInput = (VS_INPUT) 0.f;

    vtxInput.vPos = _input[0].vPos * _InputUV.x + _input[1].vPos * _InputUV.y + _input[2].vPos * _InputUV.z;
    vtxInput.vNormal = _input[0].vNormal * _InputUV.x + _input[1].vNormal * _InputUV.y + _input[2].vNormal * _InputUV.z;
    vtxInput.vTex = _input[0].vTex * _InputUV.x + _input[1].vTex * _InputUV.y + _input[2].vTex * _InputUV.z;
    vtxInput.vColor = _input[0].vColor * _InputUV.x + _input[1].vColor * _InputUV.y + _input[2].vColor * _InputUV.z;
    vtxInput.vTangent = _input[0].vTangent * _InputUV.x + _input[1].vTangent * _InputUV.y + _input[2].vTangent * _InputUV.z;
    vtxInput.vBinormal = _input[0].vBinormal * _InputUV.x + _input[1].vBinormal * _InputUV.y + _input[2].vBinormal * _InputUV.z;

    if (g_bTex0.x != 0)
    {
        float3 vLocalPos = vtxInput.vPos;
        float4 vColor = g_Tex_0.SampleLevel(g_Sample, vtxInput.vTex, 0); // 높이맵에 기록되어있던 높이 값을 넣는다.
        float fHeight = ((vColor.x + vColor.y + vColor.z) / 3.f);
        vtxInput.vPos.y = fHeight;

        float4 vRightHeight = g_Tex_0.SampleLevel(g_Sample, float2(vtxInput.vTex.x + g_float_0, vtxInput.vTex.y), 0);
        float4 vLeftHeight = g_Tex_0.SampleLevel(g_Sample, float2(vtxInput.vTex.x - g_float_0, vtxInput.vTex.y), 0);

        float3 vRightPos = vLocalPos;
        vRightPos.x += 1.f / 16.f;
        vRightPos.y = (vRightHeight.x + vRightHeight.y + vRightHeight.z) / 3.f;
      
        float3 vLeftPos = vLocalPos;
        vLeftPos.x -= 1.f / 16.f;
        vLeftPos.y = (vLeftHeight.x + vLeftHeight.y + vLeftHeight.z) / 3.f;

        vtxInput.vTangent = normalize(vRightPos - vLeftPos);
        vtxInput.vBinormal = float3(0.f,0.f,-1.f);
        vtxInput.vNormal = normalize(cross(vtxInput.vTangent, vtxInput.vBinormal));
    }

    output.vOutPos = mul(float4(vtxInput.vPos, 1.f),mtxWVP);
    output.vViewPos = mul(float4(vtxInput.vPos, 1.f), mtxWV);
    output.vViewNormal = mul(float4(vtxInput.vNormal, 0.f), mtxWV);
    output.vViewTangent = mul(float4(vtxInput.vTangent, 0.f), mtxWV);
    output.vViewBinormal = mul(float4(vtxInput.vBinormal, 0.f), mtxWV);
    output.vColor = vtxInput.vColor;
    output.vUV = vtxInput.vTex;

    return output;
}

struct PS_OUTPUT
{
    float4 vTarget0 : SV_Target0;
    float4 vTarget1 : SV_Target1;
    float4 vTarget2 : SV_Target2;
};

PS_OUTPUT PS_Tess(DS_OUTPUT _input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0.f;

    if (g_bTex2.x != 0 && g_bArrTex0.x != 0)
    {
        float4 vWeight = g_Tex_2.Sample(g_Sample, _input.vUV);
        float3 vColor = float3(0.f, 0.f, 0.f);
        for (int i = 0; i < 4; ++i)
        {
            float2 vUV = _input.vUV * g_Vec2_2;
            vColor += vWeight[i] * g_arrTex_0.Sample(g_Sample, float3(vUV, (float) i)).xyz;
        }
        if (!(vWeight.x || vWeight.y || vWeight.z || vWeight.w))
            vColor = float4(0.8f, 0.8f, 0.8f, 0.8f);
        output.vTarget0.rgb = vColor;
    }
    else
        output.vTarget0.rgb = float4(1.f, 1.f, 1.f, 1.f).xyz; //g_Tex_0.Sample(g_Sample, _input.vUV).rgb; 

    output.vTarget1 = normalize(float4(_input.vViewNormal.xyz, 0.f));
    _input.vViewPos.a = 0.f;
    output.vTarget2 = _input.vViewPos;

    if (g_bTex1.x != 0)
    {
        float2 vBrsuhUV = ((_input.vUV - g_Vec2_0) / g_Vec2_1) + float2(0.5f, 0.5f);
        if (0.f < vBrsuhUV.x && vBrsuhUV.x < 0.99f && 0.f < vBrsuhUV.y && vBrsuhUV.y < 1.f)
            output.vTarget2.a = g_Tex_1.Sample(g_Sample, vBrsuhUV).a;
    }

    return output;
}