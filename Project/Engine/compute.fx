#include "value.fx"

// SV_GroupID            uint3 �׷��� ID ��
// SV_GroupThreadID      uint3 �׷� �ȿ��� Thread ID ��
// SV_DispatchThreadID   uint3 ��� �׷��� ������ ��ü Thread ID ��
// SV_GroupIndex         uint �׷� �ȿ��� �������� �ε��� ��
// g_Tex_0               �귯�� �ؽ���
// g_RWTex_0             ���̸� �ؽ���
// g_Vec2_0              �귯��UV ��ǥ (�ͷ��� ��ü�� �ϳ��� 0~1���̷� �Ǵ��� UV)
// g_Vec2_1              �귯��UV ������ (�ͷ��� ��ü�� �ϳ��� 0~1���̷� �Ǵ��� UV)
// g_Vec2_2              ���̸� �ؽ��� ������
[numthreads(512, 1, 1)]
void CS_Brush(uint3 iID : SV_DispatchThreadID)
{
    if (g_bTex0.x == 0 || g_bRTex0.x == 0)
        return;

    float2 vBrushPos = g_Vec2_0 * g_Vec2_2; // �ͷ��� ������ ��ŭ �ø���. ���� �ȼ� ��ǥ�� ����
    float2 vBrushSize = g_Vec2_1 * g_Vec2_2;
    float2 vUV = (((float2)iID.xy - vBrushPos) / vBrushSize) + float2(0.5f,0.5f);

    if (vUV.x > 0.f && vUV.x < 1.f && vUV.y > 0.f && vUV.y < 1.f)
    {
        float4 vColor = g_Tex_0.SampleLevel(g_Sample, vUV, 0.f);
        g_RWTex_0[iID.xy] += float4(1.f, 1.f, 1.f, 1.f)  * vColor.a * g_fDT;
    }
}

//float2 vBrushUVPos = g_Vec2_0;
//float2 vBrushUVSize = float2(0.1f, 0.1f);
//float2 vBrushSize = float2(vBrushUVSize.x * g_float.x, vBrushUVSize.y * g_float.y);
//float2 vBrushLP = vBrushUVPos - (vBrushUVSize * 0.5f);
//vBrushLP.x *= g_float.x;
//vBrushLP.y *= g_float.y;
    
//float2 vUV = (iID.xy - vBrushLP) / vBrushSize;

// g_Vec2_0 : Terrain Face Count X,Y
// g_Vec2_1 : Thread Group ���� x,y ���⼭�� z�� ���� �ʴ´�.
// g_vec4_0 : Mouse Pos (�ͷ����� ���� ��ǥ��� �̵��� ��ǥ)
// g_vec4_1 : Mouse Picking Dir 0(�ͷ����� ���� ��ǥ��� �̵��� ����)
// g_RWTex_0 : cpu�� ������ �뵵
[numthreads(512, 1, 1)]
void CS_Picking(uint3 iID : SV_DispatchThreadID)
{
    if (!g_bRTex0.x)
        return;

    uint ThreadID = (512 * 1 * iID.z) + (512 * iID.y) + iID.x;///*(uint(g_Vec2_1.x * 512) * iID.y) +*/ iID.x; // ���� ThreadID ��ü�� ���ٷ� �f���� ���° ���� ���
    uint Row = ThreadID / uint(g_Vec2_0.x * 2); // �ͷ��ο����� ��
    uint Col = ThreadID % uint(g_Vec2_0.x * 2); // �ͷ��ο����� ��

    if ((float)ThreadID > g_Vec2_0.x * 2 * g_Vec2_0.y)
        return;

    float3 vTriPos[3];
    
    if(Col % 2 == 0)
    {
        vTriPos[0] = float3((Col / 2) + 1, 0.f, Row);
        vTriPos[1] = float3((Col / 2) , 0.f, Row);
        vTriPos[2] = float3((Col / 2) , 0.f, Row + 1);
    }
    else
    {
        vTriPos[0] = float3((Col / 2), 0.f, Row + 1);
        vTriPos[1] = float3((Col / 2) + 1, 0.f, Row + 1);
        vTriPos[2] = float3((Col / 2) + 1, 0.f, Row);
    }

    if (IntersectsLay(vTriPos, g_Vec4_0.xyz, g_Vec4_1.xyz))
    {
        float3 vPos = GetIntersectPlanetoLine(vTriPos, g_Vec4_0.xyz, g_Vec4_1.xyz);
        g_RWTex_0[uint2(0, 0)] = float4(vPos,0.f);
    }
}

[numthreads(32, 32, 1)]
void CS_TexClear(uint3 _iID : SV_DispatchThreadID)
{
    if (g_bRTex0.x == 0)
        return;

    if (_iID.x >= g_int_0 || _iID.y >= g_int_1);
        return;
    
    g_RWTex_0[_iID.xy] = g_Vec4_0;
}

// g_Tex_0 : Brush Texture
// g_RWTex_0 : Weight Texture 
// g_int[0] : ���° ����ġ ���� ����������
// g_Vec2_0 : BrushUVPos (��ü UV ����)
// g_Vec2_1 : BrushUVScale (��ü UV ����)
[numthreads(512,1,1)]
void CS_Weight(uint3 _iID : SV_DispatchThreadID)
{
    if (g_bTex0 == 0)
        return;
    float2 vBrushPos = g_Vec2_0 * g_Vec2_2; // �ͷ��� ������ ��ŭ �ø���. ���� �ȼ� ��ǥ�� ����
    float2 vBrushSize = g_Vec2_1 * g_Vec2_2;
    float2 vUV = (((float2)_iID.xy - vBrushPos) / vBrushSize) + float2(0.5f,0.5f);

    if (0.f > vUV.x || 0.f > vUV.y || 1.f < vUV.x || 1.f < vUV.y)
        return;


    float4 vBrushColor = g_Tex_0.SampleLevel(g_Sample, vUV,0.f);
    float4 vWeight = g_RWTex_0[_iID.xy];
    float fAdd = vBrushColor.a * g_fDT * 3;

    if (0 == g_int_0)
    {
        if (vWeight[0] + fAdd >= 1.f)
        {
            vWeight = float4(0.f, 0.f, 0.f, 0.f);
            vWeight[0] = 1.f;
        }
        else
        {
            float fAcc = 1.f - vWeight[0];
            vWeight[0] += fAdd;
            vWeight[1] -= fAdd * (vWeight[1] / fAcc);
            vWeight[2] -= fAdd * (vWeight[2] / fAcc);
            vWeight[3] -= fAdd * (vWeight[3] / fAcc);
        }
    }
    else if (1 == g_int_0)
    {
        if (vWeight[1] + fAdd >= 1.f)
        {
            vWeight = float4(0.f, 0.f, 0.f, 0.f);
            vWeight[1] = 1.f;
        }
        else
        {
            float fAcc = 1.f - vWeight[1];
            vWeight[0] -= fAdd * (vWeight[0] / fAcc);
            vWeight[1] += fAdd;
            vWeight[2] -= fAdd * (vWeight[2] / fAcc);
            vWeight[3] -= fAdd * (vWeight[3] / fAcc);
        }
    }
    else if (2 == g_int_2)
    {
        if (vWeight[2] + fAdd >= 1.f)
        {
            vWeight = float4(0.f, 0.f, 0.f, 0.f);
            vWeight[2] = 1.f;
        }
        else
        {
            float fAcc = 1.f - vWeight[2];
            vWeight[0] -= fAdd * (vWeight[0] / fAcc);
            vWeight[1] -= fAdd * (vWeight[1] / fAcc);
            vWeight[2] += fAdd;
            vWeight[3] -= fAdd * (vWeight[3] / fAcc);
        }
    }
    else if (3 == g_int_3)
    {
        if (vWeight[3] + fAdd >= 1.f)
        {
            vWeight = float4(0.f, 0.f, 0.f, 0.f);
            vWeight[3] = 1.f;
        }
        else
        {
            float fAcc = 1.f - vWeight[3];
            vWeight[0] -= fAdd * (vWeight[0] / fAcc);
            vWeight[1] -= fAdd * (vWeight[1] / fAcc);
            vWeight[2] -= fAdd * (vWeight[2] / fAcc);
            vWeight[3] += fAdd;
        }
    }

   // vWeight = float4(1.f, 0.f, 0.f, 0.f);
    g_RWTex_0[_iID.xy] = vWeight;
}

//g_Tex_0 : Pos,Age Texture
//g_RWTex_0 : DrawTexture
//g_int_0 : Ʈ���� ���� 0�̸� Ʈ���� ó�� ����, �ƴϸ� Ʈ���� �߰�
//g_int_1 : ���Ӱ� push �Ǵ��� ���θ� �ۿ��� üũ�� �ؼ� ������.

[numthreads(20, 1, 1)]
void CS_Trail(uint3 _iID : SV_DispatchThreadID)
{
    // ���̴��� ó���̸� ��ǥ�� DrawTexture�� �ִ´�.
    if (_iID.x == 0)
    {
        g_RWTex_0[int2(0, 0)] = g_Vec4_0;
        g_RWTex_0[int2(0, 1)] = g_Vec4_1;
        
        if (g_int_1 == 0) // Ǫ���ȵ�
            return;
    }
    float4 vPosUp = g_Tex_0.Load(int3(_iID.x, 0, 0));
    float4 vPosDown = g_Tex_0.Load(int3(_iID.x, 1, 0));

    vPosUp.w -= g_fDT;
    vPosDown.w -= g_fDT;

    // push ��
    if (g_int_1 == 1)
    {
        if (_iID.x + 1 >= 20)
            return;
        g_RWTex_0[int2(_iID.x + 1, 0)] = vPosUp;
        g_RWTex_0[int2(_iID.x + 1, 1)] = vPosDown;
    }
    else
    {
        g_RWTex_0[int2(_iID.x, 0)] = vPosUp;
        g_RWTex_0[int2(_iID.x, 1)] = vPosDown;
    }
}

//if (g_RWTex_0[int2(1, 0)].w <= 1.f)
        //{
        //    g_RWTex_0[int2(1, 0)] = g_Vec4_0;
        //    g_RWTex_0[int2(1, 1)] = g_Vec4_1;
        //}

//g_Tex_0 : Pos,Age Texture
//g_RWTex_0 : DrawTexture
//g_int_0 : ���Ӱ� push �Ǵ��� ���θ� �ۿ��� üũ�� �ؼ� ������.
// g_Vec4_0 : ���� Ǫ���� ��
[numthreads(20, 1, 1)]
void CS_Particle(uint3 _iID : SV_DispatchThreadID)
{
    float4 vPos = g_Tex_0.Load(int3(_iID.x, 0, 0));
    vPos.w -= g_fDT;

    if (g_int_0 == 1)
    {
        if (_iID.x == 0)
            g_RWTex_0[int2(0, 0)] = g_Vec4_0;

        if (_iID.x + 1 >= 20)
            return;
        g_RWTex_0[int2(_iID.x + 1, 0)] = vPos;
    }
    else
    {
        g_RWTex_0[int2(_iID.x, 0)] = vPos;
    }

}

cbuffer cbSettings
{
    static float gWeights[11] =
    {
        0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f,
    };
};


cbuffer cbFixed
{
    static const int gBlurRadius = 5;
};

#define N 256
groupshared float4 gCache[N + 10];

[numthreads(N, 1, 1)]
void CS_HorBlur(int3 groupThreadID : SV_GroupThreadID, // �׷�� ��ġ
               int3 dispatchThreadID : SV_DispatchThreadID) // ��� �����带 ��Ʋ�� ��ġ
{
    if (groupThreadID.x < gBlurRadius) // �׷�� ��ġ x�� ������ �ݰ溸�� �۴ٸ�
    {
        int x = max(dispatchThreadID.x - gBlurRadius, 0); // �׷��� �ƴ� ��ü �ȼ� ��ġ���� �ݰ游ŭ �� ��ġ
        gCache[groupThreadID.x] = g_Tex_0[int2(x, dispatchThreadID.y)]; // ���� �޸� x ��ġ�� �ȼ��ֱ�
    }
    if (groupThreadID.x >= N - gBlurRadius) // �� �׷��� ����ũ�⿡�� �ݰ游ŭ �� ��ġ���� ũ�ų� ���ٸ�
    {
        int x = min(dispatchThreadID.x + gBlurRadius, g_Vec2_0.x - 1); // �ȼ� ��ü ���� �������ϰ� ��ü �ȼ���ġ���� �ݰ��� ���� ��ġ�� �����ɷ� x�� ���Ѵ�.
        gCache[groupThreadID.x + 2 * gBlurRadius] = g_Tex_0[int2(x, dispatchThreadID.y)]; // �����޸� �׷� �ȼ� ��ġ + �ݰ� �ι� ����ġ�� �ȼ��� ��´�.
    }

    gCache[groupThreadID.x + gBlurRadius] = g_Tex_0[min(dispatchThreadID.xy, g_Vec2_0.xy - 1)];

    GroupMemoryBarrierWithGroupSync();
	
    float4 blurColor = float4(0, 0, 0, 0);
	
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.x + gBlurRadius + i;
        blurColor += gWeights[i + gBlurRadius] * gCache[k];
    }
    g_RWTex_0[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void CS_VerBlur(int3 groupThreadID : SV_GroupThreadID,
               int3 dispatchThreadID : SV_DispatchThreadID)
{
    if (groupThreadID.y < gBlurRadius)
    {
        int y = max(dispatchThreadID.y - gBlurRadius, 0);
        gCache[groupThreadID.y] = g_Tex_0[int2(dispatchThreadID.x, y)];
    }
    if (groupThreadID.y >= N - gBlurRadius)
    {
        int y = min(dispatchThreadID.y + gBlurRadius, g_Vec2_0.y - 1);
        gCache[groupThreadID.y + 2 * gBlurRadius] = g_Tex_0[int2(dispatchThreadID.x, y)];
    }
	
    gCache[groupThreadID.y + gBlurRadius] = g_Tex_0[min(dispatchThreadID.xy, g_Vec2_0.xy - 1)];

    GroupMemoryBarrierWithGroupSync();
	
    float4 blurColor = float4(0, 0, 0, 0);
	
	[unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = groupThreadID.y + gBlurRadius + i;
        blurColor += gWeights[i + gBlurRadius] * gCache[k];
    }
	
    g_RWTex_0[dispatchThreadID.xy] = blurColor;
}



//[numthreads(N, 1, 1)]
//void CS_HorBlur(int3 groupThreadID : SV_GroupThreadID, // �׷�� ��ġ
//               int3 dispatchThreadID : SV_DispatchThreadID) // ��� �����带 ��Ʋ�� ��ġ
//{
//    int iRadius = 0;
//    float fWeight = 1.f;
//    float fRadius = lerp(0.f, 5.f, (g_Tex_1[int2(dispatchThreadID.x, dispatchThreadID.y)].z / 3000.f));
//    if (fRadius < 1.f)
//        iRadius = 0;
//    else if (fRadius < 2.f)
//        iRadius = 1;
//    else if (fRadius < 3.f)
//        iRadius = 2;
//    else if (fRadius < 4.f)
//        iRadius = 3;
//    else if (fRadius < 5.f)
//        iRadius = 4;
//    else
//        iRadius = 5;
//    if (iRadius != 0)
//    {
//        fWeight = 1.f / (iRadius * 2.f);
//    }

//    if (groupThreadID.x < iRadius) // �׷�� ��ġ x�� ������ �ݰ溸�� �۴ٸ�
//    {
//        int x = max(dispatchThreadID.x - iRadius, 0); // �׷��� �ƴ� ��ü �ȼ� ��ġ���� �ݰ游ŭ �� ��ġ
//        gCache[groupThreadID.x] = g_Tex_0[int2(x, dispatchThreadID.y)]; // ���� �޸� x ��ġ�� �ȼ��ֱ�
//    }
//    if (groupThreadID.x >= N - iRadius) // �� �׷��� ����ũ�⿡�� �ݰ游ŭ �� ��ġ���� ũ�ų� ���ٸ�
//    {
//        int x = min(dispatchThreadID.x + iRadius, g_Vec2_0.x - 1); // �ȼ� ��ü ���� �������ϰ� ��ü �ȼ���ġ���� �ݰ��� ���� ��ġ�� �����ɷ� x�� ���Ѵ�.
//        gCache[groupThreadID.x + 2 * iRadius] = g_Tex_0[int2(x, dispatchThreadID.y)]; // �����޸� �׷� �ȼ� ��ġ + �ݰ� �ι� ����ġ�� �ȼ��� ��´�.
//    }

//    gCache[groupThreadID.x + iRadius] = g_Tex_0[min(dispatchThreadID.xy, g_Vec2_0.xy - 1)];

//    GroupMemoryBarrierWithGroupSync();
	
//    float4 blurColor = float4(0, 0, 0, 0);
	
//	[unroll]
//    for (int i = -iRadius; i <= iRadius; ++i)
//    {
//        int k = groupThreadID.x + iRadius + i;
//        blurColor += /*gWeights[i + gBlurRadius]*/fWeight * gCache[k];
//    }
//    if (blurColor.r <= 0.01f && blurColor.g <= 0.01f && blurColor.b <= 0.01f)
//        return;

//    g_RWTex_0[dispatchThreadID.xy] = blurColor;
//}

//[numthreads(1, N, 1)]
//void CS_VerBlur(int3 groupThreadID : SV_GroupThreadID,
//               int3 dispatchThreadID : SV_DispatchThreadID)
//{
//    int iRadius = 0;
//    float fWeight = 1.f;
//    float fRadius = lerp(0.f, 5.f, (g_Tex_1[int2(dispatchThreadID.x, dispatchThreadID.y)].z / 3000.f));
//    if (fRadius < 1.f)
//        iRadius = 0;
//    else if (fRadius < 2.f)
//        iRadius = 1;
//    else if (fRadius < 3.f)
//        iRadius = 2;
//    else if (fRadius < 4.f)
//        iRadius = 3;
//    else if (fRadius < 5.f)
//        iRadius = 4;
//    else
//        iRadius = 5;
//    if (iRadius != 0)
//    {
//        fWeight = 1.f / (iRadius * 2.f);
//    }

//    if (groupThreadID.y < iRadius)
//    {
//        int y = max(dispatchThreadID.y - iRadius, 0);
//        gCache[groupThreadID.y] = g_Tex_0[int2(dispatchThreadID.x, y)];
//    }
//    if (groupThreadID.y >= N - iRadius)
//    {
//        int y = min(dispatchThreadID.y + iRadius, g_Vec2_0.y - 1);
//        gCache[groupThreadID.y + 2 * iRadius] = g_Tex_0[int2(dispatchThreadID.x, y)];
//    }
	
//    gCache[groupThreadID.y + iRadius] = g_Tex_0[min(dispatchThreadID.xy, g_Vec2_0.xy - 1)];

//    GroupMemoryBarrierWithGroupSync();
	
//    float4 blurColor = float4(0, 0, 0, 0);
	
//	[unroll]
//    for (int i = -iRadius; i <= iRadius; ++i)
//    {
//        int k = groupThreadID.y + iRadius + i;
//        blurColor += /*gWeights[i + gBlurRadius]*/fWeight * gCache[k];
//    }

//    if (blurColor.r <= 0.01f && blurColor.g <= 0.01f && blurColor.b <= 0.01f)
//        return;
	
//    g_RWTex_0[dispatchThreadID.xy] = blurColor;
//}