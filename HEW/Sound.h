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
	//WAVファイルのの読み込み
	bool LoadWaveFile(const std::wstring& wFilePath, WaveData* outData);

public:
	//スピーカーを作成
	IXAudio2SourceVoice* CreateSourceVoice(IXAudio2SourceVoice* inSource);
	//音データの作成
	XAUDIO2_BUFFER GetBuffer(bool inLoop);
	//スピーカーを作成と音データの作成を両方で
	IXAudio2SourceVoice* PlayWaveSound(bool loop);
	void SetMasterVolume(float volume);
private:
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;//すべての元になるスピーカー
	IXAudio2SourceVoice* m_pSourceVoice;//スピーカー
	bool m_use;
	WaveData waveData;//ファイル関係
	XAUDIO2_BUFFER m_xAudio2Buffer{};//音データの格納先

};

