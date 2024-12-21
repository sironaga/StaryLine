#include "SceneTitle.h"
#include "StartTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"
#include "SoundList.h"
#include "BackGround.h"
CStartTitle* g_pStartTilte;

IXAudio2SourceVoice* g_pSourseTitleBGM;
CSoundList* g_TitleSound;
CBackGround* g_pTitleBG;

//‰Šú‰»ˆ—
void InitSceneTitle()
{
	

	g_TitleSound = new CSoundList(BGM_TITLE);
	g_pSourseTitleBGM = g_TitleSound->GetSound(true);
	
	
	g_pSourseTitleBGM->Start();
	g_pStartTilte = new CStartTitle();
	g_pTitleBG = new CBackGround();
}

//I—¹ˆ—
void UninitSceneTitle()
{
	delete g_pStartTilte;
	if (g_pSourseTitleBGM)
	{
		g_pSourseTitleBGM->Stop();
		g_pSourseTitleBGM = nullptr;
	}
	if (g_TitleSound)
	{
		delete g_TitleSound;
		g_TitleSound = nullptr;
	}
	delete g_pTitleBG;
	g_pTitleBG = nullptr;
	
}

//XVˆ—
void UpdateSceneTitle()
{
	g_pTitleBG->Update();
	g_pStartTilte->UpdateStartTitle();
	//if (IsKeyTrigger(VK_RETURN) || CGetButtons(XINPUT_GAMEPAD_B))
	//{
	//	//	UninitSound();
	//	//	InitSound();
	//	
	//	ChangeScene(SCENE_GAME);
	//}

	if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{
	
		ChangeScene(SCENE_DEBUGROOM);
	}
	if(g_pSourseTitleBGM)SetVolumeBGM(g_pSourseTitleBGM);
}

//•`‰æˆ—
void DrawSceneTitle()
{
	g_pTitleBG->Draw();
	g_pStartTilte->DrawStartTitle();
}