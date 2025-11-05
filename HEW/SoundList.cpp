#include "SoundList.h"
#include "Sound.h"
#include "iostream"
#include "vector"

#define MAX_SOUND (24)

#define FILENAME_WALKSE (L"Assets/Sound/walk.wav")
#define FILENAME_WALKGRASSSE (L"Assets/Sound/walkgrass.wav")
#define FILENAME_WALKDESERTSE (L"Assets/Sound/walkdesert.wav")
#define FILENAME_WALKSNOWSE (L"Assets/Sound/walksnow.wav")
#define FILENAME_SELECTSE (L"Assets/Sound/select.wav")
#define FILENAME_DECISIONSE (L"Assets/Sound/decision.wav")
#define FILENAME_CANCELSE (L"Assets/Sound/cancel.wav")
#define FILENAME_COMPLETESE (L"Assets/Sound/complete.wav")
#define FILENAME_ARRIVALSE (L"Assets/Sound/star.wav")
#define FILENAME_NORMALATTACKSE (L"Assets/Sound/normalAttack.wav") 
#define FILENAME_WEAKNESSATTACKSE (L"Assets/Sound/wwaknessAttack.wav") 
#define FILENAME_WANDONOFFSE (L"Assets/Sound/wandonoff.wav") 
#define FILENAME_SUMMONSE (L"Assets/Sound/summon.wav") 
#define FILENAME_SUMMONENEMYSE (L"Assets/Sound/SummonEnemy.wav") 
#define FILENAME_DEATHSE (L"Assets/Sound/death.wav")  
#define FILENAME_LOCKSE (L"Assets/Sound/lock.wav") 
#define FILENAME_TUTORIALSE (L"Assets/Sound/Tutorial_selected.wav") 

#define FILENAME_TITLEBGM (L"Assets/Sound/title.wav")
#define FILENAME_SELECTEBGM (L"Assets/Sound/StageSelect.wav")
#define FILENAME_BATTLEBGM1 (L"Assets/Sound/Stage1_Battle.wav")
#define FILENAME_BATTLEBGM2 (L"Assets/Sound/Stage2_Battle.wav")
#define FILENAME_BATTLEBGM3 (L"Assets/Sound/Stage3_Battle.wav")
#define FILENAME_FEVERBGM (L"Assets/Sound/fever.wav")
#define FILENAME_GAMEOVERBGM (L"Assets/Sound/gameover.wav")
#define FILENAME_GAMECLEARBGM (L"Assets/Sound/gameclear.wav")
#define FILENAME_TUTORIALBGM (L"Assets/Sound/Tutorial_BGM.wav") 

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
	case SE_WALKGRASS:
		m_sound = new CSound(FILENAME_WALKGRASSSE);
		break;
	case SE_WALKDESERT:
		m_sound = new CSound(FILENAME_WALKDESERTSE);
		break;
	case SE_WALKSNOW:
		m_sound = new CSound(FILENAME_WALKSNOWSE);
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
	case SE_NORMALATTACK:
		m_sound = new CSound(FILENAME_NORMALATTACKSE);
		break;
	case SE_WEAKNESSATTACK:
		m_sound = new CSound(FILENAME_WEAKNESSATTACKSE);
		break;
	case SE_WANDONOFF:
		m_sound = new CSound(FILENAME_WANDONOFFSE);
		break;
	case SE_SUMMON:
		m_sound = new CSound(FILENAME_SUMMONSE);
		break;
	case SE_SUMMONENEMY:
		m_sound = new CSound(FILENAME_SUMMONENEMYSE);
		break;
	case SE_DEATH:
		m_sound = new CSound(FILENAME_DEATHSE);
		break;
	case SE_LOCK:
		m_sound = new CSound(FILENAME_LOCKSE);
		break;
	case SE_TUTORIAL:
		m_sound = new CSound(FILENAME_TUTORIALSE);
		break;
	case BGM_TITLE:
		m_sound = new CSound(FILENAME_TITLEBGM);
		break;
	case BGM_SELECT:
		m_sound = new CSound(FILENAME_SELECTEBGM);
		break;
	case BGM_BATTLE1:
		m_sound = new CSound(FILENAME_BATTLEBGM1);
		break;
	case BGM_BATTLE2:
		m_sound = new CSound(FILENAME_BATTLEBGM2);
		break;
	case BGM_BATTLE3:
		m_sound = new CSound(FILENAME_BATTLEBGM3);
		break;
	case BGM_FEVER:
		m_sound = new CSound(FILENAME_FEVERBGM);
		break;
	case BGM_GAMEOVER:
		m_sound = new CSound(FILENAME_GAMEOVERBGM);
		break;
	case BGM_GAMECLEAR:
		m_sound = new CSound(FILENAME_GAMECLEARBGM);
		break;
	case BGM_TUTORIAL:
		m_sound = new CSound(FILENAME_TUTORIALBGM);
		break;
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



