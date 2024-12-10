#include "Sound.h"


std::string ConvertWideToAnsi(const std::wstring& wideStr);

using namespace std;


CSound::CSound(const std::wstring& fileName)
{
	HRESULT result;
	result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(result))
	{
		if (FAILED(result)) MessageBox(NULL, "COMの初期化失敗", " ", MB_OK);
	}

	result = XAudio2Create(&m_pXAudio2);
	if (FAILED(result)) MessageBox(NULL, "m_pXAudio2の初期化失敗", " ", MB_OK);

	if (m_pXAudio2)
	{
		result = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	}
	if (FAILED(result)) MessageBox(NULL, "m_pMasteringVoiceの初期化失敗", " ", MB_OK); 
	if (!LoadWaveFile(fileName, &waveData)) MessageBox(NULL, "ファイルの読み込み失敗", " ", MB_OK); 
	
}

CSound::~CSound()
{
	
	/*if (m_pSourceVoice)m_pSourceVoice->DestroyVoice();
	
	if(m_pMasteringVoice)m_pMasteringVoice->DestroyVoice();
	if (m_pXAudio2)m_pXAudio2->Release();
	
	
	CoUninitialize();*/

}
bool CSound::LoadWaveFile(const std::wstring& wFilePath, WaveData* outData)
{
	// 中身入ってるもの来たら、一旦解放しとく
	// (じゃないと、もとの中身のサウンドバッファーがある場合、メモリリークする)
	


	HMMIO mmioHandle = nullptr;

	// チャンク情報
	MMCKINFO chunkInfo{};

	// RIFFチャンク用
	MMCKINFO riffChunkInfo{};

#ifdef UNICODE
	// WAVファイルを開く
	mmioHandle = mmioOpen(
		(LPWSTR)wFilePath.data(),
		nullptr,
		MMIO_READ
	);
#else
	std::string ansiPath = ConvertWideToAnsi(wFilePath);
	// WAVファイルを開く
	mmioHandle = mmioOpen(
		const_cast<LPSTR>(ansiPath.c_str()),
		nullptr,
		MMIO_READ
	);
#endif
	if (!mmioHandle)
	{
		// Wavファイルを開けませんでした
		return false;
	}
	
	// RIFFチャンクに侵入するためにfccTypeにWAVEを設定をする
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに侵入する
	if (mmioDescend(
		mmioHandle,		//MMIOハンドル
		&riffChunkInfo,	//取得したチャンクの情報
		nullptr,		//親チャンク
		MMIO_FINDRIFF	//取得情報の種類
	) != MMSYSERR_NOERROR)
	{
		// 失敗
		// Riffチャンクに侵入失敗しました
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// 侵入先のチャンクを"fmt "として設定する
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		// fmtチャンクがないです
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	DWORD readSize = mmioRead(
		mmioHandle,						//ハンドル
		(HPSTR)&outData->m_wavFormat,	// 読み込み用バッファ
		chunkInfo.cksize				//バッファサイズ
	);

	if (readSize != chunkInfo.cksize)
	{
		// 読み込みサイズが一致していません
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (outData->m_wavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		// Waveフォーマットエラーです
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmtチャンクを退出する
	if (mmioAscend(mmioHandle, &chunkInfo, 0) != MMSYSERR_NOERROR)
	{
		// fmtチャンク退出失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに侵入
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(mmioHandle, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		// dataチャンク侵入失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}
	// サイズ保存
	outData->m_size = chunkInfo.cksize;

	// dataチャンク読み込み
	outData->m_soundBuffer = new char[chunkInfo.cksize];
	readSize = mmioRead(mmioHandle, (HPSTR)outData->m_soundBuffer, chunkInfo.cksize);
	if (readSize != chunkInfo.cksize)
	{
		// dataチャンク読み込み失敗
		mmioClose(mmioHandle, MMIO_FHOPEN);
		delete[] outData->m_soundBuffer;
		return false;
	}
	CreateSourceVoice();
	// ファイルを閉じる
	mmioClose(mmioHandle, MMIO_FHOPEN);

	return true;
}
bool CSound::CreateSourceVoice()
{
	//=======================
	// SourceVoiceの作成
	//=======================
	WAVEFORMATEX waveFormat{};

	// 波形フォーマットの設定
	memcpy(&waveFormat, &waveData.m_wavFormat, sizeof(waveData.m_wavFormat));

	// 1サンプル当たりのバッファサイズを算出
	waveFormat.wBitsPerSample = waveData.m_wavFormat.nBlockAlign * 8 / waveData.m_wavFormat.nChannels;


		// ソースボイスの作成 ここではフォーマットのみ渡っている
		HRESULT result = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice, (WAVEFORMATEX*)&waveFormat);
		if (FAILED(result))
		{
			// SourceVoice作成失敗
			return false;
		}
	

	

	return true;
}
IXAudio2SourceVoice* CSound::PlayWaveSound(bool loop)
{
	XAUDIO2_VOICE_STATE state;
	int i = 0;
	
		m_pSourceVoice->GetState(&state);
		
	
	//================================
	// 波形データ(音データ本体)をソースボイスに渡す
	//================================

	m_xAudio2Buffer.pAudioData = (BYTE*)waveData.m_soundBuffer;
	m_xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	m_xAudio2Buffer.AudioBytes = waveData.m_size;
	// 三項演算子を用いて、ループするか否かの設定をする
	m_xAudio2Buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	
	m_pSourceVoice->SubmitSourceBuffer(&m_xAudio2Buffer);
	if (!m_pSourceVoice)
	{
		MessageBox(NULL, "m_pSourceVoice失敗", " ", MB_OK);
	}
	// 実際に音を鳴らす
	//m_pSourceVoice[i]->Start();
	
	return m_pSourceVoice;
}



// ワイド文字列を ANSI 文字列に変換するヘルパー関数
std::string ConvertWideToAnsi(const std::wstring& wideStr) {
	int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string ansiStr(sizeNeeded, '\0');
	WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &ansiStr[0], sizeNeeded, NULL, NULL);
	return ansiStr;
}