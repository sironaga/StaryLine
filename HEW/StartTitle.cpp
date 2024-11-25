#include "StartTitle.h"
#include "DirectXTex/TextureLoad.h"
#include"SpriteDrawer.h"
#define LOGO_WIND (1000)
#define LOGO_HID (1000)
#define ACS1_WID (700)
#define ACS1_HID (700)
#define SET_POTION_X (-900)
#define SET_POTION_Y (0)
#define E_WID (540)
#define E_HID (300)

CStartTitle::CStartTitle()
	:m_vLogo(nullptr)
	, m_tLogo(nullptr)
	,m_tAccessories{}
	,f_Rotation(0)
	,f_Rad(0)
{

	HRESULT hr;
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/B_1.png", &m_tBackGround);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/L_1.png", &m_tLogo);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/M_1.png", &m_tAccessories[0]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/M_2.png", &m_tAccessories[1]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/TitleBackGround/E_1.png", &m_tAccessories[2]);
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
		{{-WIND_W, -WIND_H, 0.0f}, {0.0f, 0.0f}},
		{{-WIND_W,  WIND_H, 0.0f}, {0.0f, 1.0f}},
		{{ WIND_W, -WIND_H, 0.0f}, {1.0f, 0.0f}},
		{{ WIND_W,  WIND_H, 0.0f}, {1.0f, 1.0f}},
	};

	Vertex vtx3[] = {
		//îwåiï\é¶ÇÃç¿ïW
		{{-E_WID, -E_HID, 0.0f}, {0.0f, 0.0f}},
		{{-E_WID,  E_HID, 0.0f}, {0.0f, 1.0f}},
		{{ E_WID, -E_HID, 0.0f}, {1.0f, 0.0f}},
		{{ E_WID,  E_HID, 0.0f}, {1.0f, 1.0f}},
	};


	m_vACS = CreateVertexBuffer(vtx1, 4);
	m_vLogo = CreateVertexBuffer(vtx, 4);
	m_vBackGround = CreateVertexBuffer(vtx2, 4);
	m_vE = CreateVertexBuffer(vtx3, 4);
}

CStartTitle::~CStartTitle()
{
	

}

void CStartTitle::UpdateStartTitle()
{
	f_Rotation += 0.1f;
	f_Rad = RadianConversion(f_Rotation);
}

void CStartTitle::DrawStartTitle()
{
	//ÇªÇÃëºÅAï\é¶Ç…ïKóvÇ»SpriteDrawer.hÇÃäeéÌä÷êîÇåƒÇ—èoÇ∑

	SetSpriteTexture(m_tBackGround);
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
	ReSetSprite();


}
