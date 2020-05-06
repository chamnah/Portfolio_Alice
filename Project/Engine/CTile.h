#pragma once
#include "CComponent.h"

class CTile :
	public CComponent
{
private:
	COLL_TYPE    m_eColl;
	bool         m_bClose;
	bool         m_bScale;


public:
	void SetCollType(COLL_TYPE _eColl) { m_eColl = _eColl; }
	COLL_TYPE GetCollType() { return m_eColl; }
	void SetClose(bool _bClose) { m_bClose = _bClose; }
	bool GetClose() { return m_bClose; }
	void SetScale(bool _bScale) { m_bScale = _bScale; }
	bool GetScale() { return m_bScale; }

	CLONE(CTile);

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CTile();
	~CTile();
};