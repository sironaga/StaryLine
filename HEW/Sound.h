#pragma once
#pragma comment (lib,"xaudio2.lib")
#pragma comment (lib,"winmm.lib")


#include <xaudio2.h>
#include <mmsystem.h>
#include <string>
#include <iostream>

#define MAX_SOURCEVOICE (100)

struct WaveData
{
	WAVEFORMATEX m_wavFormat;
	char* m_soundBuffer;
	DWORD m_size;

	~WaveData() { free(m_soundBuffer); }
};

class CSound
{
public:
	CSound(const std::wstring& fileName);
	~CSound();

private:
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SourceVoice* m_pSourceVoice;
	WaveData waveData;
	XAUDIO2_BUFFER m_xAudio2Buffer{};
	bool LoadWaveFile(const std::wstring& wFilePath, WaveData* outData);
	bool CreateSourceVoice();

public:
	IXAudio2SourceVoice* PlayWaveSound(bool loop);
	
};

