#pragma once
#include "Sound.h"


enum NAME
{
	//プレイヤーが歩くSE
	SE_WALK
	//プレイヤーが歩く(草原)SE
	, SE_WALKGRASS
	//プレイヤーが歩く(砂漠)SE
	, SE_WALKDESERT
	//プレイヤーが歩く(雪)SE
	, SE_WALKSNOW
	//選択SE
	, SE_SELECT
	//決定SE
	, SE_DECISION
	//キャンセルSE
	, SE_CANCEL
	//図形完成SE
	, SE_COMPLETE
	//各星に着いた時SE
	, SE_ARRIVAL
	//攻撃音(通常)
	, SE_NORMALATTACK
	//,SE_NORMALATTACK
	//攻撃音(弱点)
	, SE_WEAKNESSATTACK
	//筆の出し入れ
	, SE_WANDONOFF
	//召喚
	, SE_SUMMON
	//召喚(敵)
	, SE_SUMMONENEMY
	//敗北音
	, SE_DEATH
	//ロック音
	, SE_LOCK
	//タイトルBGM
	, BGM_TITLE
	//セレクトBGM
	, BGM_SELECT
	//バトルBGM
	, BGM_BATTLE1
	//バトルBGM
	, BGM_BATTLE2
	//バトルBGM
	, BGM_BATTLE3
	//フィーバー時BGM
	, BGM_FEVER
	//ゲームオーバーBGM
	, BGM_GAMEOVER
	//ゲームクリアBGM
	, BGM_GAMECLEAR
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



