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
#include "Easing.h"

#define SELECT_MOVE (80.0f)
#define CENTER_POS_X SCREEN_WIDTH / 2.0f
#define CENTER_POS_Y SCREEN_HEIGHT / 2.0f
#define STAR_SPEED 2.0f
#define DECISION_SPEED 1.0f
#define SELECT_POW 1.2f
#define STAR_AJUSTPOS_X SCREEN_WIDTH / 2.0f
#define SELECT_JUST_Y 30.0f

constexpr float TAB_POS_Y = 500.0f;

bool CSceneTitle::m_bFirstPlay = false;

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
	HatFallEnd,
	LogoUpEnd,
	LogoMoveEnd,

	Select

}g_eTitleAnim;

ID3D11SamplerState* g_pTitleSPSampler;

CSceneTitle::CSceneTitle(COption* pOption)
	: m_SelectPos{550.0f, 160.0f}, m_bSelected(false)
	, m_DecisionPos{ SCREEN_WIDTH / 2.0f, 100.0f }
	, m_nAnimCount(0), m_tCharaLogoTexPos{}
	, m_nLiniYCount(0)
	, m_pEffect()
	,m_bChange(false)
	, m_tTabPos{}, m_tTabSize{}
	, m_fSelectScale(1.0f), m_tStarPos{ {0.0f,SCREEN_HEIGHT / 2.0f} }
	, m_pStarEfc{}, m_tStarRotate{}
	, m_Direction(XINPUT_GAMEPAD_START), m_bRankingCommand{},m_fRankingCommandResetTimer(0.0f)
	, m_fAnimTime(0.0f), m_fTabInitPosY(TAB_POS_Y)
{
	g_Title_type = GAMESTART;
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//if(FAILED(m_pSelect->Create(TEX_PASS("TitleBackGround/Select.png"))))MessageBox(NULL,"Select.png","Error",MB_OK);
	//テクスチャの読み込み
	/*tex = new Texture();
	tex->Create("Assets/Texture/Title/Title_Logo.png");*/
	//m_pTitleBack		 = new SpriteEx("Assets/Texture/Title/Title_BackBoard.png");
	m_pTitleFrame		 = new SpriteEx("Assets/Texture/Title/Title_Selected.png");
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

	m_pAnimSprite[(int)TitleAnim::Hat] = new SpriteEx("Assets/Texture/Title/Linie_Hat.png");
	m_pAnimSprite[(int)TitleAnim::Logo] = new SpriteEx("Assets/Texture/Title/Title_Logo.png");
	m_pAnimSprite[(int)TitleAnim::Linie] = new SpriteEx("Assets/Texture/Title/Title_Chara.png");
	m_pAnimSprite[(int)TitleAnim::UnderBar] = new SpriteEx("Assets/Texture/Title/Title_Underbar.png");

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
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());

	
	g_eTitleAnim = AnimeStart;
	m_AnimParam[(int)TitleAnim::Hat].pos = {0.0f,-300.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Hat].size = {1200.0f,-720.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Hat].rotate = {0.0f,0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Hat].uvPos = {0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Hat].uvSize = {1.0f,1.0f};
	
	m_AnimParam[(int)TitleAnim::Logo].pos = { CENTER_POS_X,SCREEN_HEIGHT + 400.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Logo].size = { 840.0f, -560.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Logo].rotate = {0.0f,0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Logo].uvPos = {0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Logo].uvSize = {1.0f,1.0f};

	m_AnimParam[(int)TitleAnim::Linie].pos = { -800.0f ,CENTER_POS_Y + 75.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Linie].size = { SCREEN_WIDTH, -SCREEN_HEIGHT,0.0f};
	m_AnimParam[(int)TitleAnim::Linie].rotate = {0.0f,0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Linie].uvPos = {0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::Linie].uvSize = {1.0f,1.0f};

	m_AnimParam[(int)TitleAnim::UnderBar].pos = { CENTER_POS_X, CENTER_POS_Y + 560.0f,0.0f};
	m_AnimParam[(int)TitleAnim::UnderBar].size = { 1920.0f, -60.0f,0.0f};
	m_AnimParam[(int)TitleAnim::UnderBar].rotate = {0.0f,0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::UnderBar].uvPos = {0.0f,0.0f};
	m_AnimParam[(int)TitleAnim::UnderBar].uvSize = {1.0f,1.0f};

	m_pOption = pOption; 
	g_pTitleBG = new CBackGround();

	g_TitleSound = new CSoundList(BGM_TITLE);
	g_pSourseTitleBGM = g_TitleSound->GetSound(true);
	g_pSourseTitleBGM->Start();
	
	for (int i = 0; i < 4; i++)
	{
		m_tTabPos[i] = { 1510.0f, CENTER_POS_Y + 180.0f + i * SELECT_MOVE };
		m_tTabSize[i] = { 500.0f,-75.0f };
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

	if (m_pTitleFrame)
	{
		delete m_pTitleFrame;
		m_pTitleFrame = nullptr;
	}

	if (m_pOption)
	{
		m_pOption = nullptr;
	}
	for (int i = 0; i < (int)Effect::Max; i++)
	{
		SAFE_DELETE(m_pEffect[i]);
	}
	for (int i = 0; i < (int)TitleAnim::Max; i++)
	{
		SAFE_DELETE(m_pAnimSprite[i]);
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
	if ((IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE)) && g_eTitleAnim != Select)
	{
		m_AnimParam[(int)TitleAnim::Linie].pos.x = CENTER_POS_X - 210.0f;
		m_AnimParam[(int)TitleAnim::Logo].pos.x = CENTER_POS_X + 500.0f;
		m_AnimParam[(int)TitleAnim::Logo].pos.y = CENTER_POS_Y - 200.0f;
		m_AnimParam[(int)TitleAnim::Logo].size.x = 780.0f;
		m_AnimParam[(int)TitleAnim::Logo].size.y = -580.0f;
		m_AnimParam[(int)TitleAnim::UnderBar].pos.y = CENTER_POS_Y + 500.0f;
		g_eTitleAnim = Select;

		return;
	}
	m_Direction = GetControllerLStickTriggerForeDirection();
	g_TitleSound->SetMasterVolume();

	//音量を設定
	SetAllMasterVolume(m_pOption->GetMasterVoluem());
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());

	g_pTitleBG->Update();
	TitleAnimation();
	TitleInput();
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

	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());

	g_pTitleBG->Draw();
	
	for (int i = 0; i < MAX_STAR; i++)
	{
		m_pStarEfc[i]->SetPos({ m_tStarPos[i].x + CENTER_POS_X,0.0f,0.0f});
		m_pStarEfc[i]->SetSize({ 1000.0f,1000.0f,100.0f });
		m_pStarEfc[i]->SetRotate(m_tStarRotate[i]);
		m_pStarEfc[i]->Draw(false);
	}





	m_pAnimSprite[(int)TitleAnim::Linie]->SetPositon(m_AnimParam[(int)TitleAnim::Linie].pos);
	m_pAnimSprite[(int)TitleAnim::Linie]->SetSize(m_AnimParam[(int)TitleAnim::Linie].size);
	m_pAnimSprite[(int)TitleAnim::Linie]->SetRotate(m_AnimParam[(int)TitleAnim::Linie].rotate);
	m_pAnimSprite[(int)TitleAnim::Linie]->SetUvPos(m_AnimParam[(int)TitleAnim::Linie].uvPos);
	m_pAnimSprite[(int)TitleAnim::Linie]->SetUvSize(m_AnimParam[(int)TitleAnim::Linie].uvSize);
	m_pAnimSprite[(int)TitleAnim::Linie]->SetView(Get2DView());
	m_pAnimSprite[(int)TitleAnim::Linie]->SetProjection(Get2DProj());
	m_pAnimSprite[(int)TitleAnim::Linie]->SetTexture();
	m_pAnimSprite[(int)TitleAnim::Linie]->Disp();

	m_pAnimSprite[(int)TitleAnim::Logo]->SetPositon(m_AnimParam[(int)TitleAnim::Logo].pos);
	m_pAnimSprite[(int)TitleAnim::Logo]->SetSize(m_AnimParam[(int)TitleAnim::Logo].size);
	m_pAnimSprite[(int)TitleAnim::Logo]->SetRotate(m_AnimParam[(int)TitleAnim::Logo].rotate);
	m_pAnimSprite[(int)TitleAnim::Logo]->SetUvPos(m_AnimParam[(int)TitleAnim::Logo].uvPos);
	m_pAnimSprite[(int)TitleAnim::Logo]->SetUvSize(m_AnimParam[(int)TitleAnim::Logo].uvSize);
	m_pAnimSprite[(int)TitleAnim::Logo]->SetView(Get2DView());
	m_pAnimSprite[(int)TitleAnim::Logo]->SetProjection(Get2DProj());
	m_pAnimSprite[(int)TitleAnim::Logo]->SetTexture();
	m_pAnimSprite[(int)TitleAnim::Logo]->Disp();
	if (g_eTitleAnim == Select)
	{

		m_pTitleStart[0]->SetTexture();
		m_pTitleStart[0]->SetProjection(Get2DProj());
		m_pTitleStart[0]->SetView(Get2DView());
		m_pTitleStart[0]->SetPositon(m_tTabPos[0].x, m_tTabPos[0].y, 0.0f);
		m_pTitleStart[0]->SetSize(m_tTabSize[0].x, m_tTabSize[0].y, 0.0f);
		m_pTitleStart[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pTitleStart[0]->Disp();

		m_pTitleContinued[0]->SetTexture();
		m_pTitleContinued[0]->SetProjection(Get2DProj());
		m_pTitleContinued[0]->SetView(Get2DView());
		m_pTitleContinued[0]->SetPositon(m_tTabPos[1].x, m_tTabPos[1].y, 0.0f);
		m_pTitleContinued[0]->SetSize(m_tTabSize[1].x, m_tTabSize[1].y, 0.0f);
		m_pTitleContinued[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
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
		m_pTitleEnd[0]->SetSize(m_tTabSize[3].x, m_tTabSize[3].y, 0.0f);
		m_pTitleEnd[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pTitleEnd[0]->Disp();


		if (m_bSelected && !m_pOption->GetOption())
		{
			switch (g_Title_type)
			{
			case GAMESTART:
				m_pTitleStart[1]->SetTexture();
				m_pTitleStart[1]->SetProjection(Get2DProj());
				m_pTitleStart[1]->SetView(Get2DView());
				m_pTitleStart[1]->SetPositon(m_tTabPos[0].x, m_tTabPos[0].y, 0.0f);
				m_pTitleStart[1]->SetSize(m_tTabSize[0].x, m_tTabSize[0].y, 0.0f);
				m_pTitleStart[1]->Disp();
				break;

			case GAMECONTINUE:
				m_pTitleContinued[1]->SetTexture();
				m_pTitleContinued[1]->SetProjection(Get2DProj());
				m_pTitleContinued[1]->SetView(Get2DView());
				m_pTitleContinued[1]->SetPositon(m_tTabPos[1].x, m_tTabPos[1].y, 0.0f);
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
		m_pTitleFrame->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pTitleFrame->Disp();

	}

	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetPositon(m_AnimParam[(int)TitleAnim::UnderBar].pos);
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetSize(m_AnimParam[(int)TitleAnim::UnderBar].size);
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetRotate(m_AnimParam[(int)TitleAnim::UnderBar].rotate);
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetUvPos(m_AnimParam[(int)TitleAnim::UnderBar].uvPos);
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetUvSize(m_AnimParam[(int)TitleAnim::UnderBar].uvSize);
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetView(Get2DView());
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetProjection(Get2DProj());
	m_pAnimSprite[(int)TitleAnim::UnderBar]->SetTexture();
	m_pAnimSprite[(int)TitleAnim::UnderBar]->Disp();


	m_pEffect[(int)Effect::Choice]->SetPos({ m_SelectPos.x - 255.0f,-m_SelectPos.y,0.0f });
	m_pEffect[(int)Effect::Choice]->SetSize({ 100.0f,100.0f,100.0f });
	m_pEffect[(int)Effect::Choice]->SetRotate({ 0.0f,0.0f,0.0f });
	m_pEffect[(int)Effect::Choice]->Draw(false);

	if (g_eTitleAnim == AnimeStart)
	{
		m_pAnimSprite[(int)TitleAnim::Hat]->SetPositon(m_AnimParam[(int)TitleAnim::Hat].pos);
		m_pAnimSprite[(int)TitleAnim::Hat]->SetSize(m_AnimParam[(int)TitleAnim::Hat].size);
		m_pAnimSprite[(int)TitleAnim::Hat]->SetRotate(m_AnimParam[(int)TitleAnim::Hat].rotate);
		m_pAnimSprite[(int)TitleAnim::Hat]->SetUvPos(m_AnimParam[(int)TitleAnim::Hat].uvPos);
		m_pAnimSprite[(int)TitleAnim::Hat]->SetUvSize(m_AnimParam[(int)TitleAnim::Hat].uvSize);
		m_pAnimSprite[(int)TitleAnim::Hat]->SetView(Get2DView());
		m_pAnimSprite[(int)TitleAnim::Hat]->SetProjection(Get2DProj());
		m_pAnimSprite[(int)TitleAnim::Hat]->SetTexture();
		m_pAnimSprite[(int)TitleAnim::Hat]->Disp();
	}

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

	SAFE_DELETE(m_pTitleFrame);
	m_pTitleFrame = new SpriteEx("Assets/Texture/Title/Title_Selected.png");
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
	SAFE_DELETE(m_pEffect[(int)Effect::Star]);
	m_pEffect[(int)Effect::Star] = new CEffectManager_sp("Assets/Effect/Sprite/BackGround_ShootginStars_2.png", 8, 8, STAR_SPEED);
	SAFE_DELETE(m_pDecition);
	m_pDecition = new SpriteEx("Assets/Effect/Sprite/Decision.png");
	SAFE_DELETE(m_pEffect[(int)Effect::Choice]);
	m_pEffect[(int)Effect::Choice] = new CEffectManager_sp("Assets/Effect/Sprite/Choice.png", 4, 11, 0.5f);
	for (int i = 0; i < (int)TitleAnim::Max; i++)
	{
		SAFE_DELETE(m_pAnimSprite[i]);
	}
	m_pAnimSprite[(int)TitleAnim::Hat] = new SpriteEx("Assets/Texture/Title/Linie_Hat.png");
	m_pAnimSprite[(int)TitleAnim::Logo] = new SpriteEx("Assets/Texture/Title/Title_Logo.png");
	m_pAnimSprite[(int)TitleAnim::Linie] = new SpriteEx("Assets/Texture/Title/Title_Chara.png");
	m_pAnimSprite[(int)TitleAnim::UnderBar] = new SpriteEx("Assets/Texture/Title/Title_Underbar.png");
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
	}
	//m_pTitleBack->SetProjection(Get2DProj());
	//m_pTitleBack->SetView(Get2DView());
	m_pTitleFrame->SetProjection(Get2DProj());
	m_pTitleFrame->SetView(Get2DView());


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
	//------------------------------------------------------------------------------------------
	static float frequency = 0.0f;
	static float deg = 0.0f;
	static float cossinVal = 0.0f;

	switch (g_eTitleAnim)
	{
	case AnimeStart:
		// 揺れの振幅
		if (frequency >= 10.0f)
		{
			frequency += 0.083161f;
			OutputDebugStringA(std::to_string(frequency).c_str());
			OutputDebugStringA("\n");
		}
		else
		{
			frequency = InEasing(m_fAnimTime, 7.5f, 10.0f, 2.0f); // 揺れの周波数
			OutputDebugStringA(std::to_string(frequency).c_str());
			OutputDebugStringA("\n");
		}

		m_AnimParam[(int)TitleAnim::Hat].rotate.z = sinf(frequency) * DirectX::XMConvertToRadians(30.0f);
		m_AnimParam[(int)TitleAnim::Hat].pos.x += 15.0f;
		m_AnimParam[(int)TitleAnim::Hat].pos.y += 10.0f;
		if (m_fAnimTime >= 3.0f)
		{
			m_fAnimTime = 0.0f;
			m_nAnimCount = 0;
			frequency = 0.0f;
			deg = 0.0f;
			cossinVal = 0.0f;
			g_eTitleAnim = HatFallEnd;
		}
		break;
	case HatFallEnd:
		cossinVal = sinf(DirectX::XMConvertToRadians(deg));
		m_AnimParam[(int)TitleAnim::Logo].pos.y = 0.0f + Easing39(m_fAnimTime, 0.5f, 0.9f) * CENTER_POS_Y;
		m_AnimParam[(int)TitleAnim::Logo].size.x = 1200.0f + cossinVal * 400.0f;
		m_AnimParam[(int)TitleAnim::Logo].size.y = -900.0f - cossinVal * 300.0f;
		deg += 5.0f;
		if (deg >= 180.0f)deg = 180.0f;
		if (m_fAnimTime >= 1.7f)
		{
			m_fAnimTime = 0.0f;
			m_nAnimCount = 0;
			frequency = 0.0f;
			deg = 0.0f;
			cossinVal = 0.0f;
			g_eTitleAnim = LogoUpEnd;
		}
		break;
	case LogoUpEnd:
		cossinVal = cosf(DirectX::XMConvertToRadians(deg));


		if (deg >= 180.0f)
		{
			deg = 180.0f;

		}
		else
		{
			if (deg <= 90.0f)
			{
				m_AnimParam[(int)TitleAnim::Logo].size.x = 800.0f * cossinVal;
				m_AnimParam[(int)TitleAnim::Logo].size.y = -600.0f * cossinVal;

				deg += 7.5f;
			}
			else if (m_fAnimTime > 0.6f)
			{
				m_AnimParam[(int)TitleAnim::Logo].pos.x = CENTER_POS_X + 500.0f;
				m_AnimParam[(int)TitleAnim::Logo].pos.y = CENTER_POS_Y - 200.0f;
				m_AnimParam[(int)TitleAnim::Logo].size.x = 780.0f * -cossinVal;
				m_AnimParam[(int)TitleAnim::Logo].size.y = -580.0f * -cossinVal;
				deg += 7.5f;
			}
		}

		
		if (m_fAnimTime >= 1.5f)
		{
			m_fAnimTime = 0.0f;
			m_nAnimCount = 0;
			frequency = 0.0f;
			deg = 0.0f;
			cossinVal = 0.0f;
			g_eTitleAnim = LogoMoveEnd;
		}
		break;
	case LogoMoveEnd:
		m_AnimParam[(int)TitleAnim::Linie].pos.x = 0.0f + Easing39(m_fAnimTime, 0.5f, 0.9f) * CENTER_POS_X - 210.0f;
		if (m_fAnimTime >= 0.8f)
		{
			m_fAnimTime = 0.0f;
			m_nAnimCount = 0;
			frequency = 0.0f;
			deg = 0.0f;
			cossinVal = 0.0f;
			g_eTitleAnim = Select;
		}
		break;
	case Select:
		EffectUpdate();
		if (m_AnimParam[(int)TitleAnim::UnderBar].pos.y <= CENTER_POS_Y + 500.0f)m_AnimParam[(int)TitleAnim::UnderBar].pos.y = CENTER_POS_Y + 500.0f;
		else m_AnimParam[(int)TitleAnim::UnderBar].pos.y = OutEasing(m_fAnimTime, CENTER_POS_Y + 560.0f, -60.0f, 1.0f);
		break;
	default:
		break;
	}




	m_nAnimCount++;
	m_fAnimTime += 1.0f / 60.0f;
}

void CSceneTitle::EffectUpdate()
{
	// エフェクト------------------------------------------------------------------
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

	m_AnimParam[(int)TitleAnim::Linie].pos.y = Y + cosMove * 50.0f;

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
			m_tTabPos[i] = { CENTER_POS_X + 550.0f, CENTER_POS_Y + 160.0f + i * SELECT_MOVE };
			m_tTabSize[i] = { 500.0f * SELECT_POW,-75.0f * SELECT_POW };
		}
		else
		{
			if (i < g_Title_type)m_tTabPos[i].y = CENTER_POS_Y + 200.0f + i * SELECT_MOVE - SELECT_JUST_Y;
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
}

void CSceneTitle::TitleInput()
{

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
			else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
			{
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
				m_pSelectsound->SetMasterVolume();
				if (m_pSourseSelectSE)SetVolumeSE(m_pSourseSelectSE);
				m_pSourseSelectSE->Start();
				m_bSelected = true;
				m_bFirstPlay = true;

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
			else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
			{
				//コンティニューシーンへ切り替える処理
				if (!OutStageData())return;
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())m_pEffect[(int)Effect::Choice]->Play(false);
				m_bSelected = true;
				m_bFirstPlay = false;
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
				else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
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
			else if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE) || CGetButtonsTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A)))
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
				if (!m_pEffect[(int)Effect::Choice]->IsPlay())
				{
					SetNext(STAGE_SELECT);
					m_bChange = true;
				}
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

bool CSceneTitle::IsFirstPlay()
{
	return m_bFirstPlay;
}

void CSceneTitle::SetPlayed()
{
	m_bFirstPlay = false;
}
