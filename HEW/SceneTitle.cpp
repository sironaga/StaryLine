#include "SceneTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"
#include "DirectXTex/TextureLoad.h"
#include "Defines.h"
#include "Sprite.h"
#include "ShaderList.h"
#include "SpriteDrawer.h"
#include "LibEffekseer.h"
#include "Geometory.h"
#include "FadeBlack.h"

#define SELECT_MOVE (90.0f)

enum E_TITLE_TYPE
{
	GAMESTART = 0,
	GAMECONTINUE,
	GAMEOPTION,
	GAMEEND,
}g_Title_type;


CSceneTitle::CSceneTitle()
	:f_SelectY(350.0f), m_pSelect(nullptr)
{
	g_Title_type = GAMESTART;
	m_pSelect = new Texture();
	if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	m_pParam = new SpriteParam();
	m_pParam->pos = { -160.0f,-170.0f };
	m_pParam->size = { 100.0f,100.0f };
	m_pParam->world = m_pParam->operator()();

	m_pOption = new COption();
	g_pTitleBG = new CBackGround();

	g_TitleSound = new CSoundList(BGM_TITLE);
	g_pSourseTitleBGM = g_TitleSound->GetSound(true);
	g_pSourseTitleBGM->Start();
	
}

CSceneTitle::~CSceneTitle()
{
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

	SAFE_DELETE(m_pSelect);
	SAFE_DELETE(m_pParam);
}

void CSceneTitle::Update()
{
	g_pTitleBG->Update();

	//音量を設定
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());

	if (m_pOption->GetOption())m_pOption->Update();

	switch (g_Title_type)
	{
	case(GAMESTART):
		if (IsKeyTrigger(VK_DOWN))
		{
			g_Title_type = GAMECONTINUE;
			m_pParam->pos.y -= SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			SetNext(STAGE_SELECT);
		}
		break;

	case(GAMECONTINUE):
		if (IsKeyTrigger(VK_DOWN))
		{
			g_Title_type = GAMEOPTION;
			m_pParam->pos.y -= SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_UP))
		{
			g_Title_type = GAMESTART;
			m_pParam->pos.y += SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			//コンティニューシーンへ切り替える処理

		}
		break;

	case(GAMEOPTION):
		if (!m_pOption->GetOption())
		{
			if (IsKeyTrigger(VK_DOWN))
			{
				g_Title_type = GAMEEND;
				m_pParam->pos.y -= SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_UP))
			{
				g_Title_type = GAMECONTINUE;
				m_pParam->pos.y += SELECT_MOVE;
			}
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			//オプションへ切り替える処理
			m_pOption->SetOption();

		}
		break;

	case(GAMEEND):
		if (IsKeyTrigger(VK_UP))
		{
			g_Title_type = GAMEOPTION;
			m_pParam->pos.y += SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			SetGameEnd();
		}
		break;

	default:break;
	}
	//if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	//{

	//	SetNext(SCENE_DEBUGROOM);
	//}
	static bool b = false;
	if (!m_pOption->GetIsFullScreen() && !b)
	{
		
		SetFullscreenSwap();
		b ^= true;
	}
	else if (m_pOption->GetIsFullScreen()  && b)
	{
		SetFullscreenSwap();
		b ^= true;
	}
	static int Resolusion=1;
	static int OldResolusion=1;
	OldResolusion = Resolusion;
	Resolusion  = m_pOption->GetResolusion();
	if (Resolusion != OldResolusion)
	{
		switch (Resolusion)
		{
		case SCREEN_1920:
			SetResolusion(1920, 1080);
			InitResolusionMain();
			break;
		case SCREEN_1600:
			SetResolusion(1600, 900);
			InitResolusionMain();
			break;
		case SCREEN_1280:
			SetResolusion(1280, 720);
			InitResolusionMain();
			break;
		case SCREEN_800:
			SetResolusion(800, 600);
			InitResolusionMain();
			break;
		}
	}

	if (g_pSourseTitleBGM)SetVolumeBGM(g_pSourseTitleBGM);
}

void CSceneTitle::Draw()
{
	g_pTitleBG->Draw();

	Sprite::SetSize(m_pParam->size);
	Sprite::SetOffset(m_pParam->pos );
	Sprite::SetColor({m_pParam->color});
	Sprite::SetUVPos(m_pParam->uvPos);
	Sprite::SetUVScale({ m_pParam ->uvSize});
	Sprite::SetWorld(m_pParam->world);
	Sprite::SetView(m_pParam->view);
	Sprite::SetProjection(m_pParam->proj);
	Sprite::SetTexture(m_pSelect);
	Sprite::Draw();
	Sprite::ReSetSprite();

	if (m_pOption->GetOption())m_pOption->Draw();
}
void CSceneTitle::SetResolusion(float wide, float height)
{
	UninitDirectX();
	InitDirectX(m_phWnd, wide, height, false);

	Geometory::Init();
	Sprite::Init();
	LibEffekseer::Init(GetDevice(), GetContext());
	InitInput();
	ShaderList::Init();
	InitSpriteDrawer(GetDevice(), GetContext(), wide, height);

	if(m_pSelect)delete m_pSelect;
	m_pSelect = new Texture();
	if (FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL, "Select.png", "Error", MB_OK);

	m_pOption->InitResolusion();

	if(g_pTitleBG)delete g_pTitleBG;
	g_pTitleBG = new CBackGround();

	if (m_pFade)delete m_pFade;
	m_pFade = new CFadeBlack();
	m_pFade->SetFade(0.5f, true);
}


