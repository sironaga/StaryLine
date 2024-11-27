#include "SoundList.h"
#include "Sound.h"

#define MAX_SOUND (8)

#define FILENAME_WALKSE (L"Asset/Sound/walk.wav")
#define FILENAME_SELECTSE (L"Asset/Sound/select.wav")
#define FILENAME_DECISIONSE (L"Asset/Sound/decision.wav")
#define FILENAME_CANCELSE (L"Asset/Sound/cancel.wav")
#define FILENAME_COMPLETE (L"Asset/Sound/complete.wav")
#define FILENAME_ARRIVALSE (L"Asset/Sound/star.wav")

#define FILENAME_TITLEBGM (L"Asset/Sound/title.wav")
#define FILENAME_BATTLEBGM (L"Asset/Sound/battle.wav")


CSound* g_Sound[MAX_SOUND];
IXAudio2SourceVoice* g_Source;

void InitSound()
{
	g_Sound[SE_WALK] = new CSound(FILENAME_WALKSE);
	g_Sound[SE_SELECT] = new CSound(FILENAME_SELECTSE);
	g_Sound[SE_DECISION] = new CSound(FILENAME_DECISIONSE);
	g_Sound[SE_CANCEL] = new CSound(FILENAME_CANCELSE);
	g_Sound[SE_COMPLETE] = new CSound(FILENAME_COMPLETE);
	g_Sound[SE_ARRIVAL] = new CSound(FILENAME_ARRIVALSE);

	g_Sound[BGM_TITLE] = new CSound(FILENAME_TITLEBGM);
	g_Sound[BGM_BATTLE] = new CSound(FILENAME_BATTLEBGM);
}

void UnInitSound()
{
	//âï˙èàóù
	for (int i = 0; i < MAX_SOUND; i++)
	{
		if (!g_Sound[i]) continue;
		delete g_Sound[i];
		g_Sound[i] = nullptr;
	}
}



IXAudio2SourceVoice* GetSound(NAME inName,bool Loop)
{
	g_Source = g_Sound[inName]->PlayWaveSound(Loop);
	return g_Source;
}
