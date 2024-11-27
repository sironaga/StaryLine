#include "SceneTitle.h"
#include "StartTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"
#include "SoundList.h"
CStartTitle* g_pStartTilte;

IXAudio2SourceVoice* g_pSourseBGM;
//‰Šú‰»ˆ—
void InitSceneTitle()
{
	g_pStartTilte = new CStartTitle();
	g_pSourseBGM = GetSound(BGM_TITLE,true);
	g_pSourseBGM->SetVolume(0.09f);
	g_pSourseBGM->Start();
	
}

//I—¹ˆ—
void UninitSceneTitle()
{
	delete g_pStartTilte;
	g_pSourseBGM->Stop();
	g_pSourseBGM->DestroyVoice();
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