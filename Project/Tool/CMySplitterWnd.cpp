#include "stdafx.h"
#include "CMySplitterWnd.h"

CMySplitterWnd::CMySplitterWnd()
{
}


CMySplitterWnd::~CMySplitterWnd()
{
}

void CMySplitterWnd::SetBorder(int _iX, int _iY)
{
	m_cxSplitterGap = _iX;
	m_cySplitterGap = _iY;

	m_cxBorder = 0;
	m_cyBorder = 0;

	m_cxSplitter = 0;
	m_cySplitter = 0;

	m_cxBorderShare = 0;
	m_cyBorderShare = 0;
}