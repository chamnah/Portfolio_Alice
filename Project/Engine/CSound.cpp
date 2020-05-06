#include "CSound.h"
#include "CResMgr.h"

FMOD::System* CSound::g_pFMOD = nullptr;

CSound::CSound()
	: m_pSound(nullptr)
	, m_pChanel(nullptr)
{
}


CSound::~CSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

void CSound::Play(int _iRoopCount)
{
	if (0 == _iRoopCount)
	{
		return;
	}
	else if (_iRoopCount < -1)
	{
		assert(nullptr);
	}
	else if (_iRoopCount == -1)
	{
		// 무한반복
	}

	bool bPlaying = false;
	if (m_pChanel)
		m_pChanel->isPlaying(&bPlaying);

	if (bPlaying)
	{
		// 이미 재생중인 경우
		m_pChanel->stop();
	}

	_iRoopCount -= 1;

	g_pFMOD->playSound(m_pSound, nullptr, false, &m_pChanel);
	m_pChanel->setMode(FMOD_LOOP_NORMAL);
	m_pChanel->setLoopCount(_iRoopCount);
}

void CSound::Load(const wstring & _strFullPath, const wstring & _strName)
{
	string path(_strFullPath.begin(), _strFullPath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}
}

void CSound::Stop()
{
	m_pChanel->stop();
}

void CSound::Save(FILE * _pFile)
{
	// 키값
	SaveWString(GetName(), _pFile);

	// 상대경로
	SaveWString(m_wcsPath, _pFile);
}

void CSound::Load(FILE * _pFile)
{
	wstring strKey, strPath;
	strKey = LoadWString(_pFile);

	// 상대경로
	strPath = LoadWString(_pFile);
}

bool CSound::IsPlaying()
{
	bool bPlaying = false;
	m_pChanel->isPlaying(&bPlaying);
	return bPlaying;
}

//void CSound::FileSave(FILE * _pFile)
//{
//	
//}
//
//void CSound::FileLoad(FILE * _pFile)
//{
//	// 키값
//	
//
//	// 해당 키로 등록된게 없으면 로딩
//	/*if (nullptr == CResMgr::GetInst()->Load<CSound>(strKey))
//		CResMgr::GetInst()->AddResource<CSound>(strKey);*/
//}