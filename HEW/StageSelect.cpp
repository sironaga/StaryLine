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

constexpr float STAGE_BETWEEN = 1100.0f;
constexpr float MOVE_TIME = 100.0f;
constexpr float FIRST_POS = 200.0f;

CStageSelect::CStageSelect()
	: f_Rotation(0)
	, f_Rad(0)
	, f_SelectX(350)
	, f_SelectY(350)
	, MainStage(true)
	, m_bEnd(false)
	, m_pModel{}
	, m_ModelParam{}
	, m_pBackGround(nullptr)
	, m_bMoving(false)
{
	g_Select_type.StageMainNumber = GRASSLAND;
	g_Select_type.StageSubNumber = STAGE1; 
	m_pGrassLand         = new SpriteEx("Assets/Texture/StageSelectBackGround/Grass_Map.png");
	m_pDesert            = new SpriteEx("Assets/Texture/StageSelectBackGround/Desert_Map.png");
	m_pSnowField         = new SpriteEx("Assets/Texture/StageSelectBackGround/Snow_Map.png");
	m_pGrassLandBack     = new SpriteEx("Assets/Texture/StageSelectBackGround/Grass_Map_Back.png");
	m_pDesertBack        = new SpriteEx("Assets/Texture/StageSelectBackGround/Desert_Map_Back.png");
	m_pSnowFieldBack     = new SpriteEx("Assets/Texture/StageSelectBackGround/Snow_Map_Back.png");
	m_pGrassLandStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pGrassLandStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pGrassLandStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
	m_pGrassLandStageBack[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/STAGE1_Back.png");
	m_pGrassLandStageBack[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/STAGE2_Back.png");
	m_pDesertStage[0]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pDesertStage[1]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pDesertStage[2]    = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
	m_pSnowFieldStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE1.png");
	m_pSnowFieldStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE2.png");
	m_pSnowFieldStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/1_STAGE3.png");
    m_pRight_Select      = new SpriteEx("Assets/Texture/StageSelectBackGround/Right_Select.png");
    m_pLeft_Select       = new SpriteEx("Assets/Texture/StageSelectBackGround/Left_Select.png");

	m_pModel[GrassField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage01_GrassField.fbx"), false);
	m_pModel[DesertField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage02_Desert.fbx"), false);
	m_pModel[SnowField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage03_SnowField.fbx"), false);
	m_pModel[WorldField] = new CModelEx(MODEL_PASS("StageSelect/WorldSelect_ForBeta.fbx"), false);



	nSlect = 0;
	for (int i = 0; i < StageKindMax; i++)
	{
		m_ModelParam[i].pos = { FIRST_POS,0.0f,100.0f };
		m_ModelParam[i].rotate = { DirectX::XMConvertToRadians(10.0f),DirectX::XMConvertToRadians(180.0f),0.0f };
		m_ModelParam[i].size = { -2.0f,2.0f,2.0f };

	}

	m_ModelParam[WorldField].pos = { 0.0f,0.0f,100.0f };
	m_pBackGround = new CBackGround();
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
	if (m_pGrassLandStageBack[0])
	{
		delete m_pGrassLandStageBack[0];
		m_pGrassLandStageBack[0] = nullptr;
	}
	if (m_pGrassLandStageBack[1])
	{
		delete m_pGrassLandStageBack[1];
		m_pGrassLandStageBack[1] = nullptr;
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
	if (m_pGrassLandBack)
	{
		delete m_pGrassLandBack;
		m_pGrassLandBack = nullptr;
	}
	if (m_pDesertBack)
	{
		delete m_pDesertBack;
		m_pDesertBack = nullptr;
	}
	if (m_pSnowFieldBack)
	{
		delete m_pSnowFieldBack;
		m_pSnowFieldBack = nullptr;
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
	
	for (int i = 0; i < StageKindMax; i++)
	{
		SAFE_DELETE(m_pModel[i]);
	}

	SAFE_DELETE(m_pBackGround);
}

void CStageSelect::Update()
{
	static bool bRight = false;/*
	SpriteDebug(&m_ModelParam,true);*/
	if (!m_bEnd)
	{
		if (!m_bMoving)
		{
			if (MainStage)
			{
				switch (g_Select_type.StageMainNumber)
				{
				case(GRASSLAND):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = DESERT; m_bMoving = true; }

					break;
				case(GRASSLANDBACK):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = DESERT; m_bMoving = true; }

					break;
				case(DESERT):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = SNOWFIELD; m_bMoving = true; }
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = GRASSLANDBACK;	m_bMoving = true; }

					break;
				case(DESERTBACK):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageMainNumber = SNOWFIELD; m_bMoving = true; }
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = GRASSLANDBACK; m_bMoving = true; }

					break;
				case(SNOWFIELD):
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) { g_Select_type.StageMainNumber = DESERTBACK; m_bMoving = true; }

					break;
				}
				if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
				{
					switch (g_Select_type.StageMainNumber)
					{
					case(GRASSLANDBACK):
						g_Select_type.StageMainNumber = GRASSLAND;
						break;
					case(DESERTBACK):
						g_Select_type.StageMainNumber = DESERT;
						break;
					}
					StartFade();
					MainStage ^= true;

					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };

				}
				if (IsKeyTrigger(VK_BACK) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
				{
					SetNext(SCENE_TITLE, g_Select_type);
					m_bEnd = true;
				}
			}
			else
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(STAGE1):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) { g_Select_type.StageSubNumber = STAGE2; m_bMoving = true; }
					break;

				case(STAGE2):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) {
						g_Select_type.StageSubNumber = STAGE3; m_bMoving = true;
					}
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) {
						g_Select_type.StageSubNumber = STAGE1BACK; m_bMoving = true;
					}
					break;

				case(STAGE3):
					//if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE1; }
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) {
						g_Select_type.StageSubNumber = STAGE2BACK; m_bMoving = true;
					}
					break;
				case(STAGE1BACK):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) {
						g_Select_type.StageSubNumber = STAGE2; m_bMoving = true;
					}
					break;
				case(STAGE2BACK):
					if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)) {
						g_Select_type.StageSubNumber = STAGE3; m_bMoving = true;
					}
					if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)) {
						g_Select_type.StageSubNumber = STAGE1BACK; m_bMoving = true;
					}
				default:break;
				}
				if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
				{
					switch (g_Select_type.StageSubNumber)
					{
					case(STAGE1BACK):
						g_Select_type.StageSubNumber = STAGE1;
						break;
					case(STAGE2BACK):
						g_Select_type.StageSubNumber = STAGE2;
						break;
					}
					CSceneResult::InStageLevel(g_Select_type);
					SetNext(SCENE_GAME, g_Select_type);
					m_bEnd = true;
				}
				if (IsKeyTrigger(VK_BACK) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
				{
					g_Select_type.StageSubNumber = STAGE1;
					MainStage ^= true;
					m_ModelParam[WorldField].pos = { 0.0f - g_Select_type.StageMainNumber * STAGE_BETWEEN / 3.0f,0.0f,100.0f };

				}
			}//シーン移行
			if (IsKeyTrigger(VK_RIGHT))
			{
				bRight = true;
			}
			if (IsKeyTrigger(VK_LEFT))
			{
				bRight = false;
			}
		}
	}
	static int moveCnt = 0;
	if (m_bMoving)
	{
		moveCnt++;

		for (int i = 0; i < StageKindMax; i++)
		{
			if (bRight)m_ModelParam[i].pos.x -= STAGE_BETWEEN / 3.0f / MOVE_TIME;
			else m_ModelParam[i].pos.x += STAGE_BETWEEN / 3.0f / MOVE_TIME;
		}
		if (moveCnt >= MOVE_TIME)
		{
			m_bMoving = false;
			moveCnt = 0;
		}
	}
	//m_bMoving = false;
}

void CStageSelect::Draw()
{
	m_pBackGround->Draw();
	// モデル描画
	SetRender3D();
	if (MainStage)
	{
		m_pModel[WorldField]->SetPostion(m_ModelParam[WorldField].pos.x, m_ModelParam[WorldField].pos.y, m_ModelParam[WorldField].pos.z);
		m_pModel[WorldField]->SetRotation(m_ModelParam[WorldField].rotate.x, m_ModelParam[WorldField].rotate.y, m_ModelParam[WorldField].rotate.z);
		m_pModel[WorldField]->SetScale(m_ModelParam[WorldField].size.x, m_ModelParam[WorldField].size.y, m_ModelParam[WorldField].size.z);
		m_pModel[WorldField]->SetViewMatrix(GetView());
		m_pModel[WorldField]->SetProjectionMatrix(GetProj());
		m_pModel[WorldField]->Draw();
	}
	else
	{
		switch (g_Select_type.StageMainNumber)
		{
		case GrassField:
			m_pModel[GrassField]->SetPostion(m_ModelParam[GrassField].pos.x, m_ModelParam[GrassField].pos.y, m_ModelParam[GrassField].pos.z);
			m_pModel[GrassField]->SetRotation(m_ModelParam[GrassField].rotate.x, m_ModelParam[GrassField].rotate.y, m_ModelParam[GrassField].rotate.z);
			m_pModel[GrassField]->SetScale(m_ModelParam[GrassField].size.x, m_ModelParam[GrassField].size.y, m_ModelParam[GrassField].size.z);
			m_pModel[GrassField]->SetViewMatrix(GetView());
			m_pModel[GrassField]->SetProjectionMatrix(GetProj());
			m_pModel[GrassField]->Draw();
			break;
		case DesertField:
			m_pModel[DesertField]->SetPostion(m_ModelParam[DesertField].pos.x, m_ModelParam[DesertField].pos.y, m_ModelParam[DesertField].pos.z);
			m_pModel[DesertField]->SetRotation(m_ModelParam[DesertField].rotate.x, m_ModelParam[DesertField].rotate.y, m_ModelParam[DesertField].rotate.z);
			m_pModel[DesertField]->SetScale(m_ModelParam[DesertField].size.x, m_ModelParam[DesertField].size.y, m_ModelParam[DesertField].size.z);
			m_pModel[DesertField]->SetViewMatrix(GetView());
			m_pModel[DesertField]->SetProjectionMatrix(GetProj());
			m_pModel[DesertField]->Draw();
			break;
		case SnowField:	
			m_pModel[SnowField]->SetPostion(m_ModelParam[SnowField].pos.x, m_ModelParam[SnowField].pos.y, m_ModelParam[SnowField].pos.z);
			m_pModel[SnowField]->SetRotation(m_ModelParam[SnowField].rotate.x, m_ModelParam[SnowField].rotate.y, m_ModelParam[SnowField].rotate.z);
			m_pModel[SnowField]->SetScale(m_ModelParam[SnowField].size.x, m_ModelParam[SnowField].size.y, m_ModelParam[SnowField].size.z);
			m_pModel[SnowField]->SetViewMatrix(GetView());
			m_pModel[SnowField]->SetProjectionMatrix(GetProj());
			m_pModel[SnowField]->Draw();
			break;
		default:
			break;
		}
	}


	SetRender2D();
	/*if (MainStage)
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
		case(GRASSLANDBACK):

			Sprite::ReSetSprite();
			m_pGrassLandBack->SetProjection(Get2DProj());
			m_pGrassLandBack->SetView(Get2DView());
			m_pGrassLandBack->SetTexture();
			m_pGrassLandBack->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandBack->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandBack->Disp();
			break;
		case(DESERTBACK):

			Sprite::ReSetSprite();
			m_pDesertBack->SetProjection(Get2DProj());
			m_pDesertBack->SetView(Get2DView());
			m_pDesertBack->SetTexture();
			m_pDesertBack->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pDesertBack->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pDesertBack->Disp();
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
		
		case(STAGE1BACK):

			Sprite::ReSetSprite();
			m_pGrassLandStageBack[0]->SetProjection(Get2DProj());
			m_pGrassLandStageBack[0]->SetView(Get2DView());
			m_pGrassLandStageBack[0]->SetTexture();
			m_pGrassLandStageBack[0]->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandStageBack[0]->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandStageBack[0]->Disp();
			break;
		case(STAGE2BACK):

			Sprite::ReSetSprite();
			m_pGrassLandStageBack[1]->SetProjection(Get2DProj());
			m_pGrassLandStageBack[1]->SetView(Get2DView());
			m_pGrassLandStageBack[1]->SetTexture();
			m_pGrassLandStageBack[1]->SetPositon(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
			m_pGrassLandStageBack[1]->SetSize(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f);
			m_pGrassLandStageBack[1]->Disp();
			break;
		}
	}*/
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
