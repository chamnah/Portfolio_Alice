#pragma once


//#include "FW1CompileSettings.h"
//#include "FW1FontWrapper.h"
// Font
#pragma comment(lib, "FW1FontWrapper")

// Font




#include "CParent.h"

class CFontEngine : public CParent
{
	SINGLETON(CFontEngine);
private:
	//IFW1Factory*		m_pFW1Factory;
	//IFW1FontWrapper*	m_pFontWrapper;

public:
	void init();

public:
	void DrawFont();
};