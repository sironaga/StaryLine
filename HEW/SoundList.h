#pragma once
#include "Sound.h"


enum NAME
{
	//プレイヤーが歩くSE
	SE_WALK
	//選択SE
	,SE_SELECT
	//決定SE
	,SE_DECISION
	//キャンセルSE
	,SE_CANCEL
	//図形完成SE
	,SE_COMPLETE
	//各星に着いた時SE
	,SE_ARRIVAL
	//攻撃音(通常)
	, SE_NORMALATTACK
	//,SE_NORMALATTACK
	//攻撃音(弱点)
	, SE_WEAKNESSATTACK
	//筆の出し入れ
	,SE_WANDONOFF
	//召喚
	, SE_SUMMON
	//タイトルBGM
	,BGM_TITLE
	//バトルBGM
	,BGM_BATTLE
	//フィーバー時SE
	,BGM_FEVER
	//ゲームオーバーBGM
	,BGM_GAMEOVER
	//ゲームクリアBGM
	,BGM_GAMECLEAR
};
class CSoundList
{
public:
	CSoundList(NAME inName);
	~CSoundList();

	

	//再生用のサウンドソースを送る
	IXAudio2SourceVoice* GetSound(bool Loop);
	XAUDIO2_BUFFER GetBuffer(bool inLoop);

	CSound* m_sound;
	void SetMasterVolume();
};
void SetAllMasterVolume(float volume);
void SetAllVolumeBGM(float volume);
void SetAllVolumeSE(float volume);
void SetVolumeBGM(IXAudio2SourceVoice* InSouse);
void SetVolumeSE(IXAudio2SourceVoice* InSouse);



