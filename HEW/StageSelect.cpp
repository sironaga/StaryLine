#include "StageSelect.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Defines.h"
#include "SpriteDrawer.h"
#include "InputEx.h"
#include "SoundList.h"
#include"SceneResult.h"

#define LOGO_WIND (1000)
#define LOGO_HID (1000)
#define ACS1_WID (700)
#define ACS1_HID (700)
#define SET_POTION_X (-900)
#define SET_POTION_Y (0)
#define E_WID (540)
#define E_HID (300)
#define SELECT_POSX (-320.0f)
#define SELECT_MOVE (185.0f)

#define ARROW_RIGHTX (560.0f)
#define ARROW_LEFTX (-560.0f)
#define ARROW_RIGHTY (300.0f)
#define ARROW_LEFTY (300.0f)
StageType g_Select_type;

CStageSelect::CStageSelect()
	: f_Rotation(0)
	, f_Rad(0)
	, f_SelectX(350)
	, f_SelectY(350)
	, MainStage(true)
{
	g_Select_type.StageMainNumber = GRASSLAND;
	g_Select_type.StageSubNumber = STAGE1;
	m_pGrassLand         = new SpriteEx("Assets/Texture/StageSelectBackGround/Grass_Map.png");
	m_pDesert            = new SpriteEx("Assets/Texture/StageSelectBackGround/Desert_Map.png");
	m_pSnowField         = new SpriteEx("Assets/Texture/StageSelectBackGround/Snow_Map.png");
	m_pGrassLandStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pGrassLandStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pGrassLandStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
	m_pDesertStage[0]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pDesertStage[1]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pDesertStage[2]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
	m_pSnowFieldStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pSnowFieldStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pSnowFieldStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
    m_pRight_Select      = new SpriteEx("Assets/Texture/StageSelectBackGround/Right_Select.png");
    m_pLeft_Select       = new SpriteEx("Assets/Texture/StageSelectBackGround/Left_Select.png");

	nSlect = 0;

}

CStageSelect::~CStageSelect()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pGrassLandStage[nLoop]->SetProjection(Get2DProj());
		m_pGrassLandStage[nLoop]->SetView(Get2DView());
		m_pDesertStage[nLoop]->SetProjection(Get2DProj());
		m_pDesertStage[nLoop]->SetView(Get2DView());
		m_pSnowFieldStage[nLoop]->SetProjection(Get2DProj());
		m_pSnowFieldStage[nLoop]->SetView(Get2DView());
	}
	if (m_pGrassLand)
	{
		delete m_pGrassLand;
		m_pGrassLand = nullptr;
	}
	if (m_pDesert)
	{
		delete m_pDesert;
		m_pDesert = nullptr;
	}
	if (m_pSnowField)
	{
		delete m_pSnowField;
		m_pSnowField = nullptr;
	}
	if (m_pRight_Select)
	{
		delete m_pRight_Select;
		m_pRight_Select = nullptr;
	}
	if (m_pLeft_Select)
	{
		delete m_pLeft_Select;
		m_pLeft_Select = nullptr;
	}
	
}

void CStageSelect::Update()
{
	if (MainStage)
	{
		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):
			if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = DESERT; }
			break;
		case(DESERT):
			if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = SNOWFIELD; }
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = GRASSLAND; }
			break;
		case(SNOWFIELD):
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = DESERT; }
			break;
		}
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B)) { MainStage ^= true; }
	}
	else
	{
		switch (g_Select_type.StageSubNumber)
		{
		case(STAGE1):
			if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = STAGE2; }
			if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B)) { SetNext(SCENE_GAME, g_Select_type); }//シーン移行
			break;

		case(STAGE2):
			if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = STAGE3; }
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = STAGE1; }
			if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B)) { SetNext(SCENE_GAME, g_Select_type); }//シーン移行
			break;

		case(STAGE3):
			//if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE1; }
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = STAGE2; }
		default: break;
		}
	
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
		{
			CSceneResult::InStageLevel(g_Select_type);
			SetNext(SCENE_GAME, g_Select_type);
		}
		if (IsKeyTrigger(VK_BACK) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
		{
			MainStage ^= true;
		}
	}//シーン移行
}

void CStageSelect::Draw()
{
	if (MainStage)
	{
		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):

			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(Get2DProj());
			m_pGrassLand->SetView(Get2DView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLand->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLand->Disp();
			break;
		case(DESERT):

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(Get2DProj());
			m_pDesert->SetView(Get2DView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pDesert->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pDesert->Disp();
			break;
		case(SNOWFIELD):

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(Get2DProj());
			m_pSnowField->SetView(Get2DView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pSnowField->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pSnowField->Disp();
			break;
		}
	}
	else
	{
		switch (g_Select_type.StageSubNumber)
		{
		case(STAGE1):
		
			Sprite::ReSetSprite();
			m_pGrassLandStage[0]->SetProjection(Get2DProj());
			m_pGrassLandStage[0]->SetView(Get2DView());
			m_pGrassLandStage[0]->SetTexture();
			m_pGrassLandStage[0]->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandStage[0]->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandStage[0]->Disp();
			break;
		
		case(STAGE2):
		
			Sprite::ReSetSprite();
			m_pGrassLandStage[1]->SetProjection(Get2DProj());
			m_pGrassLandStage[1]->SetView(Get2DView());
			m_pGrassLandStage[1]->SetTexture();
			m_pGrassLandStage[1]->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandStage[1]->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandStage[1]->Disp();
			break;
		
		case(STAGE3):
		
			Sprite::ReSetSprite();
			m_pGrassLandStage[2]->SetProjection(Get2DProj());
			m_pGrassLandStage[2]->SetView(Get2DView());
			m_pGrassLandStage[2]->SetTexture();
			m_pGrassLandStage[2]->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandStage[2]->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandStage[2]->Disp();
			break;
		
		}
	}
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
