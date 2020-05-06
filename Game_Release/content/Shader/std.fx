#include "value.fx"
struct VS_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vTex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutTex : TEXCOORD;
};

struct GeoOut
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutTex : TEXCOORD;
};

VS_OUTPUT VS_Color(VS_INPUT _Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), mtxWVP);
	output.vOutColor = _Input.vColor;

	return output;
}

float4 PS_Color(GeoOut _Input) : SV_Target
{
    return g_Vec4_0;
}

// 흑백 if (g_int[0])
    //{
    //    float fColor = float(texColor.r + texColor.g + texColor.b) / 3.f;
    //    texColor.xyz = float3(fColor,fColor,fColor);
    //}

VS_OUTPUT VS_Std_2D(VS_INPUT _Input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), mtxWVP);
    output.vOutPos.x += g_fOffset.x;
    output.vOutPos.y += g_fOffset.y;
    output.vOutTex = _Input.vTex;

    return output;
}

float4 PS_Std_2D(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4)0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float_0 * _Input.vOutTex.x) + _Input.vOutTex.x;
        fUV.y = (g_float_1 * _Input.vOutTex.y) + _Input.vOutTex.y;

        if (fUV.x >= 1.f)
            fUV.x = 1.f;

        if (fUV.y >= 1.f)
            fUV.y = 1.f;
            texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor *= g_Vec4_0;
    return texColor;
}

float4 PS_Std_Spectrum(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float_0 * _Input.vOutTex.x) + _Input.vOutTex.x;
        fUV.y = (g_float_1 * _Input.vOutTex.y) + _Input.vOutTex.y;

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor.a = 0.8f;

    return texColor;
}


float4 PS_Std_Color(GeoOut _Input) : SV_Target
{
    float4 texColor = (float4) 0.f;

    if (g_bUse)
    {
        float2 fUV = (float2) 0.f;
        fUV.x = g_fLT.x + (g_fSize.x * _Input.vOutTex.x);
        fUV.y = g_fLT.y + (g_fSize.y * _Input.vOutTex.y);

        texColor = g_Anim_Tex.Sample(g_Sample, fUV);
    }
    else
    {
        float2 fUV = (float2) 0.f;
        fUV.x = (g_float_0 * _Input.vOutTex.x) + _Input.vOutTex.x;
        fUV.y = (g_float_1 * _Input.vOutTex.y) + _Input.vOutTex.y;

        texColor = g_Tex_0.Sample(g_Sample, fUV);
    }

    if (texColor.a <= 0.1f)
        clip(-1);

    texColor = g_Vec4_0;

    return texColor;
}

VS_OUTPUT VS_Coll(VS_INPUT _Input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
	
    output.vOutPos = mul(float4(_Input.vPos, 1.f), mtxWVP);
    output.vOutColor = _Input.vColor;

    return output;
}

float4 PS_Coll(VS_OUTPUT _Input) : SV_Target
{
    if (g_int_0)
        _Input.vOutColor = float4(1,0,0,0);
    else
        _Input.vOutColor = float4(0, 0, 1, 0);

    return _Input.vOutColor;
}

struct Geo_IN
{
    float4 vPos : SV_Position;
};

struct GS_Bill
{
    float4 vOutPos : SV_Position;
    float2 vOutTex : TEXCOORD;
};

Geo_IN VS_Billboard(float3 _vPos : POSITION)
{
    Geo_IN bill;
    bill.vPos = float4(_vPos, 0.f);
    return bill;
}

//g_Vec4_0 : camera up dir
//g_Vec4_1 : camera pos

[maxvertexcount(4)]
void GS_Billboard(point Geo_IN _In[1], inout TriangleStream<GS_Bill> _input)
{
    GS_Bill bill = (GS_Bill) 0.f;
    float4 vPos = float4(0.f,0.f,0.f,1.f);
    vPos = mul(vPos, mtxWorld);

    float3 vUp = normalize(g_Vec4_0.xyz);
    float3 vFront = g_Vec4_1.xyz - vPos.xyz;
    vFront = normalize(vFront);
    float3 vRight = cross(vUp, vFront);
    vRight = normalize(vRight);
    bill.vOutPos.xyz = (0.5f * vRight) - (0.5f * vUp);
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxWVP);
    bill.vOutTex = float2(0.f, 1.f);
    _input.Append(bill);

    bill.vOutPos.xyz = (0.5f * vRight) + (0.5f * vUp); 
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxWVP);
    bill.vOutTex = float2(0.f, 0.f);
    _input.Append(bill);

    bill.vOutPos.xyz = -(0.5f * vRight) - (0.5f * vUp); 
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxWVP);
    bill.vOutTex = float2(1.f, 1.f);
    _input.Append(bill);

    bill.vOutPos.xyz = -(0.5f * vRight) + (0.5f * vUp); 
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxWVP);
    bill.vOutTex = float2(1.f, 0.f);
    _input.Append(bill);
}

//float4 vRect[4];
    //vRect[0] = float4(-0.5f, -0.5f, 0.f, 1.f);
    //vRect[1] = float4(-0.5f, 0.5f, 0.f, 1.f);
    //vRect[2] = float4(0.5f, -0.5f, 0.f, 1.f);
    //vRect[3] = float4(0.5f, 0.5f, 0.f, 1.f);

    //for (int i = 0; i < 4; ++i)
    //{
    //    bill.vOutPos = mul(vRect[i], mtxWVP);
    //    _input.Append(bill);
    //}

float4 PS_Billboard(GS_Bill _input) : SV_Target0
{
    //return float4(1.f, 0.f, 0.f, 0.f);
    return g_Tex_0.Sample(g_Sample, _input.vOutTex);
}



struct Part_IN
{
    float3 vPos : POSITION;
    uint iVtxID : SV_VertexID;
};

struct Part_OUT
{
    float3 vPos : POSITION;
    uint iVtxID : VERTEXID;
};

// g_Tex1 : PosTexture
Part_OUT VS_TrailParticle(Part_IN _vPos)
{
    Part_OUT bill;
    bill.vPos = _vPos.vPos;
    bill.iVtxID = _vPos.iVtxID;
    return bill;
}

//g_Vec4_0 : camera up dir
//g_Vec4_1 : camera pos
// g_Vec2_0 : Scale

[maxvertexcount(4)]
void GS_TrailParticle(point Part_OUT _In[1], inout TriangleStream<GS_Bill> _input)
{
    float4 vTex = g_Tex_1.Load(int3(_In[0].iVtxID, 0, 0));
    if (vTex.w <= 1.f)
        return;

    GS_Bill bill = (GS_Bill) 0.f;
    float4 vPos = float4(vTex.xyz, 1.f);

    float3 vUp = normalize(g_Vec4_0.xyz);
    float3 vFront = g_Vec4_1.xyz - vPos.xyz;
    vFront = normalize(vFront);
    float3 vRight = cross(vUp, vFront);
    vRight = normalize(vRight);
    bill.vOutPos.xyz = (0.5f * vRight * g_Vec2_0.x) - (0.5f * vUp * g_Vec2_0.y) + vPos.xyz; //float3(-0.5f, -0.5f, 0.f); ;
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxVP);
    bill.vOutTex = float2(0.f, 1.f);
    _input.Append(bill);

    bill.vOutPos.xyz = (0.5f * vRight * g_Vec2_0.x) + (0.5f * vUp * g_Vec2_0.y) + vPos.xyz; // float3(-0.5f, 0.5f, 0.f);
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxVP);
    bill.vOutTex = float2(0.f, 0.f);
    _input.Append(bill);

    bill.vOutPos.xyz = -(0.5f * vRight * g_Vec2_0.x) - (0.5f * vUp * g_Vec2_0.y) + vPos.xyz; //float3(0.5f, -0.5f, 0.f);
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxVP);
    bill.vOutTex = float2(1.f, 1.f);
    _input.Append(bill);

    bill.vOutPos.xyz = -(0.5f * vRight * g_Vec2_0.x) + (0.5f * vUp * g_Vec2_0.y) + vPos.xyz; // float3(0.5f, 0.5f, 0.f);
    bill.vOutPos = mul(float4(bill.vOutPos.xyz, 1.f), mtxVP);
    bill.vOutTex = float2(1.f, 0.f);
    _input.Append(bill);
}

float4 PS_TrailParticle(GS_Bill _input) : SV_Target0
{
    //return float4(1.f, 0.f, 0.f, 0.f);
    float4 vColor = g_Tex_0.Sample(g_Sample, _input.vOutTex) * g_mtrl;
    return vColor;
}

VS_OUTPUT VS_Decal(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vOutTex = _input.vTex;

    return output;
}

//g_Tex_0 : Position Tex
float4 PS_Decal(VS_OUTPUT _input) : SV_Target0
{
    float2 vUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);
    float4 vViewPos = g_Tex_0.Sample(g_Sample, vUV);

    float4 vWorldPos = mul(float4(vViewPos.xyz, 1.f), g_Mtx0);
    float4 vLocalPos = mul(vWorldPos, mtxInvWorld);

    if (vLocalPos.x <= -0.501f || vLocalPos.y <= -0.501f || vLocalPos.z <= -0.501f
        || vLocalPos.x >= 0.501f || vLocalPos.y >= 0.501f || vLocalPos.z >= 0.501f)
            clip(-1);

    if (vViewPos.z == 0.f)
        clip(-1);

    float2 vTexUV = float2(vLocalPos.x + 0.5f, -(vLocalPos.y - 0.5f));

    float4 vColor = g_Tex_1.Sample(g_Sample, vTexUV);
    return vColor;
}

VS_OUTPUT VS_Forward(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vOutTex = _input.vTex * g_Vec2_3;;

    return output;
}

struct VS_INST_INPUT
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float3 vNormal : NORMAL;
    float3 vBiNormal : BINORMAL;
    float3 vTangent : TANGENT;
    float4 vWeight : WEIGHT;
    float4 vIndicies : INDICIES;
    float2 vTex : TEXCOORD;
    row_major matrix mtxWorld : WORLD;
    row_major matrix mtxWV : WV;
    row_major matrix mtxVP : VP;
    row_major matrix mtxWVP : WVP;
    int iRowIdx : ROWINDEX;
    uint iInstanceID : SV_InstanceID;
};

VS_OUTPUT VS_ForwardInst(VS_INST_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
    output.vOutPos = mul(float4(_input.vPos, 1.f), _input.mtxWVP);
    output.vOutTex = _input.vTex * g_Vec2_3;;
    return output;
}

float4 PS_Forward(VS_OUTPUT _input) : SV_Target0
{
    float4 vColor = float4(0.f,0.f,0.f,0.f);
    if (g_bTex0.x != 0)
        vColor = (g_Tex_0.Sample(g_Sample, _input.vOutTex)) * g_mtrl;
    else
        vColor = g_mtrl; //float4(1.f, 0.f, 1.f, 0.f);//_input.vOutColor;// float4(1.f, 0.f, 1.f, 0.f);

    return vColor;
}


struct VS_POST_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_POST_OUTPUT
{
    float4 vOutPos : SV_Position;
    float2 vCenterUV : POSITION;
    float fMaxDistUV : POSITION2;
    float2 vOutUV : TEXCOORD;
};

// g_Tex0 : OutputTexture
VS_POST_OUTPUT VS_PostEffect(VS_POST_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_POST_OUTPUT output = (VS_POST_OUTPUT) 0.f;
   
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

VS_POST_OUTPUT VS_PostEffectInst(VS_INST_INPUT _input)
{
    VS_POST_OUTPUT output = (VS_POST_OUTPUT) 0.f;
    output.vOutPos = mul(float4(_input.vPos, 1.f), _input.mtxWVP);
  //  output.vOutTex = _input.vTex;

    return output;
}

float4 PS_PostEffect(VS_POST_OUTPUT _input) : SV_Target0
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
    //float4 vSunColor = g_tex_1.Sample(g_sam_0, vScreenUV);

    return vTargetColor * 2.f;
}

