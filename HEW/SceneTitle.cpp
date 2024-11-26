#include "SceneTitle.h"
#include "StartTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"

CStartTitle* g_pStartTilte;
XAUDIO2_BUFFER* Sound;
XAUDIO2_BUFFER* NullSound;
IXAudio2SourceVoice* pSound;
//初期化処理
void InitSceneTitle()
{
	g_pStartTilte = new CStartTitle();
//	Sound = LoadSound("Asset/Sound/メイプルチョコレートLoop.mp3", 1);
//	pSound = PlaySound(Sound);
//	pSound->SetVolume(0.01f);
}

//終了処理
void UninitSceneTitle()
{
	delete g_pStartTilte;
}

//更新処理
void UpdateSceneTitle()
{
	g_pStartTilte->UpdateStartTitle();
	if (IsKeyTrigger(VK_RETURN) || CGetButtons(XINPUT_GAMEPAD_B))
	{
	//	UninitSound();
	//	InitSound();
		ChangeScene(SCENE_GAME);
	}
}

//描画処理
void DrawSceneTitle()
{
	g_pStartTilte->DrawStartTitle();
}