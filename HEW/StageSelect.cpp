#include "StageSelect.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#define _DIRECTX_
#include "DirectX.h"
#include "Defines.h"
#include "SpriteDrawer.h"
#include "InputEx.h"
#include "SoundList.h"

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
{
	g_Select_type.StageMainNumber = DESERT;
	g_Select_type.StageSubNumber = STAGE1;

	HRESULT hr;
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("StageSelectBackGround/1_STAGE1.png"), &m_tStage1_BackGround);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("StageSelectBackGround/1_STAGE2.png"), &m_tStage2_BackGround);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("StageSelectBackGround/1_STAGE3.png"), &m_tStage3_BackGround);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("StageSelectBackGround/Right_Select.png"), &m_tRight_Select);
	hr = LoadTextureFromFile(GetDevice(), TEX_PASS("StageSelectBackGround/Left_Select.png"), &m_tLeft_Select);

	if (FAILED(hr)) {
		MessageBox(NULL, "BackGround âÊëú", "Error", MB_OK);
	}




	Vertex vtx[] = {
		//îwåiï\é¶ÇÃç¿ïW
		{{-LOGO_WIND, -LOGO_HID, 0.0f}, {0.0f, 0.0f}},
		{{-LOGO_WIND,  LOGO_HID, 0.0f}, {0.0f, 1.0f}},
		{{ LOGO_WIND, -LOGO_HID, 0.0f}, {1.0f, 0.0f}},
		{{ LOGO_WIND,  LOGO_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx1[] = {
		//îwåiï\é¶ÇÃç¿ïW
		{{-ACS1_WID, -ACS1_HID, 0.0f}, {0.0f, 0.0f}},
		{{-ACS1_WID,  ACS1_HID, 0.0f}, {0.0f, 1.0f}},
		{{ ACS1_WID, -ACS1_HID, 0.0f}, {1.0f, 0.0f}},
		{{ ACS1_WID,  ACS1_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx2[] = {
		//îwåiï\é¶ÇÃç¿ïW
		{{-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f}, {0.0f, 0.0f}},
		{{-SCREEN_WIDTH,  SCREEN_HEIGHT, 0.0f}, {0.0f, 1.0f}},
		{{ SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f}, {1.0f, 0.0f}},
		{{ SCREEN_WIDTH,  SCREEN_HEIGHT, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx3[] = {
		//îwåiï\é¶ÇÃç¿ïW
		{{-E_WID, -E_HID, 0.0f}, {0.0f, 0.0f}},
		{{-E_WID,  E_HID, 0.0f}, {0.0f, 1.0f}},
		{{ E_WID, -E_HID, 0.0f}, {1.0f, 0.0f}},
		{{ E_WID,  E_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx4[] = {
		//ëIëÇ∑ÇÈÇ∆Ç´ÇÃñÓàÛ
		{{-100.0f,-100.0f,0.0f},{0.0f, 0.0f}},
		{{-100.0f, 100.0f,0.0f},{0.0f, 1.0f}},
		{{ 100.0f,-100.0f,0.0f},{1.0f, 0.0f}},
		{{ 100.0f, 100.0f,0.0f},{1.0f, 1.0f}},

	};

	m_vSTAGE1_BackGround = CreateVertexBuffer(vtx2, 4);
	m_vSTAGE2_BackGround = CreateVertexBuffer(vtx2, 4);
	m_vSTAGE3_BackGround = CreateVertexBuffer(vtx2, 4);
	m_vRight_Select = CreateVertexBuffer(vtx4, 4);
	m_vLeft_Select = CreateVertexBuffer(vtx4, 4);
}

CStageSelect::~CStageSelect()
{

	
}

void CStageSelect::Update()
{
	switch (g_Select_type.StageSubNumber)
	{
	case(STAGE1):	
		if (IsKeyTrigger(VK_RIGHT)|| CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type.StageSubNumber = STAGE2; }
		//if (IsKeyTrigger(VK_LEFT)|| CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type = STAGE3; }
		break;

	case(STAGE2):
		if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type.StageSubNumber = STAGE3; }
		if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type.StageSubNumber = STAGE1; }
		break;

	case(STAGE3):
		//if (IsKeyTrigger(VK_RIGHT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE1; }
		if (IsKeyTrigger(VK_LEFT) || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type.StageSubNumber = STAGE2; }
	default: break;
	}
	if (IsKeyTrigger(VK_RETURN) || CGetButtonsTriger(XINPUT_GAMEPAD_B)) { SetNext(SCENE_GAME, g_Select_type); }//ÉVÅ[Éìà⁄çs
}

void CStageSelect::Draw()
{
	switch (g_Select_type.StageSubNumber)
	{
	case(STAGE1):
	{
		SetSpritePos(0.0f, 0.0f);
		SetSpriteTexture(m_tStage1_BackGround);
		DrawSprite(m_vSTAGE1_BackGround, sizeof(Vertex));
		ReSetSprite();

		SetSpritePos(ARROW_RIGHTX, ARROW_RIGHTY);
		SetSpriteTexture(m_tRight_Select);
		DrawSprite(m_vRight_Select, sizeof(Vertex));
		ReSetSprite();
		break;
	}
	case(STAGE2):
	{
		SetSpritePos(0.0f, 0.0f);
		SetSpriteTexture(m_tStage2_BackGround);
		DrawSprite(m_vSTAGE2_BackGround, sizeof(Vertex));
		ReSetSprite();

		SetSpritePos(ARROW_RIGHTX, ARROW_RIGHTY);
		SetSpriteTexture(m_tRight_Select);
		DrawSprite(m_vRight_Select, sizeof(Vertex));
		ReSetSprite();

		SetSpritePos(ARROW_LEFTX, ARROW_LEFTY);
		SetSpriteTexture(m_tLeft_Select);
		DrawSprite(m_vLeft_Select, sizeof(Vertex));
		ReSetSprite();
		break;
	}
	case(STAGE3):
	{
		SetSpritePos(0.0f, 0.0f);
		SetSpriteTexture(m_tStage3_BackGround);
		DrawSprite(m_vSTAGE3_BackGround, sizeof(Vertex));
		ReSetSprite();

		SetSpritePos(ARROW_LEFTX, ARROW_LEFTY);
		SetSpriteTexture(m_tLeft_Select);
		DrawSprite(m_vLeft_Select, sizeof(Vertex));
		ReSetSprite();
		break;
	}
	}
}

//int CStageSelect::GetStageNum()
//{
//	return g_Select_type;
//}
