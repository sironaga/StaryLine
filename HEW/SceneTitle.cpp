#include "SceneTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "InputEX.h"
#include "DirectXTex/TextureLoad.h"
#include "Defines.h"
#include "Sprite.h"
#include "ShaderList.h"
#include "SpriteDrawer.h"
#include "LibEffekseer.h"
#include "Geometory.h"
#include "FadeBlack.h"
#include "Transition.h"
#include "File.h"
#define SELECT_MOVE (80.0f)
#define CENTER_POS_X SCREEN_WIDTH / 2.0f
#define CENTER_POS_Y SCREEN_HEIGHT / 2.0f
#define STAR_SPEED 2.0f
#define DECISION_SPEED 1.0f
#define SELECT_POW 1.2f
#define STAR_AJUSTPOS_X SCREEN_WIDTH / 2.0f
#define SELECT_JUST_Y 30.0f

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

enum E_TITLE_ANIME
{
	AnimeStart = 0,
	StartToChara = 30,
	CharaToLogo = 90,
	LogoToBar = 120,

}g_eTitleAnim;

ID3D11SamplerState* g_pTitleSPSampler;

CSceneTitle::CSceneTitle(COption* pOption)
	: m_SelectPos{550.0f, 135.0f}, m_bSelected(false)
	, m_DecisionPos{ SCREEN_WIDTH / 2.0f, 100.0f }
	, m_nAnimCount(0), m_tCharaLogoTexPos{}
	, m_nLiniYCount(0), m_tLiniPos{CENTER_POS_X-210.0f ,CENTER_POS_Y + 75.0f}
	, m_pEffect()
	,m_bChange(false)
	, m_tTabPos{}, m_tTabSize{}
	, m_fSelectScale(1.0f), m_tStarPos{ {0.0f,SCREEN_HEIGHT / 2.0f} }
	, m_pStarEfc{}, m_tStarRotate{}
	, m_Direction(XINPUT_GAMEPAD_START), m_bRankingCommand{},m_fRankingCommandResetTimer(0.0f)
{
	g_Title_type = GAMESTART;
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//テクスチャの読み込み
	/*tex = new Texture();
	tex->Create("Assets/Texture/Title/Title_Logo.png");*/
	m_pTitleLogo		 = new SpriteEx("Assets/Texture/Title/Title_Logo.png");
	//m_pTitleBack		 = new SpriteEx("Assets/Texture/Title/Title_BackBoard.png");
	m_pLini[0]			 = new SpriteEx("Assets/Texture/Title/Title_Chara_Back.png");
	m_pLini[1]			 = new SpriteEx("Assets/Texture/Title/Title_Chara.png");
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
	m_pDecition = new SpriteEx("Assets/Effect/Sprite/Decision.png");
	m_pEffect[(int)Effect::Star] = new CEffectManager_sp("Assets/Effect/Sprite/BackGround_ShootginStars_2.png", 8, 8, STAR_SPEED);
	m_pEffect[(int)Effect::Choice] = new CEffectManager_sp("Assets/Effect/Sprite/Choice.png", 4, 11, 0.5f);

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
		m_pLini[nLoop]->SetProjection(Get2DProj());
		m_pLini[nLoop]->SetView(Get2DView());
	}
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	//m_pTitleBack->SetProjection(Get2DProj());
	//m_pTitleBack->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());


	g_eTitleAnim = AnimeStart;
	//m_pParam = new SpriteParam();
	//m_pParam->pos = { -160.0f,-170.0f };
	//m_pParam->size = { 100.0f,100.0f };
	//m_pParam->world = m_pParam->operator()();

	m_pOption = pOption;
	g_pTitleBG = new CBackGround();

	g_TitleSound = new CSoundList(BGM_TITLE);
	g_pSourseTitleBGM = g_TitleSound->GetSound(true);
	g_pSourseTitleBGM->Start();
	
	for (int i = 0; i < 4; i++)
	{
		m_tTabPos[i] = { CENTER_POS_X + 550.0f, CENTER_POS_Y + 130.0f + i * SELECT_MOVE };
		m_tTabSize[i] = { 540.0f,-60.0f };
	}
	m_pSelectsound = new CSoundList(SE_DECISION);
	m_pSourseSelectSE = m_pSelectsound->GetSound(false);

	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i] = new CEffectManager_sp(m_pEffect[(int)Effect::Star]);
	}

	// サンプラー
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HRESULT hr = GetDevice()->CreateSamplerState(&samplerDesc, &g_pTitleSPSampler);
	if (FAILED(hr)) { return; }
	GetContext()->PSSetSamplers(0, 1, &g_pTitleSPSampler);
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
	//SAFE_DELETE(m_pTitleBack);
	for (int i = 0; i < 2; i++)
	{

		SAFE_DELETE(m_pLini[i]);
	}
	if (m_pOption)
	{
		m_pOption = nullptr;
	}
	for (int i = 0; i < (int)Effect::Max; i++)
	{
		SAFE_DELETE(m_pEffect[i]);
	}
	if (m_pSelectsound)
	{
		m_pSourseSelectSE->Stop();
		SAFE_DELETE(m_pSelectsound);
		m_pSourseSelectSE = nullptr;
	}
	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i] = nullptr;
	}
}

void CSceneTitle::Update()
{
	m_Direction = GetControllerLStickTriggerForeDirection();
	g_pTitleBG->Update();
	g_TitleSound->SetMasterVolume();

	//音量を設定
	SetAllMasterVolume(m_pOption->GetMasterVoluem());
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());

	TitleAnimation();

	if (m_pOption->GetOption())
	{
		m_bSelected = false;
		m_pOption->ResetPos();
		m_pOption->ResetSize();
		m_pOption->Update();
	}

	if (!m_bSelected && !m_pOption->GetOption())
	{
		if (CheckRankingCommand())SetNext(SCENE_RANKING);
		switch (g_Title_type)
		{
		case(GAMESTART):
			if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)
			{
				g_Title_type = GAMECONTINUE;
				m_SelectPos.y += SELECT_MOVE;
				m_DecisionPos.y += SELECT_MOVE;
			}
			else if (IsKeyTrigger(VK_RETURN)|| IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
			{
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
				m_pSelectsound->SetMasterVolume();
				if (m_pSourseSelectSE)SetVolumeSE(m_pSourseSelectSE);
				m_pSourseSelectSE->Start();
				m_bSelected = true;

			}
			break;

		case(GAMECONTINUE):

			if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)
			{
				g_Title_type = GAMEOPTION;
				m_SelectPos.y += SELECT_MOVE;
				m_DecisionPos.y += SELECT_MOVE;
			}
			else if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)
			{
				g_Title_type = GAMESTART;
				m_SelectPos.y -= SELECT_MOVE;
				m_DecisionPos.y -= SELECT_MOVE;
			}
			else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
			{
				//コンティニューシーンへ切り替える処理
				if (!OutStageData())return;
				SetNext(STAGE_SELECT);
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
				m_bSelected = true;
			}
			break;

		case(GAMEOPTION):
			if (!m_pOption->GetOption())
			{
				if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN) || IsKeyTrigger('S') || m_Direction == XINPUT_GAMEPAD_DPAD_DOWN)
				{
					g_Title_type = GAMEEND;
					m_SelectPos.y += SELECT_MOVE;
					m_DecisionPos.y += SELECT_MOVE;
				}
				else if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)
				{
					g_Title_type = GAMECONTINUE;
					m_SelectPos.y -= SELECT_MOVE;
					m_DecisionPos.y -= SELECT_MOVE;
				}
				else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
				{
					m_bSelected = true;
					//オプションへ切り替える処理
					if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
					m_pOption->SetOption();

				}
			}
			break;

		case(GAMEEND):
			if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W') || m_Direction == XINPUT_GAMEPAD_DPAD_UP)
			{
				g_Title_type = GAMEOPTION;
				m_SelectPos.y -= SELECT_MOVE;
				m_DecisionPos.y -= SELECT_MOVE;
			}
			else if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_A))
			{
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
				m_bSelected = true;
			}
			break;

		default:break;
		}
	}
	else
	{
		if (!m_bChange)
		{
			switch (g_Title_type)
			{
			case GAMESTART:
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())
				{
					SetNext(STAGE_SELECT);
					m_bChange = true;
				}
				break;
			case GAMECONTINUE:
				m_bSelected = false;
				break;
			case GAMEOPTION:
				break;
			case GAMEEND:
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())
				{
					SetGameEnd();
					m_bChange = true;
				}
				break;
			default:
				break;
			}

		}
	}
	/*if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{

		SetNext(SCENE_DEBUGROOM);
	}*/
	OptionApply();
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
	
	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i]->SetPos({ m_tStarPos[i].x + CENTER_POS_X,0.0f,0.0f});
		m_pStarEfc[i]->SetSize({ 1000.0f,1000.0f,100.0f });
		m_pStarEfc[i]->SetRotate(m_tStarRotate[i]);
		m_pStarEfc[i]->Draw(false);
	}


	m_pLini[0]->SetTexture();
	m_pLini[0]->SetProjection(Get2DProj());
	m_pLini[0]->SetView(Get2DView());
	m_pLini[0]->SetPositon(CENTER_POS_X - 210.0, CENTER_POS_Y, 0.0f);
	m_pLini[0]->SetSize(1920.0f, -1080.0f, 0.0f);
	m_pLini[0]->SetUvSize(1.0f,1.0f);
	m_pLini[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLini[0]->Disp();
	
	m_pLini[1]->SetTexture();
	m_pLini[1]->SetProjection(Get2DProj());
	m_pLini[1]->SetView(Get2DView());
	m_pLini[1]->SetPositon(m_tLiniPos.x, m_tLiniPos.y, 0.0f);
	m_pLini[1]->SetSize(1500.0f, -1080.0f, 0.0f);
	m_pLini[1]->SetUvSize(1.0f,1.0f);
	m_pLini[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLini[1]->Disp();

	/*m_pTitleBack->SetTexture();
	m_pTitleBack->SetProjection(Get2DProj());
	m_pTitleBack->SetView(Get2DView());
	m_pTitleBack->SetPositon(CENTER_POS_X, CENTER_POS_Y, 0.0f);
	m_pTitleBack->SetSize(1920.0f, -1080.0f, 0.0f);
	m_pTitleBack->SetUvSize(m_tCharaLogoTexPos[0].x, m_tCharaLogoTexPos[0].y);
	m_pTitleBack->Disp();*/

	m_pTitleLogo->SetTexture();
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	m_pTitleLogo->SetPositon(CENTER_POS_X + 500, CENTER_POS_Y - 250, 0.0f);
	m_pTitleLogo->SetSize(840.0f, -560.0f, 0.0f);
	m_pTitleLogo->SetUvSize(1.0f, 1.0f);
	m_pTitleLogo->Disp();

	m_pTitleStart[0]->SetTexture();
	m_pTitleStart[0]->SetProjection(Get2DProj());
	m_pTitleStart[0]->SetView(Get2DView());
	m_pTitleStart[0]->SetPositon(m_tTabPos[0].x, m_tTabPos[0].y, 0.0f);
	m_pTitleStart[0]->SetSize(m_tTabSize[0].x, m_tTabSize[0].y, 0.0f);
	m_pTitleStart[0]->Setcolor(1.0f, 1.0f, 1.0f,1.0f);
	m_pTitleStart[0]->Disp();

	m_pTitleContinued[0]->SetTexture();
	m_pTitleContinued[0]->SetProjection(Get2DProj());
	m_pTitleContinued[0]->SetView(Get2DView());
	m_pTitleContinued[0]->SetPositon(m_tTabPos[1].x, m_tTabPos[1].y, 0.0f);
	m_pTitleContinued[0]->SetSize(m_tTabSize[1].x, m_tTabSize[1].y, 0.0f);
	m_pTitleContinued[0]->Setcolor(1.0f, 1.0f, 1.0f,1.0f);
	m_pTitleContinued[0]->Disp();

	m_pTitleOption[0]->SetTexture();
	m_pTitleOption[0]->SetProjection(Get2DProj());
	m_pTitleOption[0]->SetView(Get2DView());
	m_pTitleOption[0]->SetPositon(m_tTabPos[2].x, m_tTabPos[2].y, 0.0f);
	m_pTitleOption[0]->SetSize(m_tTabSize[2].x, m_tTabSize[2].y, 0.0f);
	m_pTitleOption[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pTitleOption[0]->Disp();

	m_pTitleEnd[0]->SetTexture();
	m_pTitleEnd[0]->SetProjection(Get2DProj());
	m_pTitleEnd[0]->SetView(Get2DView());
	m_pTitleEnd[0]->SetPositon(m_tTabPos[3].x, m_tTabPos[3].y, 0.0f);
	m_pTitleEnd[0]->SetSize(m_tTabSize[3].x, m_tTabSize[3].y , 0.0f);
	m_pTitleEnd[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pTitleEnd[0]->Disp();


	if (m_bSelected)
	{
		switch (g_Title_type)
		{
		case GAMESTART:
			m_pTitleStart[1]->SetTexture();
			m_pTitleStart[1]->SetProjection(Get2DProj());
			m_pTitleStart[1]->SetView(Get2DView());
			m_pTitleStart[1]->SetPositon(m_tTabPos[0].x, m_tTabPos[0].y , 0.0f);
			m_pTitleStart[1]->SetSize(m_tTabSize[0].x, m_tTabSize[0].y, 0.0f);
			m_pTitleStart[1]->Disp();
			break;

		case GAMECONTINUE:
			m_pTitleContinued[1]->SetTexture();
			m_pTitleContinued[1]->SetProjection(Get2DProj());
			m_pTitleContinued[1]->SetView(Get2DView());
			m_pTitleContinued[1]->SetPositon(m_tTabPos[1].x, m_tTabPos[1].y,0.0f);
			m_pTitleContinued[1]->SetSize(m_tTabSize[1].x, m_tTabSize[1].y, 0.0f);
			m_pTitleContinued[1]->Disp();
			break;

		case GAMEOPTION:
			m_pTitleOption[1]->SetTexture();
			m_pTitleOption[1]->SetProjection(Get2DProj());
			m_pTitleOption[1]->SetView(Get2DView());
			m_pTitleOption[1]->SetPositon(m_tTabPos[2].x, m_tTabPos[2].y, 0.0f);
			m_pTitleOption[1]->SetSize(m_tTabSize[2].x, m_tTabSize[2].y, 0.0f);
			m_pTitleOption[1]->Disp();
			break;

		case GAMEEND:
			m_pTitleEnd[1]->SetTexture();
			m_pTitleEnd[1]->SetProjection(Get2DProj());
			m_pTitleEnd[1]->SetView(Get2DView());
			m_pTitleEnd[1]->SetPositon(m_tTabPos[3].x, m_tTabPos[3].y, 0.0f);
			m_pTitleEnd[1]->SetSize(m_tTabSize[3].x, m_tTabSize[3].y, 0.0f);
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
	m_pTitleFrame->SetSize(m_tTabSize[g_Title_type].x + 25.0f * m_fSelectScale, m_tTabSize[g_Title_type].y - 25.0f * m_fSelectScale, 0.0f);
	m_pTitleFrame->Setcolor(1.0f,1.0f,1.0f, 1.0f);
	m_pTitleFrame->Disp();
	
	//m_pDecition->SetTexture();
	//m_pDecition->SetPositon(CENTER_POS_X + m_DecisionPos.x,CENTER_POS_Y + m_DecisionPos.y,0.0f);
	//m_pDecition->SetSize(100.0f,100.0f,100.0f );
	//m_pDecition->SetRotation( 0.0f,0.0f,0.0f );
	//m_pDecition->SetUvPos(2.0f / 4.0f, 3.0f / 10.0f);
	//m_pDecition->SetUvSize(1.0f / 4.0f, 1.0f / 10.0f);
	//m_pDecition->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_pDecition->SetProjection(Get2DProj());
	//m_pDecition->SetView(Get2DView());
	//m_pDecition->Disp();

	m_pTitleUnderbar->SetTexture();
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());
	m_pTitleUnderbar->SetPositon(CENTER_POS_X, CENTER_POS_Y + 510, 0.0f);
	m_pTitleUnderbar->SetSize(1920.0f, -60, 0.0f);
	m_pTitleUnderbar->Disp();

	m_pEffect[(int)Effect::Choice]->SetPos({ m_SelectPos.x - 255,-m_SelectPos.y,0.0f });
	m_pEffect[(int)Effect::Choice]->SetSize({ 100.0f,100.0f,100.0f });
	m_pEffect[(int)Effect::Choice]->SetRotate({ 0.0f,0.0f,0.0f });
	m_pEffect[(int)Effect::Choice]->Draw(false);


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
	//SAFE_DELETE(m_pTitleBack);
	//m_pTitleBack = new SpriteEx("Assets/Texture/Title/Title_BackBoard.png");
	SAFE_DELETE(m_pLini[0]);
	m_pLini[0] = new SpriteEx("Assets/Texture/Title/Title_Chara_Back.png");
	SAFE_DELETE(m_pLini[1]);
	m_pLini[1] = new SpriteEx("Assets/Texture/Title/Title_Chara.png");
	SAFE_DELETE(m_pEffect[(int)Effect::Star]);
	m_pEffect[(int)Effect::Star] = new CEffectManager_sp("Assets/Effect/Sprite/BackGround_ShootginStars_2.png", 8, 8, STAR_SPEED);
	SAFE_DELETE(m_pDecition);
	m_pDecition = new SpriteEx("Assets/Effect/Sprite/Decision.png");
	SAFE_DELETE(m_pEffect[(int)Effect::Choice]);
	m_pEffect[(int)Effect::Choice] = new CEffectManager_sp("Assets/Effect/Sprite/Choice.png", 4, 11, 0.5f);
	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i] = new CEffectManager_sp(m_pEffect[(int)Effect::Star]);
	}

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
		m_pLini[nLoop]->SetProjection(Get2DProj());
		m_pLini[nLoop]->SetView(Get2DView());
	}
	m_pTitleLogo->SetProjection(Get2DProj());
	m_pTitleLogo->SetView(Get2DView());
	//m_pTitleBack->SetProjection(Get2DProj());
	//m_pTitleBack->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());
	m_pTitleUnderbar->SetProjection(Get2DProj());
	m_pTitleUnderbar->SetView(Get2DView());


	m_pOption->InitResolusion();
	//m_pOption->SetAddPos(1000.0f, 0.0f);


	if(g_pTitleBG)delete g_pTitleBG;
	g_pTitleBG = new CBackGround();

	if (m_pFade)delete m_pFade;
	m_pFade = new CTransition();
	m_pFade->SetFade(1.0f, true);

	SAFE_DELETE(m_pSelectsound);
	m_pSelectsound = new CSoundList(SE_DECISION);
	m_pSelectsound->SetMasterVolume();
	m_pSourseSelectSE = m_pSelectsound->GetSound(false);

}

void CSceneTitle::TitleAnimation()
{
	for (int i = 0; i < (int)Effect::Max; i++)
	{
		m_pEffect[i]->Update();
	}
	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i]->Update();
	}

	m_nLiniYCount += 2;
	float rad = DirectX::XMConvertToRadians(m_nLiniYCount);
	float cosMove = cosf(rad) - 0.5f;
	float Y = CENTER_POS_Y + 75.0f;

	//switch (g_eTitleAnim)
	//{
	//case AnimeStart:

	//	if (m_nAnimCount >= StartToChara)
	//	{
	//		g_eTitleAnim = StartToChara;
	//	}
	//	break;
	//case StartToChara:

	//	for (int i = 0; i < 2; i++)
	//	{
	//		m_tCharaLogoTexPos[i] =
	//		{ (m_nAnimCount - StartToChara) * (1.0f / (float)(CharaToLogo - StartToChara)),
	//			(m_nAnimCount - StartToChara) * (1.0f / (float)(CharaToLogo - StartToChara)) };
	//	}
	//	if (m_nAnimCount >= CharaToLogo)
	//	{
	//		g_eTitleAnim = CharaToLogo;
	//	}
	//	break;
	//case CharaToLogo:
	//	if (m_nAnimCount >= LogoToBar)
	//	{
	//		g_eTitleAnim = LogoToBar;
	//	}
	//	break;
	//case LogoToBar:
	//	break;
	//default:
	//	break;
	//}
	m_tLiniPos.y = Y + cosMove * 50.0f;

	static int SelectProcess = 0;
	static float TotalMove = 485.0f * 2.0f + 60.0f;
	switch (SelectProcess)
	{
	case 0:
		if (m_DecisionPos.x <= SCREEN_WIDTH / 2.0f - 485.0f)
		{
			SelectProcess++;
		}
		else m_DecisionPos.x -= TotalMove / (DECISION_SPEED * 60.0f);
		break;
	case 1:
		if (m_DecisionPos.y >= 160.0f + SELECT_MOVE * g_Title_type)
		{
			SelectProcess++;
		}
		else m_DecisionPos.y += TotalMove / (DECISION_SPEED * 60.0f);
		break;
	case 2:
		if (m_DecisionPos.x >= SCREEN_WIDTH / 2.0f)
		{
			SelectProcess = 0;
			m_DecisionPos = { SCREEN_WIDTH / 2.0f, 100.0f + SELECT_MOVE * g_Title_type };
		}
		else m_DecisionPos.x += TotalMove / (DECISION_SPEED * 60.0f);
		break;
	default:
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		if (g_Title_type == i)
		{
			m_tTabPos[i] = { CENTER_POS_X + 550.0f, CENTER_POS_Y + 135.0f + i * SELECT_MOVE };
			m_tTabSize[i] = { 500.0f * SELECT_POW,-75.0f * SELECT_POW };
		}
		else
		{
			if(i < g_Title_type)m_tTabPos[i].y = CENTER_POS_Y + 135.0f + i * SELECT_MOVE - SELECT_JUST_Y;
			else m_tTabPos[i].y = CENTER_POS_Y + 130.0f + i * SELECT_MOVE + SELECT_JUST_Y;
			m_tTabSize[i] = { 500.0f,-75.0f };
		}
	}
	static float deg = 0.0f;    //度数
	deg += 4.0f;    // 度数を増やし続ける
	rad = DirectX::XMConvertToRadians(deg);    //ラジアンに変換する
	float cosAngle = cosf(rad);
	cosAngle /= 5.0f;
	m_fSelectScale = cosAngle + 1.0f;

	if (m_nAnimCount % 240 == 0)
	{
		m_tStarPos[0].x = 0.0f - (rand() % 20 * 100.0f);
		if (!m_pStarEfc[0]->IsPlay())m_pStarEfc[0]->Play(false);
	}
	if (m_nAnimCount % 300 == 0)
	{
		m_tStarPos[1].x = 0.0f - (rand() % 20 * 100.0f);
		if (!m_pStarEfc[1]->IsPlay())m_pStarEfc[1]->Play(false);
	}
	if (m_nAnimCount % 420 == 0)
	{
		m_tStarPos[2].x = 0.0f - (rand() % 20 * 100.0f);
		if (!m_pStarEfc[2]->IsPlay())m_pStarEfc[2]->Play(false);
	}
	if (m_nAnimCount % 540 == 0)
	{
		m_tStarPos[3].x = 0.0f - (rand() % 20 * 100.0f);
		if (!m_pStarEfc[2]->IsPlay())m_pStarEfc[2]->Play(false);
	}

	for (int i = 0; i < MAX_STAR; i++)
	{
		if (!m_pStarEfc[i]->IsPlay()) m_tStarRotate[i].z = DirectX::XMConvertToRadians(240.0f);
		else  m_tStarRotate[i].z += DirectX::XMConvertToRadians(-0.1f);
	}


	m_nAnimCount++;
}

void CSceneTitle::OptionApply()
{
	static bool b = false;
	if (m_pOption->GetIsFullScreen() == 0 && !b)
	{

		SetFullscreenSwap();
		b ^= true;
	}
	else if (m_pOption->GetIsFullScreen() == 1 && b)
	{
		SetFullscreenSwap();
		b ^= true;
	}
	static int Resolusion = 1;
	static int OldResolusion = 1;
	OldResolusion = Resolusion;
	Resolusion = m_pOption->GetResolusion();
	if (Resolusion != OldResolusion)
	{

		switch (Resolusion)
		{
		case SCREEN_1920:
			SetNowResolusion(1920, 1080);
			SetResolusion(1920, 1080, b);
			InitResolusionMain();
			break;
		case SCREEN_1600:
			SetNowResolusion(1600, 900);
			SetResolusion(1600, 900, b);
			InitResolusionMain();
			break;
		case SCREEN_1280:
			SetNowResolusion(1280, 720);
			SetResolusion(1280, 720, b);
			InitResolusionMain();
			break;
		case SCREEN_800:
			SetNowResolusion(800, 600);
			SetResolusion(800, 600, b);
			InitResolusionMain();
			break;
		}
	}

	if (g_pSourseTitleBGM)SetVolumeBGM(g_pSourseTitleBGM);
}

bool CSceneTitle::CheckRankingCommand()
{
	if (IsKeyTrigger('R')) {
		m_bRankingCommand[0] = true;
		m_fRankingCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bRankingCommand[0]) {
		m_bRankingCommand[1] = true;
		m_fRankingCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('N') && m_bRankingCommand[1]) {
		m_bRankingCommand[2] = true;
		m_fRankingCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('K') && m_bRankingCommand[2]) {
		m_bRankingCommand[3] = true;
		m_fRankingCommandResetTimer = 0.0f;
	}

	m_fRankingCommandResetTimer += 1.0f / 60.0f;

	if (m_fRankingCommandResetTimer > 4.0f)
	{
		for (int i = 0; i < 4; i++)m_bRankingCommand[i] = false;
		m_fRankingCommandResetTimer = 0.0f;
	}

	return m_bRankingCommand[3];
}


