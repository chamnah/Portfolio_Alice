//#include "value.fx"
//
//// ==========================
//// Terrain Shader
//
//// g_vec2_0 : Texcell 1칸당 UV 값
//// g_vec2_1 : Brush Position
//// g_vec2_2 : Brush Scale
//// g_vec2_3 : Face Count(지형 면 개수)
//
//// g_texarr_0 : Splatting Tile Texture
//
//// g_tex_0 : Height Map Texture
//// g_tex_1 : Splatting Weight Texture
//// g_tex_2 : Brush Texture
//
//// g_tex_3 : normal Texture
//
//// g_tex_4 : Tile Tex
//// g_tex_5 : Tile Tex
//
//// ==========================
//struct VTX_INPUT
//{
//    float3 vPos : POSITION;
//    float3 vNormal : NORMAL;
//    float3 vTangent : TANGENT;
//    float3 vBinormal : BINORMAL;
//    float2 vUV : TEXCOORD;
//};
//
//struct VTX_OUTPUT
//{
//    float3 vPos : POSITION;
//    float3 vNormal : NORMAL;
//    float3 vTangent : TANGENT;
//    float3 vBinormal : BINORMAL;
//    float2 vUV : TEXCOORD;
//    float2 vUVBasedFull : TEXCOORD1;
//};
//
//
//
//struct PS_OUTPUT
//{
//    float4 vCol_0 : SV_Target0;
//    float4 vCol_1 : SV_Target1; // View Normal 
//    float4 vCol_2 : SV_Target2; // View Position
//};
//
//VTX_OUTPUT VS_Terrain(VTX_INPUT _input)
//{
//    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
//    VTX_OUTPUT output = (VTX_OUTPUT)0.f;
//
//    output.vPos = mul(float4(_input.vPos, 1.f), mtxWorld).xyz;
//    output.vTangent = normalize(mul(float4(_input.vTangent, 0.f), mtxWorld)).xyz;
//    output.vNormal = normalize(mul(float4(_input.vNormal, 0.f), mtxWorld)).xyz;
//    output.vBinormal = normalize(mul(float4(_input.vBinormal, 0.f), mtxWorld)).xyz;
//    output.vUV = _input.vUV;
//    output.vUVBasedFull = float2(_input.vPos.x / g_Vec2_3.x, 1.f - (_input.vPos.z / g_Vec2_3.y));
//
//    return output;
//}
//
//struct PatchTess
//{
//    float arrEdge[3] : SV_TessFactor;
//    float fInner : SV_InsideTessFactor;
//};
//
//
//// Patch 당 실행되는 함수
//PatchTess PatchConstantFunc(InputPatch<VTX_OUTPUT, 3> _input, int _iIdx : SV_PrimitiveID)
//{
//    PatchTess factor = (PatchTess)0.f;
//
//    float3 vStdPos = (_input[0].vPos.xyz + _input[2].vPos.xyz) / 2.f;
//    float fXStep = (_input[1].vPos.x - _input[0].vPos.x) / 2.f;
//    float fZStep = (_input[1].vPos.z - _input[2].vPos.z) / 2.f;
//
//    float3 vXPos = float3(vStdPos.x + fXStep, vStdPos.y, vStdPos.z);
//    float3 vZPos = float3(vStdPos.x, vStdPos.y, vStdPos.z + fZStep);
//
//    float fTess = CalTessLvl(800.f, 6400.f, 16, 1, vStdPos);
//    float fXTess = CalTessLvl(800.f, 6400.f, 16, 1, vXPos);
//    float fZTess = CalTessLvl(800.f, 6400.f, 16, 1, vZPos);
//
//    factor.arrEdge[0] = fXTess;
//    factor.arrEdge[1] = fTess;
//    factor.arrEdge[2] = fZTess;
//    factor.fInner = fTess;
//
//    return factor;
//}
//
//struct HULL_OUTPUT
//{
//    float3 vPos : POSITION;
//    float3 vNormal : NORMAL;
//    float3 vTangent : TANGENT;
//    float3 vBinormal : BINORMAL;
//    float2 vUV : TEXCOORD;
//    float2 vUVBasedFull : TEXCOORD1;
//};
//
//
//[domain("tri")]
//[partitioning("integer")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("PatchConstantFunc")]
//[maxtessfactor(32.0)]
//HULL_OUTPUT HS_Terrain(InputPatch<VTX_OUTPUT, 3> _input, int _iIdx : SV_OutputControlPointID)
//{
//    HULL_OUTPUT output = (HULL_OUTPUT)0.f;
//
//    output.vPos = _input[_iIdx].vPos;
//    output.vNormal = _input[_iIdx].vNormal;
//    output.vTangent = _input[_iIdx].vTangent;
//    output.vBinormal = _input[_iIdx].vBinormal;
//    output.vUV = _input[_iIdx].vUV;
//    output.vUVBasedFull = _input[_iIdx].vUVBasedFull;
//
//    return output;
//}
//
//struct DS_OUTPUT
//{
//    float4 vPos : SV_Position;
//    float4 vViewPos : POSITION;
//    float4 vViewNormal : NORMAL;
//    float4 vViewTangent : TANGENT;
//    float4 vViewBinormal : BINORMAL;
//    float2 vUV : TEXCOORD;
//    float2 vUVBasedFull : TEXCOORD1;
//};
//
//[domain("tri")]
//DS_OUTPUT DS_Terrain(PatchTess tessfactor, OutputPatch<HULL_OUTPUT, 3> _input, float3 _inputUV : SV_DOMAINLOCATION)
//{
//    DS_OUTPUT output = (DS_OUTPUT)0.f;
//
//    HULL_OUTPUT info = (HULL_OUTPUT)0.f;
//
//    info.vPos = (_input[0].vPos * _inputUV.x + _input[1].vPos * _inputUV.y + _input[2].vPos * _inputUV.z);
//    info.vNormal = (_input[0].vNormal * _inputUV.x + _input[1].vNormal * _inputUV.y + _input[2].vNormal * _inputUV.z);
//    info.vBinormal = (_input[0].vBinormal * _inputUV.x + _input[1].vBinormal * _inputUV.y + _input[2].vBinormal * _inputUV.z);
//    info.vTangent = (_input[0].vTangent * _inputUV.x + _input[1].vTangent * _inputUV.y + _input[2].vTangent * _inputUV.z);
//    info.vUV = (_input[0].vUV * _inputUV.x + _input[1].vUV * _inputUV.y + _input[2].vUV * _inputUV.z);
//    info.vUVBasedFull = (_input[0].vUVBasedFull * _inputUV.x + _input[1].vUVBasedFull * _inputUV.y + _input[2].vUVBasedFull * _inputUV.z);
//
//    output.vUV = info.vUV;
//    output.vUVBasedFull = info.vUVBasedFull;
//
//    if (g_bTex0)
//    {
//        float3 vLocalPos = info.vPos;
//
//        float3 vTemp = float3(0.f, 1.f, 0.f);
//        float fYScale = distance(float3(0.f, 0.f, 0.f), mul(float4(vTemp, 0.f), mtxWorld).xyz);
//
//        vTemp = float3(1.f, 0.f, 0.f);
//        float fXScale = distance(float3(0.f, 0.f, 0.f), mul(float4(vTemp, 0.f), mtxWorld).xyz);
//
//
//        float4 vHeight = g_Tex_0.SampleLevel(g_Sample, output.vUVBasedFull, 0);
//        info.vPos.y = vHeight.r * fYScale;
//
//        float4 vRightHeight = g_Tex_0.SampleLevel(g_Sample, float2(output.vUVBasedFull.x + g_Vec2_0.x, output.vUVBasedFull.y), 0) * fYScale;
//        float4 vLeftHeight = g_Tex_0.SampleLevel(g_Sample, float2(output.vUVBasedFull.x - g_Vec2_0.x, output.vUVBasedFull.y), 0) * fYScale;
//
//        float3 vRightPos = vLocalPos;
//        vRightPos.x += fXScale / 16.f;
//        vRightPos.y = vRightHeight.x;
//
//        float3 vLeftPos = vLocalPos;
//        vLeftPos.x -= fXScale / 16.f;
//        vLeftPos.y = vLeftHeight.x;
//
//        info.vBinormal = float3(0.f, 0.f, -1.f);
//        info.vTangent = normalize(vRightPos - vLeftPos);
//        info.vNormal = normalize(cross(info.vTangent, info.vBinormal));
//    }
//
//    output.vPos = mul(float4(info.vPos, 1.f), matVP);
//    output.vViewPos = mul(float4(info.vPos, 1.f), matView);
//    output.vViewNormal = normalize(mul(float4(info.vNormal, 0.f), matView));
//    output.vViewBinormal = normalize(mul(float4(info.vBinormal, 0.f), matView));
//    output.vViewTangent = normalize(mul(float4(info.vTangent, 0.f), matView));
//
//    return output;
//}
//
//PS_OUTPUT PS_Terrain(DS_OUTPUT _input)
//{
//    PS_OUTPUT vRetCol = (PS_OUTPUT)0.f;
//
//    float3 vCol = float3(1.f, 0.f, 1.f);
//
//    if (g_btex_1)
//    {
//        float4 vWeight = g_tex_1.Sample(g_sam_0, _input.vUVBasedFull);
//        vCol = float3(0.f, 0.f, 0.f);
//        //vCol = g_texarr_0.Sample(g_sam_0, float3(_input.vUV, (float) 1.f)).xyz;
//
//
//        for (uint i = 0; i < 4; ++i)
//        {
//            vCol += vWeight[i] * g_texarr_0.Sample(g_sam_0, float3(_input.vUV, (float)i)).xyz;
//        }
//    }
//
//    float3 vNormal = _input.vViewNormal.xyz;
//    if (g_btex_3)
//    {
//        vNormal = g_tex_3.Sample(g_sam_0, _input.vUV).xyz;
//        vNormal = normalize(vNormal * 2.f - 1.f);
//
//        float3x3 matTBN = { _input.vViewTangent.xyz, _input.vViewBinormal.xyz, _input.vViewNormal.xyz };
//        vNormal = normalize(mul(vNormal, matTBN));
//    }
//
//    vRetCol.vCol_0.rgb = vCol.rgb;
//    vRetCol.vCol_1 = float4(vNormal, 0.f);
//    vRetCol.vCol_2 = float4(_input.vViewPos.xyz, 0.f);
//
//    if (g_btex_2)
//    {
//        // Terrain Brush 정보를 PositionTarget Alpha 자리에 넣기
//        float2 vUVLen = (_input.vUVBasedFull - g_vec2_1) / g_vec2_2;
//        float2 vBrushUV = float2(0.5f, 0.5f) - vUVLen;
//        if (0.f <= vBrushUV.x && vBrushUV.x <= 1.f &&
//            0.f <= vBrushUV.y && vBrushUV.y <= 1.f)
//        {
//            vRetCol.vCol_2.a = g_tex_2.Sample(g_sam_0, vBrushUV).a;
//        }
//    }
//
//    return vRetCol;
//}
//
//
//#endif