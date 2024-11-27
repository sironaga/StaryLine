#include "SoundList.h"
#include "Sound.h"

#define MAX_SOUND (8)

#define FILENAME_WALKSE (L"Asset/Sound/title.wav")
#define FILENAME_WALKSE (L"Asset/Sound/title.wav")
#define FILENAME_WALKSE (L"Asset/Sound/title.wav")
#define FILENAME_COMPLETE (L"Asset/Sound/complete.wav")

#define FILENAME_TITLEBGM (L"Asset/Sound/title.wav")
#define FILENAME_BATTLEBGM (L"Asset/Sound/battle.wav")


CSound* g_Sound[MAX_SOUND];
IXAudio2SourceVoice* g_Source;

void InitSound()
{
	g_Sound[SE_COMPLETE] = new CSound(FILENAME_COMPLETE);

	g_Sound[BGM_TITLE] = new CSound(FILENAME_TITLEBGM);
	g_Sound[BGM_BATTLE] = new CSound(FILENAME_BATTLEBGM);
}

void UnInitSound()
{
	for (int i = 0; i < MAX_SOUND; i++)
	{
		if (!g_Sound[i]) continue;
		delete g_Sound[i];
		g_Sound[i] = nullptr;
	}
}



IXAudio2SourceVoice* PlaySound(NAME inName,bool Loop)
{
	g_Source = g_Sound[inName]->PlayWaveSound(Loop);
	return g_Source;
}
