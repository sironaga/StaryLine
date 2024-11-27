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
	//タイトルBGM
	,BGM_TITLE
	//バトルBGM
	,BGM_BATTLE
};
void InitSound();
void UnInitSound();
//再生
IXAudio2SourceVoice* PlaySound(NAME inName, bool Loop);
