#include "StageSelect.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Defines.h"
#include "SpriteDrawer.h"
#include "InputEx.h"
#include "SoundList.h"
#include "SceneResult.h"
#include "Geometory.h"

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

#define WORLD_SIZE_X 85.0f
#define WORLD_SIZE_Y 38.0f

#define GRASS_ROTATE_Z2 0.0f

#define GRASS_ROTATE_X 21.0f
#define GRASS_ROTATE_Y 0.0f
#define GRASS_ROTATE_Z 360.0f
#define DESERT_ROTATE_X -11.0f
#define DESERT_ROTATE_Y -21.0f
#define DESERT_ROTATE_Z 122.0f
#define SNOW_ROTATE_X -12.0f
#define SNOW_ROTATE_Y 22.0f
#define SNOW_ROTATE_Z 238.0f
#define LINIE_ROTATE_FLONT 270.0f

StageType g_Select_type;
StageType g_OldSelect_type;
Model::AnimeNo g_AnimNo;

constexpr float STAGE_BETWEEN = 1100.0f;
constexpr float MOVE_TIME = 30.0f;
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
	, m_rotate{}
	, m_pDecition(nullptr)
	, m_pEffect{}, m_pStarEfc{}
	, m_tBGRotateZ(0.0f)
	, m_ModelWorldParam{}
	, LinieRotationY{ LINIE_ROTATE_FLONT }
	, m_bClear{}
{
	g_Select_type.StageMainNumber = GRASSLAND;
	g_Select_type.StageSubNumber = GRASSLAND_STAGE1; 
	g_OldSelect_type = g_Select_type;
	m_pGrassLand         = new SpriteEx("Assets/Texture/StageSelectBackGround/GRASSLAND.png");
	m_pDesert            = new SpriteEx("Assets/Texture/StageSelectBackGround/DESERT.png");
	m_pSnowField         = new SpriteEx("Assets/Texture/StageSelectBackGround/SNOWFIELD.png");

	m_pGrassLandStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.1.png");
	m_pGrassLandStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.2.png");
	m_pGrassLandStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_1.3.png");
	m_pDesertStage[0]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.1.png");
	m_pDesertStage[1]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.2.png");
	m_pDesertStage[2]    = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_2.3.png");
	m_pSnowFieldStage[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.1.png");
	m_pSnowFieldStage[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.2.png");
	m_pSnowFieldStage[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Board_3.3.png");

	m_pWorld[0] = new SpriteEx("Assets/Texture/StageSelectBackGround/WORLD1.png");
	m_pWorld[1] = new SpriteEx("Assets/Texture/StageSelectBackGround/WORLD2.png");
	m_pWorld[2] = new SpriteEx("Assets/Texture/StageSelectBackGround/WORLD3.png");

	m_pStageSelect = new SpriteEx("Assets/Texture/StageSelectBackGround/STAGESELECT.png");
	m_pWorldSelect = new SpriteEx("Assets/Texture/StageSelectBackGround/WORLDSELECT.png");

	m_pDecition = new SpriteEx("Assets/Effect/Sprite/Decision.png");
	m_pStageSelected     = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Selected.png");
	m_pLock = new SpriteEx("Assets/Texture/StageSelectBackGround/Lock.png");

    m_pRight_Select      = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Right.png");
    m_pLeft_Select       = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Left.png");
	
	m_pStageSelect_Underbar = new SpriteEx("Assets/Texture/StageSelectBackGround/Stageselect_Underbar.png");
	m_pModel[GrassField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage01_GrassField.fbx"), false);
	m_pModel[DesertField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage02_Desert.fbx"), false);
	m_pModel[SnowField] = new CModelEx(MODEL_PASS("StageSelect/StageSelect_Stage03_SnowField.fbx"), false);
	m_pWorldModel = new Model(); 
	m_pWorldModel->Load(MODEL_PASS("StageSelect/WorldSelect_World.fbx"), 1.0f, Model::ZFlip);
	m_pEffect[(int)Effect::Star] = new CEffectManager_sp("Assets/Effect/Sprite/BackGround_ShootginStars.png", 8, 8, 3.0f);
	for (int i = 0; i < SELECT_MAX_STAR; i++)
	{
		m_pStarEfc[i] = new CEffectManager_sp(m_pEffect[(int)Effect::Star]);
	}

	m_pStageLinie = new Model();
	m_pStageLinie->Load(MODEL_PASS("Leader/Linie/Anim_Linie_SelectStage_Run.fbx"), 1.0f, Model::ZFlip);
	g_AnimNo = m_pStageLinie->AddAnimation(MODEL_PASS("Leader/Linie/Anim_Linie_SelectStage_Run.fbx"));
	m_pStageLinie->PlayAnime(g_AnimNo, false);
	m_pStageLinie->Step(0.01f);
	m_pStageLinie->SetAnimeTime(g_AnimNo, 0.0f);

	m_BGMSound = new CSoundList(BGM_TITLE);
	//m_BGMSound = new CSoundList(BGM_SELECT);
	m_BGMSound->SetMasterVolume();
	m_pSourseStageSelectBGM = m_BGMSound->GetSound(true);
	SetVolumeBGM(m_pSourseStageSelectBGM);
	m_pSourseStageSelectBGM->Start();
	m_pSESound[DECISION_SE] = new CSoundList(SE_DECISION);
	m_pSESound[GRASS_SE] = new CSoundList(SE_WALKGRASS);
	m_pSESound[DESERT_SE] = new CSoundList(SE_WALKDESERT);
	m_pSESound[SNOW_SE] = new CSoundList(SE_WALKSNOW);
	for (int i = 0; i < 4; i++)
	{
		m_pSESound[i]->SetMasterVolume();
		m_pSourseStageSelectSE[i] = m_pSESound[i]->GetSound(false);
		SetVolumeSE(m_pSourseStageSelectSE[i]);
	}

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
		m_pWorld[nLoop]->SetProjection(GetProj());
		m_pWorld[nLoop]->SetView(GetView());
	}
	m_pGrassLand->SetProjection(GetProj());
	m_pGrassLand->SetView(GetView());
	m_pDesert->SetProjection(GetProj());
	m_pDesert->SetView(GetView());
	m_pSnowField->SetProjection(GetProj());
	m_pSnowField->SetView(GetView());
	m_pStageSelect->SetProjection(GetProj());
	m_pStageSelect->SetView(GetView());
	m_pWorldSelect->SetProjection(GetProj());
	m_pWorldSelect->SetView(GetView());
	m_pRight_Select->SetProjection(GetProj());
	m_pRight_Select->SetView(GetView());
	m_pLeft_Select->SetProjection(GetProj());
	m_pLeft_Select->SetView(GetView());
	m_pStageSelected->SetProjection(GetProj());
	m_pStageSelected->SetView(GetView());
	m_pStageSelect_Underbar->SetProjection(GetProj());
	m_pStageSelect_Underbar->SetView(GetView());

	}

	m_ModelWorldParam.pos    = { 0.0f,-66.0f,100.0f };
	m_ModelWorldParam.size   = { 1.8f,1.8f,1.8f };
	m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) };

	m_pBackGround = new CBackGround();

	m_tDecitionPos[0] = { -375.0f, -55.0f };
	m_tDecitionPos[1] = {  375.0f, -385.0f };

	ClearCheck();
}

CStageSelect::~CStageSelect()
{
	if (m_pSourseStageSelectBGM)
	{
		m_pSourseStageSelectBGM->Stop();
		m_pSourseStageSelectBGM = nullptr;
	}
	SAFE_DELETE(m_pStageLinie);
	SAFE_DELETE(m_pWorldModel);
	if (m_BGMSound)
	{
		delete m_BGMSound;
		m_BGMSound = nullptr;
	}
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_pSESound[i]);
		m_pSourseStageSelectSE[i] = nullptr;
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
			if (m_pWorld[nLoop])
			{
				delete m_pWorld[nLoop];
				m_pWorld[nLoop] = nullptr;
			}
		}
		if (m_pStageSelect_Underbar)
		{
			delete m_pStageSelect_Underbar;
			m_pStageSelect_Underbar = nullptr;
		}
		if (m_pLock)
		{
			delete m_pLock;
			m_pLock = nullptr;
		}
		if (m_pStageSelected)
		{
			delete m_pStageSelected;
			m_pStageSelected = nullptr;
		}
		if (m_pStageSelect)
		{
			delete m_pStageSelect;
			m_pStageSelect = nullptr;
		}
		if (m_pStageSelect)
		{
			delete m_pStageSelect;
			m_pStageSelect = nullptr;
		}
		if (m_pWorldSelect)
		{
			delete m_pWorldSelect;
			m_pWorldSelect = nullptr;
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

	SAFE_DELETE(m_pDecition);

	for (int i = 0; i < (int)Effect::Max; i++)
	{
		SAFE_DELETE(m_pEffect[i]);
	}
	for (int i = 0; i < SELECT_MAX_STAR; i++)
	{
		m_pStarEfc[i] = nullptr;
	}
}

void CStageSelect::Update()
{
	if (m_pSourseStageSelectBGM)SetVolumeBGM(m_pSourseStageSelectBGM);

	static bool rotateSwap = false;
	static bool bRight = false;
	//SpriteDebug(&m_ModelParam[WorldField], true);
	//if (IsKeyTrigger('B'))m_ModelParam[WorldField].rotate.z += DirectX::XMConvertToRadians(120.0f);
	if (!m_bEnd)
	{
		if (!m_bMoving)
		{
			LinieRotationY = LINIE_ROTATE_FLONT;
			if (MainStage)
			{
				switch (g_Select_type.StageMainNumber)
				{
				case(GRASSLAND):
					posX[0] = 0.0f;
					posX[1] = 400.0f;
					posX[2] = 800.0f;
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[GRASSLAND_STAGE3])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = DESERT;
						m_bMoving = true; 
						m_rotate.z = GRASS_ROTATE_Z2;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = true;
					}
					else if ((IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) && m_bClear[DESERT_STAGE3])
					{ 
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = SNOWFIELD;	
						m_bMoving = true; 
						m_rotate.z = GRASS_ROTATE_Z;
						posX[2] = -400.0f;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = false;
					}
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					break;
				case(DESERT):
					posX[0] = -400.0f;
					posX[1] = 0.0f;
					posX[2] = 400.0f;
					//LinieRotationY = 180.0f;
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[DESERT_STAGE3])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = SNOWFIELD; 
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = true;
					}
					else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) 
					{ 
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = GRASSLAND;
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = false;
					}
					g_Select_type.StageSubNumber = DESERT_STAGE1;
					break;
				case(SNOWFIELD):
					posX[0] = -800.0f;
					posX[1] = -400.0f;
					posX[2] = 0.0f;
					//LinieRotationY = 180.0f;
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) 
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = DESERT;
						m_bMoving = true;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = false;
					}
					else if (IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) 
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageMainNumber = GRASSLAND; 
						m_bMoving = true; 
						posX[0] = 400.0f;
						m_pStageLinie->PlayAnime(g_AnimNo, false);
						bRight = true;
					};
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
				else if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
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
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[GRASSLAND_STAGE1])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = GRASSLAND_STAGE2;
						m_bMoving = true;
						bRight = true;
					}
					break;

				case(GRASSLAND_STAGE2):
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[GRASSLAND_STAGE2])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = GRASSLAND_STAGE3;
						m_bMoving = true;
						bRight = true;
					}
					else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) 
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
						m_bMoving = true;
						bRight = false;
					}
					break;

				case(GRASSLAND_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)))
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = GRASSLAND_STAGE2; m_bMoving = true;
						bRight = false;
					}
					break;
				}
			}
			if (g_Select_type.StageMainNumber == DESERT)
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(DESERT_STAGE1):
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[DESERT_STAGE1])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = DESERT_STAGE2; 
						m_bMoving = true;
						bRight = true;
					}
					break;

				case(DESERT_STAGE2):
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[DESERT_STAGE1])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = DESERT_STAGE3; 
						m_bMoving = true;
						bRight = true;
					}
					else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) 
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = DESERT_STAGE1;
						m_bMoving = true;
						bRight = false;
					}
					break;

				case(DESERT_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)))
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = DESERT_STAGE2;
						m_bMoving = true;
						bRight = false;
					}
					break;
				}
			}
			if (g_Select_type.StageMainNumber == SNOWFIELD)
			{
				switch (g_Select_type.StageSubNumber)
				{
				case(SNOWFIELD_STAGE1):
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[SNOWFIELD_STAGE1])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE2;
						m_bMoving = true;
						bRight = true;
					}
					break;

				case(SNOWFIELD_STAGE2):
					if ((IsKeyTrigger(VK_RIGHT) || (IsKeyTrigger('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))) && m_bClear[SNOWFIELD_STAGE2])
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE3;
						m_bMoving = true;
						bRight = true;
					}
					else if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT)))
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE1;
						m_bMoving = true;
						bRight = false;
					}
					break;

				case(SNOWFIELD_STAGE3):
					if (IsKeyTrigger(VK_LEFT) || (IsKeyTrigger('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))) 
					{
						g_OldSelect_type = g_Select_type;
						g_Select_type.StageSubNumber = SNOWFIELD_STAGE2;
						bRight = false;
						m_bMoving = true;
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
				else if (IsKeyTrigger(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B))
				{
					//StartFade();
					g_Select_type.StageSubNumber = GRASSLAND_STAGE1;
					subposX[0] = 0.0f; subposX[1] = 400.0f; subposX[2] = 800.0f;
					MainStage ^= true;
					
					m_ModelWorldParam.pos = { 0.0f,-66.0f,100.0f };
					m_ModelWorldParam.size = { 1.8f,1.8f,1.8f };
					switch (g_Select_type.StageMainNumber)
					{
					case(GRASSLAND):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) };
						break;
					case(DESERT):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(DESERT_ROTATE_Z) };
						break;
					case(SNOWFIELD):
						m_ModelWorldParam.rotate = { DirectX::XMConvertToRadians(21.0f),DirectX::XMConvertToRadians(0.0f),DirectX::XMConvertToRadians(SNOW_ROTATE_Z) };
						break;
					}

				}
			}
			m_rotate.x = m_ModelWorldParam.rotate.x;
			m_rotate.y = m_ModelWorldParam.rotate.y;
			if(g_Select_type.StageMainNumber != GRASSLAND)m_rotate.z = m_ModelWorldParam.rotate.z;
		}
		else if (m_bMoving)
		{
			float rotate = 90.0f;
			static int moveCnt = 0;
			moveCnt++;
			m_pStageLinie->Step(2.0f / MOVE_TIME);
			if (bRight)m_tBGRotateZ += 120.0f / MOVE_TIME;
			else m_tBGRotateZ -= 120.0f / MOVE_TIME;
			if (g_OldSelect_type.StageMainNumber != g_Select_type.StageMainNumber|| g_OldSelect_type.StageSubNumber != g_Select_type.StageSubNumber)
			{
				XAUDIO2_BUFFER buffer;
				m_pSourseStageSelectSE[GRASS_SE]->Stop();
				m_pSourseStageSelectSE[DESERT_SE]->Stop();
				m_pSourseStageSelectSE[SNOW_SE]->Stop();
				switch (g_OldSelect_type.StageMainNumber)
				{
				case GRASSLAND:
					m_pSourseStageSelectSE[GRASS_SE]->FlushSourceBuffers();
					buffer = m_pSESound[GRASS_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[GRASS_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[GRASS_SE]);
					m_pSourseStageSelectSE[GRASS_SE]->Start();
					break;
				case DESERT:
					m_pSourseStageSelectSE[DESERT_SE]->FlushSourceBuffers();
					buffer = m_pSESound[DESERT_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[DESERT_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[DESERT_SE]);
					m_pSourseStageSelectSE[DESERT_SE]->Start();
					break;
				case SNOWFIELD:
					m_pSourseStageSelectSE[SNOW_SE]->FlushSourceBuffers();
					buffer = m_pSESound[SNOW_SE]->GetBuffer(false);
					m_pSourseStageSelectSE[SNOW_SE]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSourseStageSelectSE[SNOW_SE]);
					m_pSourseStageSelectSE[SNOW_SE]->Start();
					break;
				default:
					break;
				} 
				g_OldSelect_type = g_Select_type;
			}
			switch (g_Select_type.StageMainNumber)
			{
			case(GRASSLAND):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(GRASS_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(GRASS_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				//if (m_bMoving)
				//{
				//	if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
				//	else LinieRotationY -= 90.0f / MOVE_TIME;
				//}
				if (bRight)m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(GRASS_ROTATE_Z) - DirectX::XMConvertToRadians(SNOW_ROTATE_Z)) / MOVE_TIME;
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(GRASS_ROTATE_Z2) - DirectX::XMConvertToRadians(DESERT_ROTATE_Z)) / MOVE_TIME;
				break;
			case(DESERT):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(DESERT_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(DESERT_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				/*if (m_bMoving == true)
				{
					if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
					else LinieRotationY -= 90.0f / MOVE_TIME;
				}*/
				if (bRight)m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(DESERT_ROTATE_Z) - DirectX::XMConvertToRadians(GRASS_ROTATE_Z2)) / MOVE_TIME;
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(DESERT_ROTATE_Z) - DirectX::XMConvertToRadians(SNOW_ROTATE_Z)) / MOVE_TIME;
				break;
			case(SNOWFIELD):
				//m_ModelParam[WorldField].rotate.x += (DirectX::XMConvertToRadians(SNOW_ROTATE_X) - m_rotate.x) / MOVE_TIME;
				//m_ModelParam[WorldField].rotate.y += (DirectX::XMConvertToRadians(SNOW_ROTATE_Y) - m_rotate.y) / MOVE_TIME;
				//if (m_bMoving) 
				//{
				//	if (moveCnt >= MOVE_TIME / 2.0f)LinieRotationY += 90.0f / MOVE_TIME;
				//	else LinieRotationY -= 90.0f / MOVE_TIME;
				//}
				if (bRight) {
					m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(SNOW_ROTATE_Z) - DirectX::XMConvertToRadians(DESERT_ROTATE_Z)) / MOVE_TIME;
				}
				else m_ModelWorldParam.rotate.z += (DirectX::XMConvertToRadians(SNOW_ROTATE_Z) - DirectX::XMConvertToRadians(GRASS_ROTATE_Z)) / MOVE_TIME;
				break;
			}

			for (int i = 0; i < 3; i++)
			{
				if (!MainStage)
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
				m_pStageLinie->SetAnimeTime(g_AnimNo,0.0f);
				m_bMoving = false;
				moveCnt = 0;
			}
		}
	}

	SelectAnimation();
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
	float ArrowSize2 = 200.0f * Scale2;
	float ArrowSize3 = 105.0f * Scale2;

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

	//m_pStageLinie->SetPostion(0.0f, -66.0f, 100.0f);
	//m_pStageLinie->SetRotation(m_ModelParam[WorldField].rotate.x, m_ModelParam[WorldField].rotate.y, m_ModelParam[WorldField].rotate.z);
	//m_pStageLinie->SetScale(100.0f, 100.0f, 0.0f);
	//m_pStageLinie->SetViewMatrix(GetView());
	//m_pStageLinie->SetProjectionMatrix(GetProj());
	//m_pStageLinie->Draw();

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
		SetRender3D();
		//シェーダーへ変換行列を設定
		DirectX::XMFLOAT4X4 wvp[3];
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_ModelWorldParam.pos.x, m_ModelWorldParam.pos.y, m_ModelWorldParam.pos.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(m_ModelWorldParam.rotate.x, m_ModelWorldParam.rotate.y, m_ModelWorldParam.rotate.z, 0.0f));
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_ModelWorldParam.size.x, m_ModelWorldParam.size.y, m_ModelWorldParam.size.z);
		DirectX::XMMATRIX mat = S * R * T;
		mat = DirectX::XMMatrixTranspose(mat);
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, mat);
		wvp[0] = world;
		wvp[1] = GetView();
		wvp[2] = GetProj();


		Geometory::SetView(wvp[1]);
		Geometory::SetProjection(wvp[2]);
		ShaderList::SetWVP(wvp);	//引数にはXMFLOAT4x4型の、要素数３の配列のアドレスを渡すこと

		//モデルに使用する頂点シェーダー、ピクセルシェーダーを設
		m_pWorldModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pWorldModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));


		//複数のメッシュで構成されている場合ある部分は金属的な表現、ある部分は非金属的な表現と
		//分ける場合がある前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
		//切り替える
		for (int i = 0; i < m_pWorldModel->GetMeshNum(); ++i)
		{
			//メッシュに割り当てられているマテリアルを取得
			Model::Material material = *m_pWorldModel->GetMaterial(m_pWorldModel->GetMesh(i)->materialID);
			material.ambient.x = 0.85f; // x (r) 
			material.ambient.y = 0.85f; // y (g) 
			material.ambient.z = 0.85f; // z (b) 
			//シェーダーへマテリアルを設定
			ShaderList::SetMaterial(material);

			//モデルの描画
			m_pWorldModel->Draw(i);
		}
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
		LinieDraw();

		SetRender2D;
		Sprite::ReSetSprite();
		m_pWorldSelect->SetProjection(Get2DProj());
		m_pWorldSelect->SetView(Get2DView());
		m_pWorldSelect->SetTexture();
		m_pWorldSelect->SetPositon(CENTER_POS_X - 660, CENTER_POS_Y - 470, 00.0f);
		m_pWorldSelect->SetSize(600.0f, -150.0f, 0.0f);
		m_pWorldSelect->Disp();
		switch (g_Select_type.StageMainNumber)
		{
		case(GRASSLAND):

			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			SetRender3D();
			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();

			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pWorld[0]->SetProjection(GetProj());
				m_pWorld[0]->SetView(GetView());
				m_pWorld[0]->SetTexture();
				m_pWorld[0]->SetPositon(0.0f, 171.0f, 145.0f);
				m_pWorld[0]->SetSize(180.0f, 40.0f, 100.0f);
				m_pWorld[0]->Disp();

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
			}
			break;
		case(DESERT):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pWorld[1]->SetProjection(GetProj());
				m_pWorld[1]->SetView(GetView());
				m_pWorld[1]->SetTexture();
				m_pWorld[1]->SetPositon(0.0f, 171.0f, 145.0f);
				m_pWorld[1]->SetSize(180.0f, 40.0f, 100.0f);
				m_pWorld[1]->Disp();

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
			}
			break;
		case(SNOWFIELD):
			SetRender3D();
			Sprite::ReSetSprite();
			m_pGrassLand->SetProjection(GetProj());
			m_pGrassLand->SetView(GetView());
			m_pGrassLand->SetTexture();
			m_pGrassLand->SetPositon(posX[0], 70.0f, 0.0f);
			m_pGrassLand->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pGrassLand->Disp();

			SetRender3D();
			Sprite::ReSetSprite();
			m_pDesert->SetProjection(GetProj());
			m_pDesert->SetView(GetView());
			m_pDesert->SetTexture();
			m_pDesert->SetPositon(posX[1], 70.0f, 0.0f);
			m_pDesert->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pDesert->Disp();

			Sprite::ReSetSprite();
			m_pSnowField->SetProjection(GetProj());
			m_pSnowField->SetView(GetView());
			m_pSnowField->SetTexture();
			m_pSnowField->SetPositon(posX[2], 70.0f, 0.0f);
			m_pSnowField->SetSize(WORLD_SIZE_X, WORLD_SIZE_Y, 100.0f);
			m_pSnowField->Disp();
			if (m_bMoving == false)
			{
				Sprite::ReSetSprite();
				m_pWorld[2]->SetProjection(GetProj());
				m_pWorld[2]->SetView(GetView());
				m_pWorld[2]->SetTexture();
				m_pWorld[2]->SetPositon(0.0f, 171.0f, 145.0f);
				m_pWorld[2]->SetSize(180.0f, 40.0f, 100.0f);
				m_pWorld[2]->Disp();
				
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

			}
			break;
		}

		if (m_bMoving == false)
		{
			SetRender2D();

			//m_pDecition->SetView(Get2DView());
			//m_pDecition->SetProjection(Get2DProj());
			//m_pDecition->SetPositon(CENTER_POS_X + m_tDecitionPos[0].x, CENTER_POS_Y + m_tDecitionPos[0].y, 0.0f);
			//m_pDecition->SetSize(100.0f, 100.0f, 100.0f);
			//m_pDecition->SetRotation(0.0f, 0.0f, 0.0f);
			//m_pDecition->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
			//m_pDecition->SetUvPos(2.0f / 4.0f, 3.0f / 10.0f);
			//m_pDecition->SetUvSize(1.0f / 4.0f, 1.0f / 10.0f);
			//m_pDecition->SetTexture();
			//m_pDecition->Disp();

			//m_pDecition->SetPositon(CENTER_POS_X + m_tDecitionPos[1].x, CENTER_POS_Y + m_tDecitionPos[1].y, 0.0f);
			//m_pDecition->Disp();

			Sprite::ReSetSprite();
			m_pStageSelected->SetProjection(GetProj());
			m_pStageSelected->SetView(GetView());
			m_pStageSelected->SetTexture();
			m_pStageSelected->SetPositon(posX[g_Select_type.StageMainNumber], 103.0f, 140.0f);
			m_pStageSelected->SetSize(ArrowSize2, ArrowSize3, 100.0f);
			m_pStageSelected->Disp();

		}
	}
	else
	{
		{
			SetRender2D;
			Sprite::ReSetSprite();
			m_pStageSelect->SetProjection(Get2DProj());
			m_pStageSelect->SetView(Get2DView());
			m_pStageSelect->SetTexture();
			m_pStageSelect->SetPositon(CENTER_POS_X - 660, CENTER_POS_Y - 470, 00.0f);
			m_pStageSelect->SetSize(600.0f, -150.0f, 0.0f);
			m_pStageSelect->Disp();
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

void CStageSelect::LinieDraw()
{
	SetRender3D();
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(0.0f, 25.0f, 0.0f, 0.0f));
	//拡大縮小行列(Scaling)
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.5f, 1.5f, 1.5f);
	//回転行列(Rotation)
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(LinieRotationY), DirectX::XMConvertToRadians(0.0f), 0.0f));
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	ShaderList::SetWVP(wvp);

	m_pStageLinie->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pStageLinie->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	for (int i = 0; i < m_pStageLinie->GetMeshNum(); ++i)
	{
		Model::Mesh tMesh = *m_pStageLinie->GetMesh(i);
		Model::Material material = *m_pStageLinie->GetMaterial(tMesh.materialID);
		material.ambient.x = 0.85f; // x (r) 
		material.ambient.y = 0.85f; // y (g) 
		material.ambient.z = 0.85f; // z (b) 
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る

		DirectX::XMFLOAT4X4 bones[200];
		for (int j = 0; j < tMesh.bones.size(); ++j)
		{
			DirectX::XMStoreFloat4x4(&bones[j], DirectX::XMMatrixTranspose(
				tMesh.bones[j].invOffset * m_pStageLinie->GetBoneMatrix(tMesh.bones[j].nodeIndex)
			));
		}
		ShaderList::SetBones(bones);


		if (m_pStageLinie) 
		{
			m_pStageLinie->Draw(i);
		}
	}
}


void CStageSelect::SelectAnimation()
{
	if(MainStage)m_pBackGround->MultiScrollRotate({ 0.0f,0.0f,DirectX::XMConvertToRadians(m_tBGRotateZ) });
}

void CStageSelect::ClearCheck()
{
	for (int i = 0; i < MAX_STAGE; i++)
	{
		m_bClear[i] = true;
		//m_bClear[i] = CSceneResult::GetStageClear(i);
	}
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
