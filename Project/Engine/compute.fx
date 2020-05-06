#include "value.fx"

// SV_GroupID            uint3 그룹의 ID 값
// SV_GroupThreadID      uint3 그룹 안에서 Thread ID 값
// SV_DispatchThreadID   uint3 모든 그룹을 포함한 전체 Thread ID 값
// SV_GroupIndex         uint 그룹 안에서 스레드의 인덱스 값
// g_Tex_0               브러쉬 텍스쳐
// g_RWTex_0             높이맵 텍스쳐
// g_Vec2_0              브러쉬UV 좌표 (터레인 전체를 하나의 0~1사이로 판단한 UV)
// g_Vec2_1              브러쉬UV 사이즈 (터레인 전체를 하나의 0~1사이로 판단한 UV)
// g_Vec2_2              높이맵 텍스쳐 사이즈
[numthreads(512, 1, 1)]
void CS_Brush(uint3 iID : SV_DispatchThreadID)
{
    if (g_bTex0.x == 0 || g_bRTex0.x == 0)
        return;

    float2 vBrushPos = g_Vec2_0 * g_Vec2_2; // 터레인 사이즈 만큼 늘린다. 실제 픽셀 좌표를 구함
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
// g_Vec2_1 : Thread Group 갯수 x,y 여기서는 z는 쓰지 않는다.
// g_vec4_0 : Mouse Pos (터레인의 로컬 좌표계로 이동한 좌표)
// g_vec4_1 : Mouse Picking Dir 0(터레인의 로컬 좌표계로 이동한 방향)
// g_RWTex_0 : cpu로 보내는 용도
[numthreads(512, 1, 1)]
void CS_Picking(uint3 iID : SV_DispatchThreadID)
{
    if (!g_bRTex0.x)
        return;

    uint ThreadID = (512 * 1 * iID.z) + (512 * iID.y) + iID.x;///*(uint(g_Vec2_1.x * 512) * iID.y) +*/ iID.x; // 현재 ThreadID 전체를 한줄로 봣을때 몇번째 인지 계산
    uint Row = ThreadID / uint(g_Vec2_0.x * 2); // 터레인에서의 행
    uint Col = ThreadID % uint(g_Vec2_0.x * 2); // 터레인에서의 열

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
// g_int[0] : 몇번째 가중치 값을 넣을것인지
// g_Vec2_0 : BrushUVPos (전체 UV 기준)
// g_Vec2_1 : BrushUVScale (전체 UV 기준)
[numthreads(512,1,1)]
void CS_Weight(uint3 _iID : SV_DispatchThreadID)
{
    if (g_bTex0 == 0)
        return;
    float2 vBrushPos = g_Vec2_0 * g_Vec2_2; // 터레인 사이즈 만큼 늘린다. 실제 픽셀 좌표를 구함
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
//g_int_0 : 트레일 시작 0이면 트레일 처음 시작, 아니면 트레일 중간
//g_int_1 : 새롭게 push 되는지 여부를 밖에서 체크를 해서 보낸다.

[numthreads(20, 1, 1)]
void CS_Trail(uint3 _iID : SV_DispatchThreadID)
{
    // 쉐이더가 처음이면 좌표를 DrawTexture에 넣는다.
    if (_iID.x == 0)
    {
        g_RWTex_0[int2(0, 0)] = g_Vec4_0;
        g_RWTex_0[int2(0, 1)] = g_Vec4_1;
        
        if (g_int_1 == 0) // 푸쉬안됨
            return;
    }
    float4 vPosUp = g_Tex_0.Load(int3(_iID.x, 0, 0));
    float4 vPosDown = g_Tex_0.Load(int3(_iID.x, 1, 0));

    vPosUp.w -= g_fDT;
    vPosDown.w -= g_fDT;

    // push 됨
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
//g_int_0 : 새롭게 push 되는지 여부를 밖에서 체크를 해서 보낸다.
// g_Vec4_0 : 새로 푸쉬된 값
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
void CS_HorBlur(int3 groupThreadID : SV_GroupThreadID, // 그룹당 위치
               int3 dispatchThreadID : SV_DispatchThreadID) // 모든 쓰레드를 통틀은 위치
{
    if (groupThreadID.x < gBlurRadius) // 그룹당 위치 x가 임의의 반경보다 작다면
    {
        int x = max(dispatchThreadID.x - gBlurRadius, 0); // 그룹이 아닌 전체 픽셀 위치에서 반경만큼 뺀 위치
        gCache[groupThreadID.x] = g_Tex_0[int2(x, dispatchThreadID.y)]; // 공유 메모리 x 위치에 픽셀넣기
    }
    if (groupThreadID.x >= N - gBlurRadius) // 한 그룹의 가로크기에서 반경만큼 뺀 위치보다 크거나 같다면
    {
        int x = min(dispatchThreadID.x + gBlurRadius, g_Vec2_0.x - 1); // 픽셀 전체 가로 사이즈하고 전체 픽셀위치에서 반경을 더한 위치중 작은걸로 x를 정한다.
        gCache[groupThreadID.x + 2 * gBlurRadius] = g_Tex_0[int2(x, dispatchThreadID.y)]; // 공유메모리 그룹 픽셀 위치 + 반경 두배 한위치에 픽셀을 담는다.
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
//void CS_HorBlur(int3 groupThreadID : SV_GroupThreadID, // 그룹당 위치
//               int3 dispatchThreadID : SV_DispatchThreadID) // 모든 쓰레드를 통틀은 위치
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

//    if (groupThreadID.x < iRadius) // 그룹당 위치 x가 임의의 반경보다 작다면
//    {
//        int x = max(dispatchThreadID.x - iRadius, 0); // 그룹이 아닌 전체 픽셀 위치에서 반경만큼 뺀 위치
//        gCache[groupThreadID.x] = g_Tex_0[int2(x, dispatchThreadID.y)]; // 공유 메모리 x 위치에 픽셀넣기
//    }
//    if (groupThreadID.x >= N - iRadius) // 한 그룹의 가로크기에서 반경만큼 뺀 위치보다 크거나 같다면
//    {
//        int x = min(dispatchThreadID.x + iRadius, g_Vec2_0.x - 1); // 픽셀 전체 가로 사이즈하고 전체 픽셀위치에서 반경을 더한 위치중 작은걸로 x를 정한다.
//        gCache[groupThreadID.x + 2 * iRadius] = g_Tex_0[int2(x, dispatchThreadID.y)]; // 공유메모리 그룹 픽셀 위치 + 반경 두배 한위치에 픽셀을 담는다.
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