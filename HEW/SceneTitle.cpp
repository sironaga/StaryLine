#include "SceneTitle.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Main.h"
#include "Controller.h"
#include "Input.h"
#include "DirectXTex/TextureLoad.h"
#include "SpriteDrawer.h"

#define SELECT_POSX (-320.0f)
#define SELECT_MOVE (185.0f)

enum E_TITLE_TYPE
{
	GAMESTART = 0,
	GAMECONTINUE,
	GAMEOPTION,
	GAMEEND,
}g_Title_type;


CSceneTitle::CSceneTitle()
	:f_SelectY(350.0f), m_vSelect(nullptr), m_tSelect(nullptr)
{
	g_Title_type = GAMESTART;
	g_TitleSound = new CSoundList(BGM_TITLE);
	g_pSourseTitleBGM = g_TitleSound->GetSound(true);
	g_pSourseTitleBGM->Start();

	//選択するときの矢印
	Vertex vtx4[] = {
	{{-100.0f,-100.0f,0.0f},{0.0f, 0.0f}},
	{{-100.0f, 100.0f,0.0f},{0.0f, 1.0f}},
	{{ 100.0f,-100.0f,0.0f},{1.0f, 0.0f}},
	{{ 100.0f, 100.0f,0.0f},{1.0f, 1.0f}},

	};
	m_vSelect = CreateVertexBuffer(vtx4, 4);
	HRESULT hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/Select.png", &m_tSelect);
	if (FAILED(hr)) MessageBox(NULL, "BackGround 画像", "Error", MB_OK);

	m_pOption = new COption();
	g_pTitleBG = new CBackGround();
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
}

void CSceneTitle::Update()
{
	g_pTitleBG->Update();

	//音量を設定
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());

	m_pOption->Update();

	switch (g_Title_type)
	{
	case(GAMESTART):
		if (IsKeyTrigger(VK_DOWN))
		{
			g_Title_type = GAMECONTINUE;
			f_SelectY += SELECT_MOVE;
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
			f_SelectY += SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_UP))
		{
			g_Title_type = GAMESTART;
			f_SelectY -= SELECT_MOVE;
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
				f_SelectY += SELECT_MOVE;
			}
			if (IsKeyTrigger(VK_UP))
			{
				g_Title_type = GAMECONTINUE;
				f_SelectY -= SELECT_MOVE;
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
			f_SelectY -= SELECT_MOVE;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			SetGameEnd();
		}
		break;

	default:break;
	}
	if (IsKeyTrigger(VK_TAB) || CGetButtons(XINPUT_GAMEPAD_X))
	{

		SetNext(SCENE_DEBUGROOM);
	}
	if (g_pSourseTitleBGM)SetVolumeBGM(g_pSourseTitleBGM);

	if (IsKeyTrigger(VK_ESCAPE))SetFullscreenSwap();
}

void CSceneTitle::Draw()
{
	g_pTitleBG->Draw();
	SetSpritePos(SELECT_POSX, f_SelectY);
	SetSpriteTexture(m_tSelect);
	DrawSprite(m_vSelect, sizeof(Vertex));
	ReSetSprite();

	m_pOption->Draw();
}
