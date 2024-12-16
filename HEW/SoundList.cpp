#include "SoundList.h"
#include "Sound.h"
#include "iostream"
#include "vector"

#define MAX_SOUND (9)

#define FILENAME_WALKSE (L"Asset/Sound/walk.wav")
#define FILENAME_SELECTSE (L"Asset/Sound/select.wav")
#define FILENAME_DECISIONSE (L"Asset/Sound/decision.wav")
#define FILENAME_CANCELSE (L"Asset/Sound/cancel.wav")
#define FILENAME_COMPLETESE (L"Asset/Sound/complete.wav")
#define FILENAME_ARRIVALSE (L"Asset/Sound/star.wav")
#define FILENAME_ATTACKSE (L"Asset/Sound/Attack.wav") 

#define FILENAME_TITLEBGM (L"Asset/Sound/title.wav")
#define FILENAME_BATTLEBGM (L"Asset/Sound/battle.wav")


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
	default:
		break;
	}
	
}

CSoundList::~CSoundList()
{
	delete m_sound;
	m_sound = nullptr;
}

IXAudio2SourceVoice* CSoundList::GetSound(bool Loop)
{
	IXAudio2SourceVoice* m_Source = m_sound->PlayWaveSound(Loop);
	return m_Source;
}

XAUDIO2_BUFFER CSoundList::GetBuffer(bool inLoop)
{
	XAUDIO2_BUFFER buffer = m_sound->GetBuffer(inLoop);
	return buffer;
}






