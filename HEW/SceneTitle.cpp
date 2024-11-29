#include "SceneTitle.h"
#include "StartTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"
#include "SoundList.h"
CStartTitle* g_pStartTilte;

IXAudio2SourceVoice* g_pSourseTitleBGM;
IXAudio2SourceVoice* g_pSourseTitleSE;
//‰Šú‰»ˆ—
void InitSceneTitle()
{
	g_pStartTilte = new CStartTitle();
	g_pSourseTitleBGM = GetSound(BGM_TITLE,true);
	g_pSourseTitleSE = GetSound(SE_DECISION, false);
	g_pSourseTitleBGM->SetVolume(0.4f);
	g_pSourseTitleBGM->Start();
	
}

//I—¹ˆ—
void UninitSceneTitle()
{
	delete g_pStartTilte;
	g_pSourseTitleBGM->Stop();
	g_pSourseTitleBGM->DestroyVoice();
}

//XVˆ—
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

//•`‰æˆ—
void DrawSceneTitle()
{
	g_pStartTilte->DrawStartTitle();
}