#include "SoundList.h"
#include "Sound.h"
#include "iostream"
#include "vector"

#define MAX_SOUND (9)

#define FILENAME_WALKSE (L"Assets/Sound/walk.wav")
#define FILENAME_SELECTSE (L"Assets/Sound/select.wav")
#define FILENAME_DECISIONSE (L"Assets/Sound/decision.wav")
#define FILENAME_CANCELSE (L"Assets/Sound/cancel.wav")
#define FILENAME_COMPLETESE (L"Assets/Sound/complete.wav")
#define FILENAME_ARRIVALSE (L"Assets/Sound/star.wav")
#define FILENAME_ATTACKSE (L"Assets/Sound/Attack.wav") 

#define FILENAME_TITLEBGM (L"Assets/Sound/title.wav")
#define FILENAME_BATTLEBGM (L"Assets/Sound/battle.wav")

#define FILENAME_FEVERBGM (L"Assets/Sound/fever.wav")

float g_volumeBGM = 0.5f;
float g_volumeSE = 0.5f;
float g_masterSound = 0.5f;
CSoundList::CSoundList(NAME inName)
{
	switch (inName)
	{
	case SE_WALK:
		m_sound = new CSound(FILENAME_WALKSE);
		break;
	case SE_SELECT:
		m_sound = new CSound(FILENAME_SELECTSE);
		break;
	case SE_DECISION:
		m_sound = new CSound(FILENAME_DECISIONSE);
		break;
	case SE_CANCEL:
		m_sound = new CSound(FILENAME_CANCELSE);
		break;
	case SE_COMPLETE:
		m_sound = new CSound(FILENAME_COMPLETESE);
		break;
	case SE_ARRIVAL:
		m_sound = new CSound(FILENAME_ARRIVALSE);
		break;
	case SE_ATTACK:
		m_sound = new CSound(FILENAME_ATTACKSE);
		break;
	case BGM_TITLE:
		m_sound = new CSound(FILENAME_TITLEBGM);
		break;
	case BGM_BATTLE:
		m_sound = new CSound(FILENAME_BATTLEBGM);
		break;
	case BGM_FEVER:
		m_sound = new CSound(FILENAME_FEVERBGM);
	default:
		break;
	}
	m_sound->SetMasterVolume(0.5f);
}

CSoundList::~CSoundList()
{
	delete m_sound;
	m_sound = nullptr;
}



IXAudio2SourceVoice* CSoundList::GetSound(bool Loop)
{
	IXAudio2SourceVoice* m_Source = m_sound->PlayWaveSound(Loop);
	m_Source->SetVolume(0.5f);
	return m_Source;
}

XAUDIO2_BUFFER CSoundList::GetBuffer(bool inLoop)
{
	XAUDIO2_BUFFER buffer = m_sound->GetBuffer(inLoop);
	return buffer;
}

void CSoundList::SetMasterVolume()
{
	m_sound->SetMasterVolume(g_masterSound);
}


void SetAllMasterVolume(float volume)
{
	volume = volume / 10.0f;
	g_masterSound = volume;
}
void SetAllVolumeBGM(float volume)
{
	volume = volume / 10.0f;
	g_volumeBGM = volume;
}
void SetAllVolumeSE(float volume)
{
	volume = volume / 10.0f;
	g_volumeSE = volume;
}

void SetVolumeBGM(IXAudio2SourceVoice* InSouse)
{
	InSouse->SetVolume(g_volumeBGM);
}
void SetVolumeSE(IXAudio2SourceVoice* InSouse)
{
	InSouse->SetVolume(g_volumeSE);
}



