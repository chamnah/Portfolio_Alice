#include "CShaderMgr.h"
#include "CShader.h"
#include "CRenderMgr.h"

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
	SafeMapDelete(m_mapShader);
	for (UINT i = 0; i < m_vecShaderClone.size(); ++i)
	{
		delete m_vecShaderClone[i];
	}
	m_vecShaderClone.clear();
}

void CShaderMgr::Init()
{
	CShader* pShader = new CShader;
	pShader->CreateVertexShader("VS_Color", "vs_5_0",L"std.fx");
	pShader->CreatePixelShader("PS_Color", "ps_5_0", L"std.fx");
	//pShader->CreateGeoMetryShader("GS_Color", "gs_5_0");
	//pShader->CreateComputeShader("CS_HorizenBlur","cs_5_0");

	pShader->SetName(L"Color");
	AddShader(L"Color",pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Coll", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Coll", "ps_5_0", L"std.fx");

	pShader->SetName(L"Coll");
	AddShader(L"Coll", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Std_2D", "ps_5_0", L"std.fx");

	//pShader->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));
	pShader->SetName(L"Std");
	AddShader(L"Std", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Std_Spectrum", "ps_5_0", L"std.fx");

	pShader->SetName(L"Spectrum");
	AddShader(L"Spectrum", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Std_Color", "ps_5_0", L"std.fx");

	pShader->SetName(L"TexColor");
	AddShader(L"TexColor", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_SkyBox", "vs_5_0", L"skybox.fx");
	pShader->CreatePixelShader("PS_SkyBox", "ps_5_0", L"skybox.fx");

	pShader->SetName(L"SkyBox");
	AddShader(L"SkyBox", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Planet", "vs_5_0", L"sun.fx");
	pShader->CreatePixelShader("PS_Planet", "ps_5_0", L"sun.fx");

	pShader->SetName(L"Planet");
	AddShader(L"Planet", pShader);

	/*pShader = new CShader;
	pShader->CreateVertexShader("VS_Light", "vs_5_0", L"light.fx");
	pShader->CreatePixelShader("PS_Light", "ps_5_0", L"light.fx");

	pShader->SetName(L"Light");
	AddShader(L"Light", pShader);*/

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Merge", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_Merge", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"Merge");
	AddShader(L"Merge", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_PointLight", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_PointLight", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"PointLight");
	AddShader(L"PointLight", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_DirLight", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_DirLight", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"DirLight");
	AddShader(L"DirLight", pShader);


	pShader = new CShader;
	pShader->CreateVertexShader("VS_VolumeMesh", "vs_5_0", L"deferred.fx");

	pShader->SetName(L"Volume");
	AddShader(L"Volume", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Vision", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_Vision", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"Vision");
	AddShader(L"Vision", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Shadow", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_Shadow", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"Shadow");
	AddShader(L"Shadow", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_PointShadow", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_PointShadow", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"PointShadow");
	AddShader(L"PointShadow", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Tess", "vs_5_0", L"tess.fx");
	pShader->CreateHullShader("HS_Tess", "hs_5_0", L"tess.fx");
	pShader->CreateDomainShader("DS_Tess", "ds_5_0", L"tess.fx");
	pShader->CreatePixelShader("PS_Tess", "ps_5_0", L"tess.fx");

	pShader->SetName(L"Terrain");
	AddShader(L"Terrain", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Particle", "vs_5_0", L"particle.fx");
	pShader->CreateGeoMetryShader("GS_Particle", "gs_5_0", L"particle.fx");
	pShader->CreatePixelShader("PS_Particle", "ps_5_0", L"particle.fx");

	pShader->SetName(L"Particle");
	AddShader(L"Particle", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_AddRain", "vs_5_0", L"particle.fx");
	pShader->CreateStreamGeoMetryShader("GS_AddRain", "gs_5_0", L"particle.fx");

	pShader->SetName(L"AddRain");
	AddShader(L"AddRain", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_DrawRain", "vs_5_0", L"particle.fx");
	pShader->CreateGeoMetryShader("GS_DrawRain", "gs_5_0", L"particle.fx");
	pShader->CreatePixelShader("PS_DrawRain", "ps_5_0", L"particle.fx");
	pShader->SetName(L"DrawRain");
	AddShader(L"DrawRain", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_Brush", "cs_5_0", L"compute.fx");
	pShader->SetName(L"Compute");
	AddShader(L"Compute", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_Picking", "cs_5_0", L"compute.fx");
	pShader->SetName(L"Picking");
	AddShader(L"Picking", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_TexClear", "cs_5_0", L"compute.fx");
	pShader->SetName(L"TexClear");
	AddShader(L"TexClear", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_Weight", "cs_5_0", L"compute.fx");
	pShader->SetName(L"Weight");
	AddShader(L"Weight", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_Trail", "cs_5_0", L"compute.fx");
	pShader->SetName(L"AddTrail");
	AddShader(L"AddTrail", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Trail", "vs_5_0", L"deferred.fx");
	pShader->CreateGeoMetryShader("GS_Trail", "gs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_Trail", "ps_5_0", L"deferred.fx");
	pShader->SetName(L"Trail");
	AddShader(L"Trail", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Billboard", "vs_5_0", L"std.fx");
	pShader->CreateGeoMetryShader("GS_Billboard", "gs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Billboard", "ps_5_0", L"std.fx");
	pShader->SetName(L"Billboard");
	AddShader(L"Billboard", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_Particle", "cs_5_0", L"compute.fx");
	pShader->SetName(L"AddTrailParticle");
	AddShader(L"AddTrailParticle", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_TrailParticle", "vs_5_0", L"std.fx");
	pShader->CreateGeoMetryShader("GS_TrailParticle", "gs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_TrailParticle", "ps_5_0", L"std.fx");
	pShader->SetName(L"TrailParticle");
	AddShader(L"TrailParticle", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Decal", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Decal", "ps_5_0", L"std.fx");
	pShader->SetName(L"Decal");
	AddShader(L"Decal", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Forward", "vs_5_0", L"std.fx");
	pShader->CreateVertexInstanceShader("VS_ForwardInst", "vs_5_0", L"std.fx");
	pShader->CreatePixelShader("PS_Forward", "ps_5_0", L"std.fx");
	pShader->SetName(L"Forward");
	AddShader(L"Forward", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Deferred", "vs_5_0", L"deferred.fx");
	pShader->CreateVertexInstanceShader("VS_Deferred_Instance", "vs_5_0", L"deferred.fx");
	pShader->CreatePixelShader("PS_Deferred", "ps_5_0", L"deferred.fx");

	pShader->SetName(L"3d_Deferred");
	AddShader(L"3d_Deferred", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Distortion", "vs_5_0", L"posteffect.fx");
	pShader->CreatePixelShader("PS_Distortion", "ps_5_0", L"posteffect.fx");

	pShader->SetName(L"PostEffect");
	AddShader(L"PostEffect", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_DistortionSphere", "vs_5_0", L"posteffect.fx");
	pShader->CreatePixelShader("PS_DistortionSphere", "ps_5_0", L"posteffect.fx");

	pShader->SetName(L"DistortionSphere");
	AddShader(L"DistortionSphere", pShader);


	pShader = new CShader;
	pShader->CreateComputeShader("CS_HorBlur", "cs_5_0", L"compute.fx");
	pShader->SetName(L"HorBlur");
	AddShader(L"HorBlur", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader("CS_VerBlur", "cs_5_0", L"compute.fx");
	pShader->SetName(L"VerBlur");
	AddShader(L"VerBlur", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Blur", "vs_5_0", L"posteffect.fx");
	pShader->CreatePixelShader("PS_Blur", "ps_5_0", L"posteffect.fx");

	pShader->SetName(L"Blur");
	AddShader(L"Blur", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Fade", "vs_5_0", L"posteffect.fx");
	pShader->CreatePixelShader("PS_Fade", "ps_5_0", L"posteffect.fx");

	pShader->SetName(L"Fade");
	AddShader(L"Fade", pShader);
}

void CShaderMgr::AddShader(wstring _wcsKey, CShader * _pShader)
{
	if (m_mapShader.find(_wcsKey) != m_mapShader.end()) // 이미 같은 셰이더가 존재한다면 오류!!!
		assert(false);
	m_mapShader.insert(make_pair(_wcsKey,_pShader));
}

CShader * CShaderMgr::FindShader(wstring _wcsKey)
{
	map<wstring,CShader*>::iterator iter = m_mapShader.find(_wcsKey);

	if (iter == m_mapShader.end())
		assert(NULL);

	return iter->second;
}