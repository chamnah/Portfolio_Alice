#include "CText.h"
#include "CRenderMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CCore.h"

CText::CText()
	:m_textFormat(nullptr)
	, m_brush(nullptr)
	, m_wFactory(nullptr)
	, m_strText(L"aa")
	, m_strFont(L"¸¼Àº °íµñ")
	, m_fSize(30.f)
{
	m_eType = COMPONENT_TYPE::TEXT;
	Setup();
}

CText::CText(const CText & _other)
{
	*this = _other;
	DWInit(m_strFont.c_str(), m_fSize, &m_wFactory, &m_textFormat);
	TEXT_RT->CreateSolidColorBrush(D2D1::ColorF(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w), &m_brush);
	m_wFactory->CreateTextFormat(m_strFont.c_str(), 0, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, m_fSize, L"ko", &m_textFormat);
}

CText::~CText()
{
	SAFE_RELEASE(m_textFormat);
	SAFE_RELEASE(m_wFactory);
	SAFE_RELEASE(m_brush);
}

void CText::Start()
{
}

bool CText::Setup()
{
	if (FAILED(DWInit(m_strFont.c_str(), m_fSize, &m_wFactory, &m_textFormat)))
		return false;
	ChangeColor(m_vColor);
	return true;
}

HRESULT CText::DWInit(LPCWSTR fontname, float size,
	IDWriteFactory **factory, IDWriteTextFormat **format)
{
	HRESULT hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*factory),
		(IUnknown**)factory
	);
	if (FAILED(hr))
		return hr;

	hr = (*factory)->CreateTextFormat(
		fontname,
		0,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		format
	);

	return hr;
}

void CText::ChangeColor(const Vec4& _vColor)
{
	SAFE_RELEASE(m_brush);
	m_vColor = _vColor;
	TEXT_RT->CreateSolidColorBrush(D2D1::ColorF(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w), &m_brush);
}

void CText::ChangeFontAndSize()
{
	SAFE_RELEASE(m_textFormat);
	m_wFactory->CreateTextFormat(m_strFont.c_str(),0,DWRITE_FONT_WEIGHT_REGULAR,DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,m_fSize,L"ko", &m_textFormat);
}

void CText::Render()
{
	float fAngle = XMConvertToDegrees(TransForm()->GetRotation().z);
	D2D1_POINT_2F pt = { TransForm()->GetWorldPos().x,TransForm()->GetWorldPos().y};
	TEXT_RT->SetTransform(D2D1::Matrix3x2F::Rotation(fAngle, pt));

	D2D1_RECT_F rt;
	rt.left = TransForm()->GetWorldPos().x - TransForm()->GetScale().x * 0.5f;
	rt.top = TransForm()->GetWorldPos().y - TransForm()->GetScale().y * 0.5f;
	rt.right = TransForm()->GetWorldPos().x + TransForm()->GetScale().x * 0.5f;
	rt.bottom = TransForm()->GetWorldPos().y + TransForm()->GetScale().y * 0.5f;

	ID2D1RenderTarget* pRT = TEXT_RT;
	TEXT_RT->DrawTextW(m_strText.c_str(),(UINT32)m_strText.length(),
		m_textFormat,rt,m_brush);
}

void CText::Save(FILE * _pFile)
{
	SaveWString(m_strText,_pFile);
	fwrite(&m_vColor,sizeof(Vec4) ,1 , _pFile);
	fwrite(&m_fSize, sizeof(float), 1, _pFile);
}

void CText::Load(FILE * _pFile)
{
	m_strText = LoadWString(_pFile);
	fread(&m_vColor, sizeof(Vec4), 1, _pFile);
	fread(&m_fSize, sizeof(float), 1, _pFile);

	ChangeColor(m_vColor);
	ChangeFontAndSize();
}