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

#define CENTER_POS_X SCREEN_WIDTH / 2.0f
#define CENTER_POS_Y SCREEN_HEIGHT / 2.0f

StageType g_Select_type;

CStageSelect::CStageSelect()
	: f_Rotation(0)
	, f_Rad(0)
	, f_SelectX(350)
	, f_SelectY(350)
	, MainStage(true)
{
	g_Select_type.StageMainNumber = GRASSLAND;
	g_Select_type.StageSubNumber = GRASSLAND_STAGE1; 
	m_pGrassLand         = new SpriteEx("Assets/Texture/StageSelectBackGround/Grass_Map.png");
	m_pDesert            = new SpriteEx("Assets/Texture/StageSelectBackGround/Desert_Map.png");
	m_pSnowField         = new SpriteEx("Assets/Texture/StageSelectBackGround/Snow_Map.png");

	m_pGrassLandStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.1.png");
	m_pGrassLandStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.2.png");
	m_pGrassLandStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.3.png");
	m_pDesertStage[0]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.1.png");
	m_pDesertStage[1]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.2.png");
	m_pDesertStage[2]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.3.png");
	m_pSnowFieldStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.1.png");
	m_pSnowFieldStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.2.png");
	m_pSnowFieldStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.3.png");

    m_pRight_Select      = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Right.png");
    m_pLeft_Select       = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Left.png");

	nSlect = 0;

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pGrassLandStage[nLoop]->SetProjection(GetProj());
		m_pGrassLandStage[nLoop]->SetView(GetView());
		m_pDesertStage[nLoop]->SetProjection(GetProj());
		m_pDesertStage[nLoop]->SetView(GetView());
		m_pSnowFieldStage[nLoop]->SetProjection(GetProj());
		m_pSnowFieldStage[nLoop]->SetView(GetView());
	}
	m_pGrassLand->SetProjection(GetProj());
	m_pGrassLand->SetView(GetView());
	m_pDesert->SetProjection(GetProj());
	m_pDesert->SetView(GetView());
	m_pSnowField->SetProjection(GetProj());
	m_pSnowField->SetView(GetView());
	m_pRight_Select->SetProjection(GetProj());
	m_pRight_Select->SetView(GetView());
	m_pLeft_Select->SetProjection(GetProj());
	m_pLeft_Select->SetView(GetView());
}

CStageSelect::~CStageSelect()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		if (m_pGrassLandStage[nLoop])
		{
		delete m_pGrassLandStage[nLoop];
		m_pGrassLandStage[nLoop] = nullptr;
		}
		if (m_pDesertStage[nLoop])
		{
		delete m_pDesertStage[nLoop];
		m_pDesertStage[nLoop] = nullptr;
		}
		if (m_pSnowFieldStage[nLoop])
		{
		delete m_pSnowFieldStage[nLoop];
		m_pSnowFieldStage[nLoop] = nullptr;
		}
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
			g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
			break;
		case(DESERT):
			if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = SNOWFIELD; }
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = GRASSLAND; }
			g_Select_type.StageSubNumber = DESERT_STAGE1;
			break;
		case(SNOWFIELD):
			if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = DESERT; }
			g_Select_type.StageSubNumber = SNOWFIELD_STAGE1;
			break;
		}
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
		{
			MainStage ^= true; 
		}
		if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
		{
			SetNext(SCENE_TITLE, g_Select_type);
		}
	}
	else
	{
		if (g_Select_type.StageMainNumber == GRASSLAND)
		{
			switch (g_Select_type.StageSubNumber)
			{
			case(GRASSLAND_STAGE1):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = GRASSLAND_STAGE2; }
				break;

			case(GRASSLAND_STAGE2):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = GRASSLAND_STAGE3; }
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = GRASSLAND_STAGE1; }
				break;

			case(GRASSLAND_STAGE3):
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = GRASSLAND_STAGE2; }
				break;
			}
		}
		if (g_Select_type.StageMainNumber == DESERT)
		{
			switch (g_Select_type.StageSubNumber)
			{
			case(DESERT_STAGE1):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = DESERT_STAGE2; }
				break;

			case(DESERT_STAGE2):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = DESERT_STAGE3; }
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = DESERT_STAGE1; }
				break;

			case(DESERT_STAGE3):
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = DESERT_STAGE2; }
				break;
			}
		}
		if (g_Select_type.StageMainNumber == SNOWFIELD)
		{
			switch (g_Select_type.StageSubNumber)
			{
			case(SNOWFIELD_STAGE1):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = SNOWFIELD_STAGE2; }
				break;

			case(SNOWFIELD_STAGE2):
				if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = SNOWFIELD_STAGE3; }
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = SNOWFIELD_STAGE1; }
				break;

			case(SNOWFIELD_STAGE3):
				if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageSubNumber = SNOWFIELD_STAGE2; }
				break;
			}
		}
		if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
		{
			CSceneResult::InStageLevel(g_Select_type);
			SetNext(SCENE_GAME, g_Select_type);
		}
		if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
		{
			MainStage ^= true;
		}
	}//ƒV[ƒ“ˆÚs
}

void CStageSelect::Draw()
{
	if (MainStage)
	{
		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):

			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(0.0f, 50.0f, 50.0f);
			m_pGrassLand->SetSize(180.0f, 100.0f, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(200.0f, 50.0f, 50.0f);
			m_pDesert->SetSize(170.0f, 90.0f, 100.0f);
			m_pDesert->Disp();
			break;
		case(DESERT):

			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(-200.0f, 50.0f, 50.0f);
			m_pGrassLand->SetSize(170.0f, 90.0f, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(0.0f, 50.0f, 50.0f);
			m_pDesert->SetSize(180.0f, 100.0f, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(200.0f, 50.0f, 50.0f);
			m_pSnowField->SetSize(170.0f, 90.0f, 100.0f);
			m_pSnowField->Disp();
			break;
		case(SNOWFIELD):

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(-200.0f, 50.0f, 50.0f);
			m_pDesert->SetSize(170.0f, 90.0f, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(0.0f, 50.0f, 50.0f);
			m_pSnowField->SetSize(180.0f, 100.0f, 100.0f);
			m_pSnowField->Disp();
			break;
		}
	}
	else
	{
		{
			switch (g_Select_type.StageSubNumber)
			{
				case(GRASSLAND_STAGE1):

					Sprite::ReSetSprite();
					m_pGrassLandStage[0]->SetProjection(GetProj());
					m_pGrassLandStage[0]->SetView(GetView());
					m_pGrassLandStage[0]->SetTexture();
					m_pGrassLandStage[0]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pGrassLandStage[0]->SetSize(180.0f, 100.0f, 100.0f);
					m_pGrassLandStage[0]->Disp();
					break;

				case(GRASSLAND_STAGE2):

					Sprite::ReSetSprite();
					m_pGrassLandStage[1]->SetProjection(GetProj());
					m_pGrassLandStage[1]->SetView(GetView());
					m_pGrassLandStage[1]->SetTexture();
					m_pGrassLandStage[1]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pGrassLandStage[1]->SetSize(180.0f, 100.0f, 100.0f);
					m_pGrassLandStage[1]->Disp();
					break;

				case(GRASSLAND_STAGE3):

					Sprite::ReSetSprite();
					m_pGrassLandStage[2]->SetProjection(GetProj());
					m_pGrassLandStage[2]->SetView(GetView());
					m_pGrassLandStage[2]->SetTexture();
					m_pGrassLandStage[2]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pGrassLandStage[2]->SetSize(180.0f, 100.0f, 100.0f);
					m_pGrassLandStage[2]->Disp();
					break;
				case(DESERT_STAGE1):

					Sprite::ReSetSprite();
					m_pDesertStage[0]->SetProjection(GetProj());
					m_pDesertStage[0]->SetView(GetView());
					m_pDesertStage[0]->SetTexture();
					m_pDesertStage[0]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pDesertStage[0]->SetSize(180.0f, 100.0f, 100.0f);
					m_pDesertStage[0]->Disp();
					break;

				case(DESERT_STAGE2):

					Sprite::ReSetSprite();
					m_pDesertStage[1]->SetProjection(GetProj());
					m_pDesertStage[1]->SetView(GetView());
					m_pDesertStage[1]->SetTexture();
					m_pDesertStage[1]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pDesertStage[1]->SetSize(180.0f, 100.0f, 100.0f);
					m_pDesertStage[1]->Disp();
					break;

				case(DESERT_STAGE3):

					Sprite::ReSetSprite();
					m_pDesertStage[2]->SetProjection(GetProj());
					m_pDesertStage[2]->SetView(GetView());
					m_pDesertStage[2]->SetTexture();
					m_pDesertStage[2]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pDesertStage[2]->SetSize(180.0f, 100.0f, 100.0f);
					m_pDesertStage[2]->Disp();
					break;
				case(SNOWFIELD_STAGE1):

					Sprite::ReSetSprite();
					m_pSnowFieldStage[0]->SetProjection(GetProj());
					m_pSnowFieldStage[0]->SetView(GetView());
					m_pSnowFieldStage[0]->SetTexture();
					m_pSnowFieldStage[0]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pSnowFieldStage[0]->SetSize(180.0f, 100.0f, 100.0f);
					m_pSnowFieldStage[0]->Disp();
					break;

				case(SNOWFIELD_STAGE2):

					Sprite::ReSetSprite();
					m_pSnowFieldStage[1]->SetProjection(GetProj());
					m_pSnowFieldStage[1]->SetView(GetView());
					m_pSnowFieldStage[1]->SetTexture();
					m_pSnowFieldStage[1]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pSnowFieldStage[1]->SetSize(180.0f, 100.0f, 100.0f);
					m_pSnowFieldStage[1]->Disp();
					break;

				case(SNOWFIELD_STAGE3):

					Sprite::ReSetSprite();
					m_pSnowFieldStage[2]->SetProjection(GetProj());
					m_pSnowFieldStage[2]->SetView(GetView());
					m_pSnowFieldStage[2]->SetTexture();
					m_pSnowFieldStage[2]->SetPositon(0.0f, 50.0f, 50.0f);
					m_pSnowFieldStage[2]->SetSize(180.0f, 100.0f, 100.0f);
					m_pSnowFieldStage[2]->Disp();
					break;
			
			}
		}
	}
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
