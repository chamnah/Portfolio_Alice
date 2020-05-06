#include "value.fx"
struct VS_INPUT
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
    float2 vUV : UV;
    uint iInstanceID : SV_InstanceID;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float4 vOutViewPos : POSITION;
    float3 vOutNormal : NORMAL;
    float3 vOutBinormal : BINORMAL;
    float3 vOutTangent : TANGENT;
    float2 vOutTex : TEXCOORD;
};

// Tex 0 : Diffuse
// Tex 1 : Normal
// Tex 2 : Specular
// Tex 3 : Masking
// g_Vec4_0 : Color
VS_OUTPUT VS_Deferred(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

   if(g_bTex7)
        Skinnig(_input.vPos, _input.vTangent, _input.vBiNormal, _input.vNormal, _input.vWeight, _input.vIndicies, 0);

    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vOutViewPos = mul(float4(_input.vPos, 1.f), mtxWV);
    output.vOutColor = _input.vColor;
    output.vOutNormal = normalize(mul(float4(_input.vNormal, 0.f), mtxWV)).xyz;
    output.vOutTex = _input.vTex * g_Vec2_3;

    return output;
}

VS_OUTPUT VS_Deferred_Instance(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    if (g_bTex7)
        Skinnig(_input.vPos, _input.vTangent, _input.vBiNormal, _input.vNormal, _input.vWeight, _input.vIndicies, _input.iRowIdx);

    output.vOutPos = mul(float4(_input.vPos, 1.f), _input.mtxWVP);
    output.vOutViewPos = mul(float4(_input.vPos, 1.f), _input.mtxWV);
    output.vOutColor = _input.vColor;
    output.vOutNormal = normalize(mul(float4(_input.vNormal, 0.f), _input.mtxWV)).xyz;
    output.vOutTex = _input.vTex * _input.vUV;

    return output;
}

struct Def_OUTPUT
{
    float4 vTarget0 : SV_Target0; // Diffuse Color
    float4 vTarget1 : SV_Target1; // Normal
    float4 vTarget2 : SV_Target2; // ViewPos
    float4 vTarget3 : SV_Target3; // Obj Specular
};

Def_OUTPUT PS_Deferred(VS_OUTPUT _input)
{
    Def_OUTPUT output = (Def_OUTPUT) 0.f;

    if (g_bTex0.x != 0)
        output.vTarget0 = (g_Tex_0.Sample(g_Sample, _input.vOutTex)) * g_mtrl;
    else
        output.vTarget0 = g_mtrl; //float4(1.f, 0.f, 1.f, 0.f);//_input.vOutColor;// float4(1.f, 0.f, 1.f, 0.f);

    if (g_bTex3.x != 0)
    {
        output.vTarget0 *= float4(g_Tex_3.Sample(g_Sample, _input.vOutTex).xyz, 0.f);

        if (output.vTarget0.x == 0.f && output.vTarget0.y == 0.f && output.vTarget0.z == 0.f)
            clip(-1);
    }

    if(g_bTex1.x != 0)
    {
        float3 vNormal = g_Tex_1.Sample(g_Sample, _input.vOutTex).rgb;
        vNormal = (vNormal * 2) - 1;
        float3x3 matTBN = { _input.vOutTangent.xyz, _input.vOutBinormal.xyz, _input.vOutNormal.xyz };
        output.vTarget1 = float4(normalize(mul(vNormal, matTBN)), 0.f);
    }
    else
        output.vTarget1 = normalize(float4(_input.vOutNormal, 0.f));

    if (g_bTex2.x != 0)
        output.vTarget3 = float4(g_Tex_2.Sample(g_Sample, _input.vOutTex).xyz, 0.f);
    else
        output.vTarget3 = float4(1.f,1.f,1.f,0.f);
    
    //_input.vOutViewPos.a = 0.f;
    output.vTarget2 = _input.vOutViewPos;

    return output;
}

VS_OUTPUT VS_Merge(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWorld); /*float4(_input.vPos,0.5f);*/ // 스케일을 곱해서 만들기 위해서
    output.vOutTex = _input.vTex;

    return output;

}

float4 PS_Merge(VS_OUTPUT _input) : SV_Target
{
    float4 vColor = g_Tex_0.Sample(g_Sample, _input.vOutTex);
    
    float4 vDiffuse = g_Tex_3.Sample(g_Sample, _input.vOutTex);
    float4 vSpecluar = g_Tex_4.Sample(g_Sample, _input.vOutTex);

    if (g_int_0 == 0)
        return vColor;
    else if (g_int_0 == 1)
    {
        float4 vNormal = g_Tex_1.Sample(g_Sample, _input.vOutTex);
        return vNormal;
    }
    else if (g_int_0 == 2)
    {
        return g_Tex_2.Sample(g_Sample, _input.vOutTex);
        //float4 vPosition = g_Tex_2.Sample(g_Sample, _input.vOutTex);
        //return float4(0.2f, 1.f, 0.2f, 1.f) * vPosition.a;
    }
    else if (g_int_0 == 3)
        return vDiffuse;
    else if (g_int_0 == 4)
        return vSpecluar;
    else if (g_int_0 == 5)
    {
        float vShadow = g_Tex_5.Sample(g_Sample, _input.vOutTex).r;
        return float4(vShadow * 4, 0.f, 0.f, 0.f);
    }
    else
    {
        float4 vBrush = float4(0.2f, 1.f, 0.2f, 0.f) * g_Tex_2.Sample(g_Sample, _input.vOutTex).a;
        float4 vMerge = vColor * vDiffuse + vSpecluar;// + vBrush;
        return vMerge;
    }
    //else
    //if (g_int.x == 0)
    //{
    //    vMerge.b += 0.2f;
    //    return vMerge;
    //}
}

VS_OUTPUT VS_DirLight(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWorld); /*float4(_input.vPos,0.5f);*/ // 스케일을 곱해서 만들기 위해서
    output.vOutTex = _input.vTex;

    return output;
}

VS_OUTPUT VS_PointLight(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    output.vOutPos = mul(float4(_input.vPos, 1.f), mtxWorld); /*float4(_input.vPos,0.5f);*/ // 스케일을 곱해서 만들기 위해서
    output.vOutPos = mul(output.vOutPos, mtxView);     
    output.vOutPos = mul(output.vOutPos, mtxProj);
    output.vOutTex = _input.vTex;

    return output;
}

struct LightTarget
{
    float4 vDiffuse : SV_Target0;
    float4 vSpecular : SV_Target1;
};

// Tex 0 = Normal
// Tex 1 = Position
// Tex 2 = ShadowMap
// Tex 3 = Specular
// Mtx 0 = MainCam Inv View
// Mtx 1 = Light View * Proj

LightTarget PS_DirLight(VS_OUTPUT _input)
{
    float3 vNormal = g_Tex_0.Sample(g_Sample, _input.vOutTex).xyz;
    vNormal = normalize(vNormal);

    float4 vViewPos = g_Tex_1.Sample(g_Sample, _input.vOutTex);

    if(vViewPos.z == 0.f)
        clip(-1);

    float fLightPow = 0.f;
    LightColor Color = CalculateLight(vNormal, vViewPos.xyz, g_Info[0], fLightPow);

    if(g_bTex3.x != 0)
    {
        float4 vSpec = g_Tex_3.Sample(g_Sample, _input.vOutTex);
        Color.vSpec *= vSpec;
    }

    if (fLightPow != 0.f)
    {
        float4 vWorldPos = mul(vViewPos, g_Mtx0);
        float4 vShadowPos = mul(vWorldPos, g_Mtx1);
        // projection 좌표계로 변환후에 그것을 UV 좌표계로 변환
        float2 vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

        if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
        {
            float fDepth = vShadowPos.z / vShadowPos.w;
            float vShadowMap = g_Tex_2.Sample(g_Sample, vShadowUV).r;

            if (fDepth > vShadowMap + 0.00005)
            {
                Color.vSpec = (float4) 0.f;
                Color.vDiff *= (float4) 0.1f;
            }
        }
    }

    float3 vEye = -normalize(vViewPos.xyz);
    float4 vLim = float4(0.f,0.f,0.f,0.f);
    if (g_int_0 == 1)
    {
        vLim = smoothstep(0.1f, 1.f, 1 - max(0, dot(vNormal, vEye)));
    }
    
    LightTarget Target;
    Target.vDiffuse = Color.vDiff + Color.vAmb + vLim;
    Target.vSpecular = Color.vSpec;

    return Target;
}

LightTarget PS_PointLight(VS_OUTPUT _input)
{
    float2 vUV = float2(_input.vOutPos.x / g_fWidth, _input.vOutPos.y / g_fHeight);

    float3 vNormal = g_Tex_0.Sample(g_Sample, vUV).xyz;
    vNormal = normalize(vNormal);

    float4 vViewPos = g_Tex_1.Sample(g_Sample, vUV);

    float4 vWorldPos = mul(float4(vViewPos.xyz, 1.f), g_Mtx0);
    float4 vLocalPos = mul(vWorldPos, mtxInvWorld);

    if (1.f < distance(vLocalPos.xyz, float3(0.f, 0.f, 0.f)))
        clip(-1);

    if (vViewPos.z == 0.f)
        clip(-1);

    float fLightPow = 0.f;
    LightColor Color = CalculateLight(vNormal, vViewPos.xyz, g_Info[0], fLightPow);

    if (fLightPow != 0)
    {
        float4 vWorld = mul(vViewPos, g_Mtx0);
        float4 vShadowPos = mul(vWorld, g_Mtx1);
        float2 vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

        if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
        {
            float fDepth = vShadowPos.z / vShadowPos.w;
            float vShadowMap = g_Tex_2.Sample(g_Sample, vShadowUV).r;

            if (fDepth > vShadowMap + 0.0005)
            {
                Color.vSpec = (float4) 0.f;
                Color.vDiff *= (float4) 0.1f;
                Color.vAmb = 0.f;

            }
        }

       //vShadowPos = mul(vWorldPos, g_Mtx2);
       //vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

       // if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
       // {
       //     float fDepth = vShadowPos.z / vShadowPos.w;
       //     float vShadowMap = g_Tex_3.Sample(g_Sample, vShadowUV).r;

       //     if (fDepth > vShadowMap + 0.00005)
       //     {
       //         Color.vSpec = (float4) 0.f;
       //         Color.vDiff *= (float4) 0.1f;
       //     }
       // }

       // vShadowPos = mul(vWorldPos, g_Mtx3);
       // vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

       // if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
       // {
       //     float fDepth = vShadowPos.z / vShadowPos.w;
       //     float vShadowMap = g_Tex_4.Sample(g_Sample, vShadowUV).r;

       //     if (fDepth > vShadowMap + 0.00005)
       //     {
       //         Color.vSpec = (float4) 0.f;
       //         Color.vDiff *= (float4) 0.1f;
       //     }
       // }

       // vShadowPos = mul(vWorldPos, g_Mtx4);
       // vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

       // if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
       // {
       //     float fDepth = vShadowPos.z / vShadowPos.w;
       //     float vShadowMap = g_Tex_5.Sample(g_Sample, vShadowUV).r;

       //     if (fDepth > vShadowMap + 0.00005)
       //     {
       //         Color.vSpec = (float4) 0.f;
       //         Color.vDiff *= (float4) 0.1f;
       //     }
       // }

       // vShadowPos = mul(vWorldPos, g_Mtx5);
       // vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

       // if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
       // {
       //     float fDepth = vShadowPos.z / vShadowPos.w;
       //     float vShadowMap = g_Tex_6.Sample(g_Sample, vShadowUV).r;

       //     if (fDepth > vShadowMap + 0.00005)
       //     {
       //         Color.vSpec = (float4) 0.f;
       //         Color.vDiff *= (float4) 0.1f;
       //     }
       // }

       // vShadowPos = mul(vWorldPos, g_Mtx6);
       // vShadowUV = float2(vShadowPos.x / vShadowPos.w * 0.5f + 0.5f, vShadowPos.y / vShadowPos.w * -0.5f + 0.5f);

       // if (0.01 < vShadowUV.x && 0.01 < vShadowUV.y && 0.99 > vShadowUV.x && 0.99 > vShadowUV.y)
       // {
       //     float fDepth = vShadowPos.z / vShadowPos.w;
       //     float vShadowMap = g_Tex_7.Sample(g_Sample, vShadowUV).r;

       //     if (fDepth > vShadowMap + 0.00005)
       //     {
       //         Color.vSpec = (float4) 0.f;
       //         Color.vDiff *= (float4) 0.1f;
       //     }
       // }
   }

    LightTarget Target;
    Target.vDiffuse = Color.vDiff + Color.vAmb;
    Target.vSpecular = Color.vSpec;

    return Target;
}

float4 VS_VolumeMesh(float3 _Pos : POSITION) : SV_Position
{
    return mul(float4(_Pos, 1.f), mtxWVP);
}

VS_OUTPUT VS_Vision(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT)0.f;

    output.vOutPos = mul(float4(_input.vPos,1.f),mtxWorld);
    output.vOutTex = _input.vTex;

    return output;
}

float4 PS_Vision(VS_OUTPUT _input) : SV_Target0
{
    float4 output = (float4) 0.f;

    output = g_Tex_0.Sample(g_Sample,_input.vOutTex);
    
    return output;
}

struct SHADOW_OUT
{
    float4 vPos : SV_Position;
    float4 vProj : POSITION;
};

SHADOW_OUT VS_Shadow(VS_INPUT _input)
{
    SHADOW_OUT output = (SHADOW_OUT)0.f;

    if (g_bTex7)
        Skinnig(_input.vPos, _input.vTangent, _input.vBiNormal, _input.vNormal, _input.vWeight, _input.vIndicies, 0);

    output.vPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vProj = output.vPos;
    return output;
}

struct SHADOW_TARGET
{
    float4 vTarget0 : SV_Target0;
};

SHADOW_TARGET PS_Shadow(SHADOW_OUT _input)
{
    SHADOW_TARGET Target;
    Target.vTarget0 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    
    return Target;
}

struct POINTSHADOW_OUT
{
    float4 vPos : SV_Position;
    float4 vProj : POSITION;
};

POINTSHADOW_OUT VS_PointShadow(VS_INPUT _input)
{
    POINTSHADOW_OUT output = (POINTSHADOW_OUT) 0.f;

    if (g_bTex7)
        Skinnig(_input.vPos, _input.vTangent, _input.vBiNormal, _input.vNormal, _input.vWeight, _input.vIndicies, 0);

    output.vPos = mul(float4(_input.vPos, 1.f), mtxWVP);
    output.vProj = output.vPos;
    return output;
}

struct POINTSHADOW_TARGET
{
    float4 vTarget0 : SV_Target0;
    //float4 vTarget1 : SV_Target1;
    //float4 vTarget2 : SV_Target2;
    //float4 vTarget3 : SV_Target3;
    //float4 vTarget4 : SV_Target4;
    //float4 vTarget5 : SV_Target5;
};

SHADOW_TARGET PS_PointShadow(POINTSHADOW_OUT _input)
{
    SHADOW_TARGET Target;
    
   // if (g_iTarget.x == 0)
   Target.vTarget0 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    //if (g_iTarget.x == 1)
    //    Target.vTarget1 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    //if (g_iTarget.x == 2)
    //    Target.vTarget2 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    //if (g_iTarget.x == 3)
    //    Target.vTarget3 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    //if (g_iTarget.x == 4)
    //    Target.vTarget4 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    //if (g_iTarget.x == 5)
    //    Target.vTarget5 = float4(_input.vProj.z / _input.vProj.w, 0.f, 0.f, 0.f);
    return Target;
}

VS_OUTPUT VS_Masking(VS_INPUT _input)
{
}

float4 PS_Masking(VS_OUTPUT _input) : SV_Target0
{
    float4 vColor = float4(0.f,0.f,0.f,0.f);
    return vColor;
}

struct Geo_IN
{
    float4 vPos : SV_Position;
};


// g_Tex_0 : Position Texture
// g_Tex_1 : Trail Texture
Geo_IN VS_Trail(float3 vPos : POSITION)
{
    Geo_IN trail;
    trail.vPos = float4(vPos,0.f);
    return trail;
}

[maxvertexcount(40)]
void GS_Trail(point Geo_IN _In[1], inout TriangleStream<VS_OUTPUT> _input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0.f;

    float4 vPos = (float4) 0.f;

    for (int i = 0; i < 20; ++i)
    {
        vPos = g_Tex_0.Load(int3(i, 1, 0));
        if (vPos.w <= 1.f)
            return;

        output.vOutPos = mul(float4(vPos.xyz, 1.f), mtxVP);
        output.vOutTex = float2((float)i / 19.f, 0.f);
        _input.Append(output);
      
        vPos = g_Tex_0.Load(int3(i, 0, 0));
        output.vOutPos = mul(float4(vPos.xyz, 1.f), mtxVP);
        output.vOutTex = float2((float) i / 19.f, 1.f);
        _input.Append(output);
    }
}

float4 PS_Trail(VS_OUTPUT _input) : SV_Target0
{
    float4 vColor = g_Tex_1.Sample(g_Sample, _input.vOutTex) * g_mtrl;
    return vColor;
}



// 지오메트리 순서에 관한 낙서
//output.vOutPos = mul(float4(200.f + _In[0].vPos.x, -100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    //_input.Append(output);
    //output.vOutPos = mul(float4(200.f + _In[0].vPos.x, 100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    //_input.Append(output);

    //output.vOutPos = mul(float4(100.f + _In[0].vPos.x, -100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    //_input.Append(output);

    //output.vOutPos = mul(float4(100.f + _In[0].vPos.x, 100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    //_input.Append(output);

    //output.vOutPos = mul(float4(-100.f + _In[0].vPos.x, -100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    //_input.Append(output);

    //output.vOutPos = mul(float4(-100.f + _In[0].vPos.x, 100.f + _In[0].vPos.y, 10.f + _In[0].vPos.z, 1.f), mtxVP);
    //output.vOutColor = float4(0.f, 1.f, 0.f, 1.f);
    //_input.Append(output);