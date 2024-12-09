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
	//攻撃音
	,SE_ATTACK
	//タイトルBGM
	,BGM_TITLE
	//バトルBGM
	,BGM_BATTLE
};
void InitSound();
void UnInitSound();
//再生用のサウンドソースを送る
IXAudio2SourceVoice* GetSound(NAME inName, bool Loop);
int GetSoundNumber(NAME inName);