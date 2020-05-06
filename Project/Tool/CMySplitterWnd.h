#pragma once
#include <afxext.h>
class CMySplitterWnd :
	public CSplitterWnd
{

public:
	void SetBorder(int _iX,int _iY);

public:
	CMySplitterWnd();
	~CMySplitterWnd();
};