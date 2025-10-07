#pragma once
#include "CManager.h"

class CSoundManager : public CManager
{
public:
	CSoundManager();
	~CSoundManager();

public:
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayFX(const TCHAR* pSoundKey, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	static CSoundManager* m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	unordered_map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
};

