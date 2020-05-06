#pragma once
#include "CRes.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_codec.h"

#ifdef _DEBUG

#ifdef BIT32
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmodL64_vc.lib")
#endif

#else

#ifdef BIT32
#pragma comment(lib, "fmod_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")

#endif

#endif

class CSound :
	public CRes
{
public:
	static FMOD::System* g_pFMOD;

private:
	FMOD::Sound*		m_pSound;
	FMOD::Channel*		m_pChanel;

public:
	// -1(무한 반복)
	void Play(int _iRoopCount);
	virtual void Load(const wstring& _strFullPath, const wstring& _strName);
	void Stop();
	bool IsPlaying();

public:
	void Save(FILE* _pFile);
	static void Load(FILE* _pFile);

public:
	CSound();
	~CSound();
};