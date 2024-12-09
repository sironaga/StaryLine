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

struct sound
{
	std::vector<CSound*> m_Sound;
	int i=0;
};
sound g_Soundvoise[MAX_SOUND];
CSound* g_Source;
CSound* g_Source2;

void InitSound()
{
	
	g_Source = new CSound(FILENAME_BATTLEBGM);
	g_Soundvoise[BGM_BATTLE].m_Sound.push_back(g_Source);
	
}

void UnInitSound()
{
	//âï˙èàóù
	for (int i = 0; i < MAX_SOUND; i++)
	{
		
		if (g_Soundvoise[i].m_Sound.size() != 0)
		{
			g_Soundvoise[i].m_Sound.clear();
		}
		g_Soundvoise[i].i = 0;
		
	}
}



IXAudio2SourceVoice* GetSound(NAME inName,bool Loop)
{
	
	switch (inName)
	{
	case SE_WALK:
		g_Source = new CSound(FILENAME_WALKSE);
		g_Soundvoise[SE_WALK].m_Sound.push_back(g_Source);
		break;
	case SE_SELECT:
		g_Source= new CSound(FILENAME_SELECTSE);
		g_Soundvoise[SE_SELECT].m_Sound.push_back(g_Source); 
		break;
	case SE_DECISION:
		g_Source= new CSound(FILENAME_DECISIONSE);
		g_Soundvoise[SE_DECISION].m_Sound.push_back(g_Source); 
		break;
	case SE_CANCEL:
		g_Source= new CSound(FILENAME_CANCELSE);
		g_Soundvoise[SE_CANCEL].m_Sound.push_back(g_Source); 
		break;
	case SE_COMPLETE:
		g_Source= new CSound(FILENAME_COMPLETESE);
		g_Soundvoise[SE_COMPLETE].m_Sound.push_back(g_Source); 
		break;
	case SE_ARRIVAL:
		g_Source = new CSound(FILENAME_ARRIVALSE);
		g_Soundvoise[SE_ARRIVAL].m_Sound.push_back(g_Source);
		break;
	case SE_ATTACK:
		g_Source= new CSound(FILENAME_ATTACKSE);
		g_Soundvoise[SE_ATTACK].m_Sound.push_back(g_Source); 
		break;
	case BGM_TITLE:
		g_Source = new CSound(FILENAME_TITLEBGM);
		g_Soundvoise[BGM_TITLE].m_Sound.push_back(g_Source);
		break;
	case BGM_BATTLE:
		g_Source= new CSound(FILENAME_BATTLEBGM);
		g_Soundvoise[BGM_BATTLE].m_Sound.push_back(g_Source); 
		break;
	default:
		break;
	}
	
	IXAudio2SourceVoice* g_Source2 = g_Soundvoise[inName].m_Sound[g_Soundvoise[inName].i]->PlayWaveSound(Loop);
	g_Soundvoise[inName].i++;
	return g_Source2;
}
int GetSoundNumber(NAME inName)
{
	return g_Soundvoise[inName].i;
}
