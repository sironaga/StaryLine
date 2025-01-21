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
	, posX{ 0.0f,400.0f,800.0f }
	, subposX{ 0.0f,400.0f,800.0f }
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

	m_pStageSelected     = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Selected.png");

    m_pRight_Select      = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Right.png");
    m_pLeft_Select       = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Left.png");
	
	m_pStageSelect_Underbar = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Underbar.png");
	m_pModel[GrassField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage01_GrassField.fbx"), false);
	m_pModel[DesertField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage02_Desert.fbx"), false);
	m_pModel[SnowField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage03_SnowField.fbx"), false);
	m_pModel[WorldField] = new CModelEx(MODEL_PASS("StageSelect/WorldSelect_ForBeta.fbx"), false);

	g_StageSelectSound = new CSoundList(BGM_TITLE);
	m_pSourseStageSelectBGM = g_StageSelectSound->GetSound(true);
	m_pSourseStageSelectBGM->Start();

	nSlect = 0;
	for (int i = 0; i < StageKindMax; i++)
	{
		m_ModelParam[i].pos = { FIRST_POS,0.0f,100.0f };
		m_ModelParam[i].rotate = { DirectX::XMConvertToRadians(10.0f),DirectX::XMConvertToRadians(180.0f),0.0f };
		m_ModelParam[i].size = { -2.0f,2.0f,2.0f };


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
	m_pStageSelected->SetProjection(GetProj());
	m_pStageSelected->SetView(GetView());
	m_pStageSelect_Underbar->SetProjection(GetProj());
	m_pStageSelect_Underbar->SetView(GetView());

	}

	m_ModelParam[WorldField].pos = { 0.0f,0.0f,100.0f };
	m_pBackGround = new CBackGround();

}

CStageSelect::~CStageSelect()
{
	if (m_pSourseStageSelectBGM)
	{
		m_pSourseStageSelectBGM->Stop();
		m_pSourseStageSelectBGM = nullptr;
	}
	if (g_StageSelectSound)
	{
		delete g_StageSelectSound;
		g_StageSelectSound = nullptr;
	}
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
	if (m_pStageSelect_Underbar)
	{
		delete m_pStageSelect_Underbar;
		m_pStageSelect_Underbar = nullptr;
	}
	if (m_pStageSelected)
	{
		delete m_pStageSelected;
		m_pStageSelected = nullptr;
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
	
	for (int i = 0; i < StageKindMax; i++)
	{
		SAFE_DELETE(m_pModel[i]);
	}

	SAFE_DELETE(m_pBackGround);
}

void CStageSelect::Update()
{
	if (m_pSourseStageSelectBGM)SetVolumeBGM(m_pSourseStageSelectBGM);

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
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) { g_Select_type.StageMainNumber = DESERT; m_bMoving = true; }
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					break;
				case(DESERT):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) { g_Select_type.StageMainNumber = SNOWFIELD; m_bMoving = true; }
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) { g_Select_type.StageMainNumber = GRASSLAND;	m_bMoving = true; }
					g_Select_type.StageSubNumber = DESERT_STAGE1;
					break;
				case(SNOWFIELD):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) { g_Select_type.StageMainNumber = DESERT; m_bMoving = true; }
					g_Select_type.StageSubNumber = SNOWFIELD_STAGE1;
					break;
				}
				if (IsKeyTrigger(VK_RETURN) || (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A)))
				{
					StartFade();
					MainStage ^= true;

					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };
					m_ModelParam[g_Select_type.StageMainNumber].pos = { FIRST_POS,0.0f,100.0f };

				}
				if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
				{
					SetNext(SCENE_TITLE, g_Select_type);
					m_bEnd = true;
				}
			}
			else
			{
			if (g_Select_type.StageMainNumber == GRASSLAND)
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(GRASSLAND_STAGE1):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) { g_Select_type.StageSubNumber = GRASSLAND_STAGE2; m_bMoving = true; }
					break;

				case(GRASSLAND_STAGE2):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) {
						g_Select_type.StageSubNumber = GRASSLAND_STAGE3; m_bMoving = true;
					}
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = GRASSLAND_STAGE1; m_bMoving = true;
					}
					break;

				case(GRASSLAND_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = GRASSLAND_STAGE2; m_bMoving = true;
					}
					break;
				}
			}
			if (g_Select_type.StageMainNumber == DESERT)
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(DESERT_STAGE1):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) { g_Select_type.StageSubNumber = DESERT_STAGE2; m_bMoving = true; }
					break;

				case(DESERT_STAGE2):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) {
						g_Select_type.StageSubNumber = DESERT_STAGE3; m_bMoving = true;
					}
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = DESERT_STAGE1; m_bMoving = true;
					}
					break;

				case(DESERT_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = DESERT_STAGE2; m_bMoving = true;
					}
					break;
				}
			}
			if (g_Select_type.StageMainNumber == SNOWFIELD)
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(SNOWFIELD_STAGE1):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) { g_Select_type.StageSubNumber = SNOWFIELD_STAGE2; m_bMoving = true; }
					break;

				case(SNOWFIELD_STAGE2):
					if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) {
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE3; m_bMoving = true;
					}
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE1; m_bMoving = true;
					}
					break;

				case(SNOWFIELD_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) {
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE2; m_bMoving = true;
					}
					break;
				}
			}
				if (IsKeyTrigger(VK_RETURN) || (IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A)))
				{
					CSceneResult::InStageLevel(g_Select_type);
					SetNext(SCENE_GAME, g_Select_type);
					m_bEnd = true;
				}
				if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
				{
					//StartFade();
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					subposX[0] = 0.0f; subposX[1] = 400.0f; subposX[2] = 800.0f;
					MainStage ^= true;
					m_ModelParam[WorldField].pos = { 0.0f - g_Select_type.StageMainNumber * STAGE_BETWEEN / 3.0f,0.0f,100.0f };

				}
			}//V[Ús
			if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT)))
			{
				bRight = true;
			}
			if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)))
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
			if (bRight)
			{
				m_ModelParam[i].pos.x -= STAGE_BETWEEN / 3.0f / MOVE_TIME;
			}
			else
			{
				m_ModelParam[i].pos.x += STAGE_BETWEEN / 3.0f / MOVE_TIME;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			if (MainStage)
			{
				if (bRight)
				{
					posX[i] -= 400.0f / MOVE_TIME;
				}
				else
				{
					posX[i] += 400.0f / MOVE_TIME;
				}
			}
			else
			{
				if (bRight)
				{
					subposX[i] -= 400.0f / MOVE_TIME;
				}
				else
				{
					subposX[i] += 400.0f / MOVE_TIME;
				}
			}
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
	//static float deg = 0.0f;    //度数
	//deg += 5.0f;    // 度数を増やし続ける
	//	float rad = DirectX::XMConvertToRadians(deg);    //ラジアンに変換する
	//// 角度を増やしていく度にsinが変化していく
	//// それを使いスケールをかけていく
	//float ScaleX = (sinf(rad) * 0.5f + 0.5f) * 10.0f + 40.0f;
	//float ScaleY = (sinf(rad + PI) * 0.5f + 0.5f) * 10.0f + 40.0f;

	static float deg = 0.0f;    //度数
	deg += 4.0f;    // 度数を増やし続ける
		float rad = DirectX::XMConvertToRadians(deg);    //ラジアンに変換する
	float cosAngle = cosf(rad);
	float Scale1 = 1.0f + cosAngle / 5.0f;
	float ArrowSize1 = 30.0f * Scale1;

	float Scale2 = 1.05f + cosAngle / 30.0f;
	float ArrowSize2 = 220.0f * Scale2;
	float ArrowSize3 = 162.0f * Scale2;

	float Scale3 = 1.05f + cosAngle / 30.0f;
	float ArrowSize4 = 220.0f * Scale2;
	float ArrowSize5 = 140.0f * Scale2;

	m_pBackGround->Draw();
	// f`æ
	
	SetRender2D;
	Sprite::ReSetSprite();
	m_pStageSelect_Underbar->SetProjection(Get2DProj());
	m_pStageSelect_Underbar->SetView(Get2DView());
	m_pStageSelect_Underbar->SetTexture();
	m_pStageSelect_Underbar->SetPositon(CENTER_POS_X, CENTER_POS_Y + 510, 0.0f);
	m_pStageSelect_Underbar->SetSize(1920.0f, -60.0f, 0.0f);
	m_pStageSelect_Underbar->Disp();

	//if (g_Select_type.StageSubNumber == GRASSLAND_STAGE1)
	//{
	//Sprite::ReSetSprite();
	//m_pStageSelect_Underbar->SetProjection(Get2DProj());
	//m_pStageSelect_Underbar->SetView(Get2DView());
	//m_pStageSelect_Underbar->SetTexture();
	//m_pStageSelect_Underbar->SetPositon(CENTER_POS_X, CENTER_POS_Y + 510, 0.0f);
	//m_pStageSelect_Underbar->SetSize(1920.0f, -60.0f, 0.0f);
	//m_pStageSelect_Underbar->Disp();
	//}
	//if (g_Select_type.StageSubNumber == GRASSLAND_STAGE2)
	//{
	//	Sprite::ReSetSprite();
	//	m_pStageSelect_Underbar->SetProjection(Get2DProj());
	//	m_pStageSelect_Underbar->SetView(Get2DView());
	//	m_pStageSelect_Underbar->SetTexture();
	//	m_pStageSelect_Underbar->SetPositon(CENTER_POS_X, CENTER_POS_Y, 0.0f);
	//	m_pStageSelect_Underbar->SetSize(1920.0f, -60.0f, 0.0f);
	//	m_pStageSelect_Underbar->Disp();
	//}
	//if (g_Select_type.StageSubNumber == GRASSLAND_STAGE3)
	//{
	//	Sprite::ReSetSprite();
	//	m_pStageSelect_Underbar->SetProjection(Get2DProj());
	//	m_pStageSelect_Underbar->SetView(Get2DView());
	//	m_pStageSelect_Underbar->SetTexture();
	//	m_pStageSelect_Underbar->SetPositon(CENTER_POS_X, CENTER_POS_Y - 510, 0.0f);
	//	m_pStageSelect_Underbar->SetSize(1920.0f, -60.0f, 0.0f);
	//	m_pStageSelect_Underbar->Disp();
	//}

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


	SetRender3D();
	if (MainStage)
	{
		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 80.0f, 145.0f);
			m_pGrassLand->SetSize(220.0f, 140.0f, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 80.0f, 145.0f);
			m_pDesert->SetSize(210.0f, 130.0f, 100.0f);
			m_pDesert->Disp();

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pRight_Select->SetProjection(GetProj());
				m_pRight_Select->SetView(GetView());
				m_pRight_Select->SetTexture();
				m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
				m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
				m_pRight_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(posX[0], 80.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize2, ArrowSize3, 100.0f);
				m_pStageSelected->Disp();
			}
			break;
		case(DESERT):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 80.0f, 145.0f);
			m_pGrassLand->SetSize(210.0f, 130.0f, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 80.0f, 145.0f);
			m_pDesert->SetSize(220.0f, 140.0f, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 80.0f, 145.0f);
			m_pSnowField->SetSize(210.0f, 130.0f, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1,ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				Sprite::ReSetSprite();
				m_pRight_Select->SetProjection(GetProj());
				m_pRight_Select->SetView(GetView());
				m_pRight_Select->SetTexture();
				m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
				m_pRight_Select->SetSize(ArrowSize1,ArrowSize1, 100.0f);
				m_pRight_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(posX[1], 80.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize2, ArrowSize3, 100.0f);
				m_pStageSelected->Disp();
			}
			break;
		case(SNOWFIELD):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 80.0f, 145.0f);
			m_pDesert->SetSize(210.0f, 130.0f, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 80.0f, 145.0f);
			m_pSnowField->SetSize(220.0f, 140.0f, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pLeft_Select->SetProjection(GetProj());
				m_pLeft_Select->SetView(GetView());
				m_pLeft_Select->SetTexture();
				m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
				m_pLeft_Select->SetSize(ArrowSize1,ArrowSize1, 100.0f);
				m_pLeft_Select->Disp();

				SetRender2D();
				Sprite::ReSetSprite();
				m_pStageSelected->SetProjection(GetProj());
				m_pStageSelected->SetView(GetView());
				m_pStageSelected->SetTexture();
				m_pStageSelected->SetPositon(posX[2], 80.0f, 140.0f);
				m_pStageSelected->SetSize(ArrowSize2, ArrowSize3, 100.0f);
				m_pStageSelected->Disp();
			}
			break;
		}
	}
	else
	{
		{
			switch (g_Select_type.StageSubNumber)
			{
				case(GRASSLAND_STAGE1):
					SetRender3D();
					Sprite::ReSetSprite();
					m_pGrassLandStage[0]->SetProjection(GetProj());
					m_pGrassLandStage[0]->SetView(GetView());
					m_pGrassLandStage[0]->SetTexture();
					m_pGrassLandStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pGrassLandStage[0]->SetSize(220.0f, 140.0f, 100.0f);
					m_pGrassLandStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pGrassLandStage[1]->SetProjection(GetProj());
					m_pGrassLandStage[1]->SetView(GetView());
					m_pGrassLandStage[1]->SetTexture();
					m_pGrassLandStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					//if (!m_bMoving)
					m_pGrassLandStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pGrassLandStage[1]->Disp();

					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[0], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}
					break;

				case(GRASSLAND_STAGE2):
					SetRender3D();
					Sprite::ReSetSprite();
					m_pGrassLandStage[0]->SetProjection(GetProj());
					m_pGrassLandStage[0]->SetView(GetView());
					m_pGrassLandStage[0]->SetTexture();
					m_pGrassLandStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pGrassLandStage[0]->SetSize(210.0f, 130.0f, 100.0f);
					m_pGrassLandStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pGrassLandStage[1]->SetProjection(GetProj());
					m_pGrassLandStage[1]->SetView(GetView());
					m_pGrassLandStage[1]->SetTexture();
					m_pGrassLandStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pGrassLandStage[1]->SetSize(220.0f, 140.0f, 100.0f);
					m_pGrassLandStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pGrassLandStage[2]->SetProjection(GetProj());
					m_pGrassLandStage[2]->SetView(GetView());
					m_pGrassLandStage[2]->SetTexture();
					m_pGrassLandStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pGrassLandStage[2]->SetSize(210.0f, 130.0f, 100.0f);
					m_pGrassLandStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[1], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}
					break;

				case(GRASSLAND_STAGE3):
					SetRender3D();
					Sprite::ReSetSprite();
					m_pGrassLandStage[1]->SetProjection(GetProj());
					m_pGrassLandStage[1]->SetView(GetView());
					m_pGrassLandStage[1]->SetTexture();
					m_pGrassLandStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pGrassLandStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pGrassLandStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pGrassLandStage[2]->SetProjection(GetProj());
					m_pGrassLandStage[2]->SetView(GetView());
					m_pGrassLandStage[2]->SetTexture();
					m_pGrassLandStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pGrassLandStage[2]->SetSize(220.0f, 140.0f, 100.0f);
					m_pGrassLandStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[2], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}
					break;
				case(DESERT_STAGE1):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pDesertStage[0]->SetProjection(GetProj());
					m_pDesertStage[0]->SetView(GetView());
					m_pDesertStage[0]->SetTexture();
					m_pDesertStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pDesertStage[0]->SetSize(220.0f, 140.0f, 100.0f);
					m_pDesertStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pDesertStage[1]->SetProjection(GetProj());
					m_pDesertStage[1]->SetView(GetView());
					m_pDesertStage[1]->SetTexture();
					m_pDesertStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pDesertStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pDesertStage[1]->Disp();

					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[0], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}
					break;

				case(DESERT_STAGE2):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pDesertStage[0]->SetProjection(GetProj());
					m_pDesertStage[0]->SetView(GetView());
					m_pDesertStage[0]->SetTexture();
					m_pDesertStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pDesertStage[0]->SetSize(210.0f, 130.0f, 100.0f);
					m_pDesertStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pDesertStage[1]->SetProjection(GetProj());
					m_pDesertStage[1]->SetView(GetView());
					m_pDesertStage[1]->SetTexture();
					m_pDesertStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pDesertStage[1]->SetSize(220.0f, 140.0f, 100.0f);
					m_pDesertStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pDesertStage[2]->SetProjection(GetProj());
					m_pDesertStage[2]->SetView(GetView());
					m_pDesertStage[2]->SetTexture();
					m_pDesertStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pDesertStage[2]->SetSize(210.0f, 130.0f, 100.0f);
					m_pDesertStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[1], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}

					break;

				case(DESERT_STAGE3):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pDesertStage[1]->SetProjection(GetProj());
					m_pDesertStage[1]->SetView(GetView());
					m_pDesertStage[1]->SetTexture();
					m_pDesertStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pDesertStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pDesertStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pDesertStage[2]->SetProjection(GetProj());
					m_pDesertStage[2]->SetView(GetView());
					m_pDesertStage[2]->SetTexture();
					m_pDesertStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pDesertStage[2]->SetSize(220.0f, 140.0f, 100.0f);
					m_pDesertStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[2], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}

					break;
				case(SNOWFIELD_STAGE1):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pSnowFieldStage[0]->SetProjection(GetProj());
					m_pSnowFieldStage[0]->SetView(GetView());
					m_pSnowFieldStage[0]->SetTexture();
					m_pSnowFieldStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pSnowFieldStage[0]->SetSize(220.0f, 140.0f, 100.0f);
					m_pSnowFieldStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pSnowFieldStage[1]->SetProjection(GetProj());
					m_pSnowFieldStage[1]->SetView(GetView());
					m_pSnowFieldStage[1]->SetTexture();
					m_pSnowFieldStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pSnowFieldStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pSnowFieldStage[1]->Disp();

					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[0], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}
					break;

				case(SNOWFIELD_STAGE2):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pSnowFieldStage[0]->SetProjection(GetProj());
					m_pSnowFieldStage[0]->SetView(GetView());
					m_pSnowFieldStage[0]->SetTexture();
					m_pSnowFieldStage[0]->SetPositon(subposX[0], 80.0f, 145.0f);
					m_pSnowFieldStage[0]->SetSize(210.0f, 130.0f, 100.0f);
					m_pSnowFieldStage[0]->Disp();

					Sprite::ReSetSprite();
					m_pSnowFieldStage[1]->SetProjection(GetProj());
					m_pSnowFieldStage[1]->SetView(GetView());
					m_pSnowFieldStage[1]->SetTexture();
					m_pSnowFieldStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pSnowFieldStage[1]->SetSize(220.0f, 140.0f, 100.0f);
					m_pSnowFieldStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pSnowFieldStage[2]->SetProjection(GetProj());
					m_pSnowFieldStage[2]->SetView(GetView());
					m_pSnowFieldStage[2]->SetTexture();
					m_pSnowFieldStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pSnowFieldStage[2]->SetSize(210.0f, 130.0f, 100.0f);
					m_pSnowFieldStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						Sprite::ReSetSprite();
						m_pRight_Select->SetProjection(GetProj());
						m_pRight_Select->SetView(GetView());
						m_pRight_Select->SetTexture();
						m_pRight_Select->SetPositon(180.0f, 80.0f, 145.0f);
						m_pRight_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pRight_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[1], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}

					break;

				case(SNOWFIELD_STAGE3):

					SetRender3D();
					Sprite::ReSetSprite();
					m_pSnowFieldStage[1]->SetProjection(GetProj());
					m_pSnowFieldStage[1]->SetView(GetView());
					m_pSnowFieldStage[1]->SetTexture();
					m_pSnowFieldStage[1]->SetPositon(subposX[1], 80.0f, 145.0f);
					m_pSnowFieldStage[1]->SetSize(210.0f, 130.0f, 100.0f);
					m_pSnowFieldStage[1]->Disp();

					Sprite::ReSetSprite();
					m_pSnowFieldStage[2]->SetProjection(GetProj());
					m_pSnowFieldStage[2]->SetView(GetView());
					m_pSnowFieldStage[2]->SetTexture();
					m_pSnowFieldStage[2]->SetPositon(subposX[2], 80.0f, 145.0f);
					m_pSnowFieldStage[2]->SetSize(220.0f, 140.0f, 100.0f);
					m_pSnowFieldStage[2]->Disp();
					if (m_bMoving == false)
					{
						Sprite::ReSetSprite();
						m_pLeft_Select->SetProjection(GetProj());
						m_pLeft_Select->SetView(GetView());
						m_pLeft_Select->SetTexture();
						m_pLeft_Select->SetPositon(-180.0f, 80.0f, 145.0f);
						m_pLeft_Select->SetSize(ArrowSize1, ArrowSize1, 100.0f);
						m_pLeft_Select->Disp();

						SetRender2D();
						Sprite::ReSetSprite();
						m_pStageSelected->SetProjection(GetProj());
						m_pStageSelected->SetView(GetView());
						m_pStageSelected->SetTexture();
						m_pStageSelected->SetPositon(subposX[2], 80.0f, 140.0f);
						m_pStageSelected->SetSize(ArrowSize4, ArrowSize5, 100.0f);
						m_pStageSelected->Disp();
					}

					break;
			
			}
		}
	}
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
