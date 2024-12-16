#include "StartTitle.h"
#include "DirectXTex/TextureLoad.h"
#include "SpriteDrawer.h"
#include "Defines.h"
#include "Input.h"
#include "Main.h"

#define LOGO_WIND (1000)
#define LOGO_HID (1000)
#define ACS1_WID (700)
#define ACS1_HID (700)
#define SET_POTION_X (-900)
#define SET_POTION_Y (0)
#define E_WID (540)
#define E_HID (300)
#define SELECT_POSX (-300.0f)

E_TITLE_TYPE g_Title_type;

CStartTitle::CStartTitle()
	:m_vLogo(nullptr)
	, m_tLogo(nullptr)
	,m_tAccessories{}
	,f_Rotation(0)
	,f_Rad(0)
	,f_SelectY(200)
{
	g_Title_type = GAMESTART;

	HRESULT hr;
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/B_1.png", &m_tBackGround);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/L_1.png", &m_tLogo);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/M_1.png", &m_tAccessories[0]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/M_2.png", &m_tAccessories[1]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/E_1.png", &m_tAccessories[2]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/Select.png", &m_tSelect);

	if (FAILED(hr)) {
		MessageBox(NULL, "BackGround 画像", "Error", MB_OK);
	}




	Vertex vtx[] = {
		//背景表示の座標
		{{-LOGO_WIND, -LOGO_HID, 0.0f}, {0.0f, 0.0f}},
		{{-LOGO_WIND,  LOGO_HID, 0.0f}, {0.0f, 1.0f}},
		{{ LOGO_WIND, -LOGO_HID, 0.0f}, {1.0f, 0.0f}},
		{{ LOGO_WIND,  LOGO_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx1[] = {
		//背景表示の座標
		{{-ACS1_WID, -ACS1_HID, 0.0f}, {0.0f, 0.0f}},
		{{-ACS1_WID,  ACS1_HID, 0.0f}, {0.0f, 1.0f}},
		{{ ACS1_WID, -ACS1_HID, 0.0f}, {1.0f, 0.0f}},
		{{ ACS1_WID,  ACS1_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx2[] = {
		//背景表示の座標
		{{-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f}, {0.0f, 0.0f}},
		{{-SCREEN_WIDTH,  SCREEN_HEIGHT, 0.0f}, {0.0f, 1.0f}},
		{{ SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f}, {1.0f, 0.0f}},
		{{ SCREEN_WIDTH,  SCREEN_HEIGHT, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx3[] = {
		//背景表示の座標
		{{-E_WID, -E_HID, 0.0f}, {0.0f, 0.0f}},
		{{-E_WID,  E_HID, 0.0f}, {0.0f, 1.0f}},
		{{ E_WID, -E_HID, 0.0f}, {1.0f, 0.0f}},
		{{ E_WID,  E_HID, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx4[] = {
		//選択するときの矢印
		{{-100.0f,-100.0f,0.0f},{0.0f, 0.0f}},
		{{-100.0f, 100.0f,0.0f},{0.0f, 1.0f}},
		{{ 100.0f,-100.0f,0.0f},{1.0f, 0.0f}},
		{{ 100.0f, 100.0f,0.0f},{1.0f, 1.0f}},

	};

	m_vACS = CreateVertexBuffer(vtx1, 4);
	m_vLogo = CreateVertexBuffer(vtx, 4);
	m_vBackGround = CreateVertexBuffer(vtx2, 4);
	m_vE = CreateVertexBuffer(vtx3, 4);
	m_vSelect = CreateVertexBuffer(vtx4, 4);
}

CStartTitle::~CStartTitle()
{
	

}

void CStartTitle::UpdateStartTitle()
{
	f_Rotation += 0.1f;
	f_Rad = DirectX::XMConvertToRadians(f_Rotation);

	switch (g_Title_type)
	{
	case(GAMESTART):
		if (IsKeyTrigger(VK_DOWN)) 
		{
			g_Title_type = GAMECONTINUE;
			f_SelectY += 200;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			ChangeScene(SCENE_GAME);
		}
		break;

	case(GAMECONTINUE):
		if (IsKeyTrigger(VK_DOWN))
		{
			g_Title_type = GAMEOPTION;
			f_SelectY += 200;
		}
		if (IsKeyTrigger(VK_UP))
		{
			g_Title_type = GAMESTART;
			f_SelectY -= 200;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			//コンティニューシーンへ切り替える処理
		}
		break;

	case(GAMEOPTION):
		if (IsKeyTrigger(VK_DOWN))
		{
			g_Title_type = GAMEEND;
			f_SelectY += 200;
		}
		if (IsKeyTrigger(VK_UP))
		{
			g_Title_type = GAMECONTINUE;
			f_SelectY -= 200;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			//オプションへ切り替える処理
		}
		break;

	case(GAMEEND):
		if (IsKeyTrigger(VK_UP)) 
		{
			g_Title_type = GAMEOPTION; 
			f_SelectY -= 200;
		}
		if (IsKeyTrigger(VK_RETURN))
		{
			SetGameEnd();
		}
		break;

	default:break;
	}
}

void CStartTitle::DrawStartTitle()
{
	//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す

	/*SetSpriteTexture(m_tBackGround);
	DrawSprite(m_vBackGround, sizeof(Vertex));
	ReSetSprite();
	SetSpritePos(SET_POTION_X, SET_POTION_Y);
	SetSpriteAngle(f_Rad);
	SetSpriteTexture(m_tAccessories[0]);
	DrawSprite(m_vACS, sizeof(Vertex));
	ReSetSprite();
	SetSpritePos(SET_POTION_X, SET_POTION_Y);
	SetSpriteAngle(f_Rad * -1);
	SetSpriteTexture(m_tAccessories[1]);
	DrawSprite(m_vACS, sizeof(Vertex));
	ReSetSprite();
	SetSpritePos(SET_POTION_X, SET_POTION_Y);
	SetSpriteTexture(m_tLogo);
	DrawSprite(m_vLogo, sizeof(Vertex));
	ReSetSprite();
	SetSpritePos(-SET_POTION_X, SET_POTION_Y);
	SetSpriteTexture(m_tAccessories[2]);
	DrawSprite(m_vE,sizeof(Vertex));
	ReSetSprite();*/
	SetSpritePos(SELECT_POSX, f_SelectY);
	SetSpriteTexture(m_tSelect);
	DrawSprite(m_vSelect, sizeof(Vertex));
	ReSetSprite();
}
