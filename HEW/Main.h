#pragma once
/*
* File Main.h
* 2024/10/07 音量のセッターゲッターの追加
* 2024/10/08 モード管理用関数追加
*/

/* モード管理用 */
enum Screen
{
	TITLE, /* TITLE */
};

void SetVolume(int InVolume);	/* g_nSoundVolumeの変数を変更します */
int GetVolume();				/* g_nSoundVolumeの変数の値を取得します*/
void SetScreen(Screen InScreen);/* g_Screenの変数の値を変更します */