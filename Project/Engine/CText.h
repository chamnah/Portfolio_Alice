#pragma once
#include "CComponent.h"
class CText :
	public CComponent
{
private:
	IDWriteTextFormat*    m_textFormat;
	ID2D1SolidColorBrush* m_brush;
	IDWriteFactory*       m_wFactory;
	
	wstring m_strText;
	Vec4    m_vColor;
	wstring m_strFont;
	float   m_fSize;

public:
	virtual void Start();

	CLONE(CText);
	bool Setup();
	HRESULT DWInit(LPCWSTR fontname, float size, IDWriteFactory **factory, IDWriteTextFormat **format);
	void ChangeColor(const Vec4& _vColor);
	void ChangeFontAndSize();
	void SetSize(float _fSize) { m_fSize = _fSize; }
	float GetSize() { return m_fSize; }
	void SetFontName(wstring& _strFont) { m_strFont = _strFont; }
	void SetText(const wstring& _strText) { m_strText = _strText; }
	wstring& GetTextName() { return m_strText; }
	virtual void Render();
	void SetColor(const Vec4& _vColor) { m_vColor = _vColor; }
	Vec4& GetColor() { return m_vColor; }

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
public:
	CText();
	CText(const CText& _other);
	virtual ~CText();
};