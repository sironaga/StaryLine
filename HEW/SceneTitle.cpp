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

#define SELECT_MOVE (80.0f)
#define CENTER_POS_X SCREEN_WIDTH / 2.0f
#define CENTER_POS_Y SCREEN_HEIGHT / 2.0f

enum
{
	SCREEN_800,
	SCREEN_1280,
	SCREEN_1600,
	SCREEN_1920,
};

enum E_TITLE_TYPE
{
	GAMESTART = 0,
	GAMECONTINUE,
	GAMEOPTION,
	GAMEEND,
}g_Title_type;


CSceneTitle::CSceneTitle()
	: m_SelectPos{735.0f, 130.0f}, m_bSelected(false)
{
	g_Title_type = GAMESTART;
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//テクスチャの読み込み
	/*tex = new Texture();
	tex->Create("Assets/Texture/Title/Title_Logo.png");*/
	m_pTitleLogo		 = new SpriteEx("Assets/Texture/Title/Title_Logo.png");
	m_pTitleBack		 = new SpriteEx("Assets/Texture/Title/Title_BackBoard.png");
	m_pLini				 = new SpriteEx("Assets/Texture/Title/Title_Chara.png");
	m_pTitleFrame		 = new SpriteEx("Assets/Texture/Title/Title_Selected.png");
	m_pTitleUnderbar	 = new SpriteEx("Assets/Texture/Title/Title_Underbar.png");
	m_pTitleStart[0]	 = new SpriteEx("Assets/Texture/Title/Title_First.png");
	m_pTitleStart[1]	 = new SpriteEx("Assets/Texture/Title/Title_Start_Push.png");
	m_pTitleContinued[0] = new SpriteEx("Assets/Texture/Title/Title_Continued.png");
	m_pTitleContinued[1] = new SpriteEx("Assets/Texture/Title/Title_Continue_push.png");
	m_pTitleOption[0]	 = new SpriteEx("Assets/Texture/Title/Title_Option.png");
	m_pTitleOption[1]	 = new SpriteEx("Assets/Texture/Title/Title_Option_push.png");
	m_pTitleEnd[0]		 = new SpriteEx("Assets/Texture/Title/Title_Finish.png");
	m_pTitleEnd[1]		 = new SpriteEx("Assets/Texture/Title/Title_Finish_push.png");

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pTitleStart[nLoop]->SetProjection(Get2DProj());
		m_pTitleStart[nLoop]->SetView(Get2DView());
		m_pTitleContinued[nLoop]->SetProjection(Get2DProj());
		m_pTitleContinued[nLoop]->SetView(Get2DView());
		m_pTitleOption[nLoop]->SetProjection(Get2DProj());
		m_pTitleOption[nLoop]->SetView(Get2DView());
		m_pTitleEnd[nLoop]->SetProjection(Get2DProj());
		m_pTitleEnd[nLoop]->SetView(Get2DView());
	}
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	m_pLini->SetProjection(Get2DProj());
	m_pLini->SetView(Get2DView());
	m_pTitleBack->SetProjection(Get2DProj());
	m_pTitleBack->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());


	//m_pParam = new SpriteParam();
	//m_pParam->pos = { -160.0f,-170.0f };
	//m_pParam->size = { 100.0f,100.0f };
	//m_pParam->world = m_pParam->operator()();

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

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		if (m_pTitleStart[nLoop])
		{
			delete m_pTitleStart[nLoop];
			m_pTitleStart[nLoop] = nullptr;
		}
		if (m_pTitleContinued[nLoop])
		{
			delete m_pTitleContinued[nLoop];
			m_pTitleContinued[nLoop] = nullptr;
		}
		if (m_pTitleOption[nLoop])
		{
			delete m_pTitleOption[nLoop];
			m_pTitleOption[nLoop] = nullptr;
		}
		if (m_pTitleEnd[nLoop])
		{
			delete m_pTitleEnd[nLoop];
			m_pTitleEnd[nLoop] = nullptr;
		}
	}
	if (m_pTitleLogo)
	{
		delete m_pTitleLogo;
		m_pTitleLogo = nullptr;
	}
	if (m_pTitleFrame)
	{
		delete m_pTitleFrame;
		m_pTitleFrame = nullptr;
	}
	if (m_pTitleUnderbar)
	{
		delete m_pTitleUnderbar;
		m_pTitleLogo = nullptr;
	}
	SAFE_DELETE(m_pTitleBack);
	SAFE_DELETE(m_pLini);

}

void CSceneTitle::Update()
{
	g_pTitleBG->Update();
	g_TitleSound->SetMasterVolume();
	//音量を設定
	SetAllMasterVolume(m_pOption->GetMasterVoluem());
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());

	if (m_pOption->GetOption())
	{
		m_bSelected = false;
		m_pOption->Update();
	}

	if (!m_bSelected)
	{
		switch (g_Title_type)
		{
		case(GAMESTART):
			if (IsKeyTrigger(VK_DOWN))
			{
				g_Title_type = GAMECONTINUE;
				m_SelectPos.y += SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_RETURN))
			{
				m_bSelected = true;
				SetNext(STAGE_SELECT);
			}
			break;

		case(GAMECONTINUE):
			if (IsKeyTrigger(VK_DOWN))
			{
				g_Title_type = GAMEOPTION;
				m_SelectPos.y += SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_UP))
			{
				g_Title_type = GAMESTART;
				m_SelectPos.y -= SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_RETURN))
			{
				//コンティニューシーンへ切り替える処理
				m_bSelected = true;
			}
			break;

		case(GAMEOPTION):
			if (!m_pOption->GetOption())
			{
				if (IsKeyTrigger(VK_DOWN))
				{
					g_Title_type = GAMEEND;
					m_SelectPos.y += SELECT_MOVE;
				}
				if (IsKeyTrigger(VK_UP))
				{
					g_Title_type = GAMECONTINUE;
					m_SelectPos.y -= SELECT_MOVE;
				}
			}
			if (IsKeyTrigger(VK_RETURN))
			{
				m_bSelected = true;
				//オプションへ切り替える処理
				m_pOption->SetOption();

			}
			break;

		case(GAMEEND):
			if (IsKeyTrigger(VK_UP))
			{
				g_Title_type = GAMEOPTION;
				m_SelectPos.y -= SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_RETURN))
			{
				m_bSelected = true;
				SetGameEnd();
			}
			break;

		default:break;
		}
	}
	if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{

		SetNext(SCENE_DEBUGROOM);
	}
	static bool b = false;
	if (m_pOption->GetIsFullScreen()==0 && !b)
	{
		
		SetFullscreenSwap();
		b ^= true;
	}
	else if (m_pOption->GetIsFullScreen()==1  && b)
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
			SetNowResolusion(1920, 1080);
			SetResolusion(1920, 1080,b);
			InitResolusionMain();
			break;
		case SCREEN_1600:
			SetNowResolusion(1600, 900);
			SetResolusion(1600, 900,b);
			InitResolusionMain();
			break;
		case SCREEN_1280:
			SetNowResolusion(1280, 720);
			SetResolusion(1280, 720,b);
			InitResolusionMain();
			break;
		case SCREEN_800:
			SetNowResolusion(800, 600);
			SetResolusion(800, 600,b);
			InitResolusionMain();
			break;
		}
	}

	if (g_pSourseTitleBGM)SetVolumeBGM(g_pSourseTitleBGM);
}

void CSceneTitle::Draw()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pTitleStart[nLoop]->SetProjection(Get2DProj());
		m_pTitleStart[nLoop]->SetView(Get2DView());
		m_pTitleContinued[nLoop]->SetProjection(Get2DProj());
		m_pTitleContinued[nLoop]->SetView(Get2DView());
		m_pTitleOption[nLoop]->SetProjection(Get2DProj());
		m_pTitleOption[nLoop]->SetView(Get2DView());
		m_pTitleEnd[nLoop]->SetProjection(Get2DProj());
		m_pTitleEnd[nLoop]->SetView(Get2DView());
	}

	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());


	g_pTitleBG->Draw();

	m_pTitleBack->SetTexture();
	m_pTitleBack->SetProjection(Get2DProj());
	m_pTitleBack->SetView(Get2DView());
	m_pTitleBack->SetPositon(CENTER_POS_X, CENTER_POS_Y, 0.0f);
	m_pTitleBack->SetSize(1920.0f, -1080.0f, 0.0f);
	m_pTitleBack->Disp();

	m_pLini->SetTexture();
	m_pLini->SetProjection(Get2DProj());
	m_pLini->SetView(Get2DView());
	m_pLini->SetPositon(CENTER_POS_X, CENTER_POS_Y, 0.0f);
	m_pLini->SetSize(1920.0f, -1080.0f, 0.0f);
	m_pLini->Disp();

	m_pTitleLogo->SetTexture();
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	m_pTitleLogo->SetPositon(CENTER_POS_X + 500, CENTER_POS_Y - 250, 0.0f);
	m_pTitleLogo->SetSize(840.0f, -560.0f, 0.0f);	
	m_pTitleLogo->Disp();

	m_pTitleUnderbar->SetTexture();
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());
	m_pTitleUnderbar->SetPositon(CENTER_POS_X, CENTER_POS_Y + 510, 0.0f);
	m_pTitleUnderbar->SetSize(1920.0f, -60, 0.0f);
	m_pTitleUnderbar->Disp();

	m_pTitleStart[0]->SetTexture();
	m_pTitleStart[0]->SetProjection(Get2DProj());
	m_pTitleStart[0]->SetView(Get2DView());
	m_pTitleStart[0]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 130, 0.0f);
	m_pTitleStart[0]->SetSize(450.0f, -60.0f, 0.0f);
	m_pTitleStart[0]->Disp();

	m_pTitleContinued[0]->SetTexture();
	m_pTitleContinued[0]->SetProjection(Get2DProj());
	m_pTitleContinued[0]->SetView(Get2DView());
	m_pTitleContinued[0]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 210, 0.0f);
	m_pTitleContinued[0]->SetSize(450.0f, -60.0f, 0.0f);
	m_pTitleContinued[0]->Disp();

	m_pTitleOption[0]->SetTexture();
	m_pTitleOption[0]->SetProjection(Get2DProj());
	m_pTitleOption[0]->SetView(Get2DView());
	m_pTitleOption[0]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 290, 0.0f);
	m_pTitleOption[0]->SetSize(450.0f, -60.0f, 0.0f);
	m_pTitleOption[0]->Disp();

	m_pTitleEnd[0]->SetTexture();
	m_pTitleEnd[0]->SetProjection(Get2DProj());
	m_pTitleEnd[0]->SetView(Get2DView());
	m_pTitleEnd[0]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 370, 0.0f);
	m_pTitleEnd[0]->SetSize(450.0f, -60.0f, 0.0f);
	m_pTitleEnd[0]->Disp();
	
	if(m_bSelected)
	{
		switch (g_Title_type)
		{
		case GAMESTART:
			m_pTitleStart[1]->SetTexture();
			m_pTitleStart[1]->SetProjection(Get2DProj());
			m_pTitleStart[1]->SetView(Get2DView());
			m_pTitleStart[1]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 130, 0.0f);
			m_pTitleStart[1]->SetSize(450.0f, -60.0f, 0.0f);
			m_pTitleStart[1]->Disp();
			break;

		case GAMECONTINUE:
			m_pTitleContinued[1]->SetTexture();
			m_pTitleContinued[1]->SetProjection(Get2DProj());
			m_pTitleContinued[1]->SetView(Get2DView());
			m_pTitleContinued[1]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 210, 0.0f);
			m_pTitleContinued[1]->SetSize(450.0f, -60.0f, 0.0f);
			m_pTitleContinued[1]->Disp();
			break;

		case GAMEOPTION:
			m_pTitleOption[1]->SetTexture();
			m_pTitleOption[1]->SetProjection(Get2DProj());
			m_pTitleOption[1]->SetView(Get2DView());
			m_pTitleOption[1]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 290, 0.0f);
			m_pTitleOption[1]->SetSize(450.0f, -60.0f, 0.0f);
			m_pTitleOption[1]->Disp();
			break;

		case GAMEEND:
			m_pTitleEnd[1]->SetTexture();
			m_pTitleEnd[1]->SetProjection(Get2DProj());
			m_pTitleEnd[1]->SetView(Get2DView());
			m_pTitleEnd[1]->SetPositon(CENTER_POS_X + 735, CENTER_POS_Y + 370, 0.0f);
			m_pTitleEnd[1]->SetSize(450.0f, -60.0f, 0.0f);
			m_pTitleEnd[1]->Disp();
			break;

		default:
			break;
		}
	}

	m_pTitleFrame->SetTexture();
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleFrame->SetPositon(CENTER_POS_X + m_SelectPos.x, CENTER_POS_Y + m_SelectPos.y, 0.0f);
	m_pTitleFrame->SetSize(470.0f, -80, 0.0f);
	m_pTitleFrame->Disp();

	//Sprite::SetSize(m_pParam->size);
	//Sprite::SetOffset(m_pParam->pos );
	//Sprite::SetColor({m_pParam->color});
	//Sprite::SetUVPos(m_pParam->uvPos);
	//Sprite::SetUVScale({ m_pParam ->uvSize});
	//Sprite::SetWorld(m_pParam->world);
	//Sprite::SetView(m_pParam->view);
	//Sprite::SetProjection(m_pParam->proj);
	//Sprite::SetTexture(m_pSelect);
	//Sprite::Draw();
	//Sprite::ReSetSprite();

	if (m_pOption->GetOption())m_pOption->Draw();
}
void CSceneTitle::SetResolusion(float wide, float height,bool fullscreen)
{
	UninitDirectX();
	InitDirectX(m_phWnd, wide, height, fullscreen);

	Geometory::Uninit();
	Geometory::Init();
	Sprite::Init();
	LibEffekseer::Uninit();
	LibEffekseer::Init(GetDevice(), GetContext());
	InitInput();
	ShaderList::Uninit();
	ShaderList::Init();
	UninitSpriteDrawer();
	InitSpriteDrawer(GetDevice(), GetContext(), wide, height);

	SAFE_DELETE(m_pTitleLogo);
	m_pTitleLogo = new SpriteEx("Assets/Texture/Title/Title_Logo.png");
	SAFE_DELETE(m_pTitleFrame);
	m_pTitleFrame = new SpriteEx("Assets/Texture/Title/Title_Selected.png");
	SAFE_DELETE(m_pTitleUnderbar);
	m_pTitleUnderbar = new SpriteEx("Assets/Texture/Title/Title_Underbar.png");
	SAFE_DELETE(m_pTitleStart[0]);
	m_pTitleStart[0] = new SpriteEx("Assets/Texture/Title/Title_First.png");
	SAFE_DELETE(m_pTitleStart[1]);
	m_pTitleStart[1] = new SpriteEx("Assets/Texture/Title/Title_Start_Push.png");
	SAFE_DELETE(m_pTitleContinued[0]);
	m_pTitleContinued[0] = new SpriteEx("Assets/Texture/Title/Title_Continued.png");
	SAFE_DELETE(m_pTitleContinued[1]);
	m_pTitleContinued[1] = new SpriteEx("Assets/Texture/Title/Title_Continue_push.png");
	SAFE_DELETE(m_pTitleOption[0]);
	m_pTitleOption[0] = new SpriteEx("Assets/Texture/Title/Title_Option.png");
	SAFE_DELETE(m_pTitleOption[1]);
	m_pTitleOption[1] = new SpriteEx("Assets/Texture/Title/Title_Option_push.png");
	SAFE_DELETE(m_pTitleEnd[0]);
	m_pTitleEnd[0] = new SpriteEx("Assets/Texture/Title/Title_Finish.png");
	SAFE_DELETE(m_pTitleEnd[1]);
	m_pTitleEnd[1] = new SpriteEx("Assets/Texture/Title/Title_Finish_push.png");
	SAFE_DELETE(m_pTitleBack);
	m_pTitleBack = new SpriteEx("Assets/Texture/Title/Title_BackBoard.png");
	SAFE_DELETE(m_pLini);
	m_pLini = new SpriteEx("Assets/Texture/Title/Title_Chara.png");

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pTitleStart[nLoop]->SetProjection(Get2DProj());
		m_pTitleStart[nLoop]->SetView(Get2DView());
		m_pTitleContinued[nLoop]->SetProjection(Get2DProj());
		m_pTitleContinued[nLoop]->SetView(Get2DView());
		m_pTitleOption[nLoop]->SetProjection(Get2DProj());
		m_pTitleOption[nLoop]->SetView(Get2DView());
		m_pTitleEnd[nLoop]->SetProjection(Get2DProj());
		m_pTitleEnd[nLoop]->SetView(Get2DView());
	}
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	m_pTitleBack->SetProjection(Get2DProj());
	m_pTitleBack->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());


	m_pOption->InitResolusion();


	if(g_pTitleBG)delete g_pTitleBG;
	g_pTitleBG = new CBackGround();

	if (m_pFade)delete m_pFade;
	m_pFade = new CFadeBlack();
	m_pFade->SetFade(0.5f, true);
}


