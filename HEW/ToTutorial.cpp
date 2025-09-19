#include "ToTutorial.h"
#include "Defines.h"
#include "Sprite.h"
#include "Oparation.h"
#include "InputEx.h"
#include "Option.h"
#include "Main.h"
#include "Easing.h"

#define MOVE_SPEED 0.5f
#undef min

constexpr DirectX::XMFLOAT2 ce_f2DownPos = DirectX::XMFLOAT2(0.0f, 0.0f);
constexpr DirectX::XMFLOAT2 ce_f2UpPos = DirectX::XMFLOAT2(0.0f, SCREEN_HEIGHT);
constexpr DirectX::XMFLOAT2 ce_f2ButtonOffSet = DirectX::XMFLOAT2(300.0f, 200.0f);

CToTutorial::CToTutorial()
	: m_pTexture{}, m_tParam{}
	, m_eMove(ScreenMove::None), m_fTime(0.0f)
	, m_nSelect(1)
{
	const char* chPath[(int)TutorialTextures::Max] =
	{
		TEX_PASS("Tutorial/BackGround.png"),
		TEX_PASS("Tutorial/YesButton.png"),
		TEX_PASS("Tutorial/NoButton.png"),
		TEX_PASS("Tutorial/Select.png"),
	};

	for (int i = 0; i < (int)TutorialTextures::Max; i++)
	{
		m_pTexture[i] = new Texture();
		m_pTexture[i]->Create(chPath[i]);
		m_tParam[i] = SpriteParam();
		m_tParam[i].pos = ce_f2UpPos;
		m_tParam[i].world = Get2DWorld();
		m_tParam[i].view = Get2DView();
		m_tParam[i].proj = Get2DProj();
	}

	m_tParam[(int)TutorialTextures::Back].size = DirectX::XMFLOAT2(SCREEN_WIDTH -320, SCREEN_HEIGHT - 180);
	m_tParam[(int)TutorialTextures::YesButton].size = DirectX::XMFLOAT2(200.0f, 100.0f);
	m_tParam[(int)TutorialTextures::NoButton].size = DirectX::XMFLOAT2(200.0f, 100.0f);
	m_tParam[(int)TutorialTextures::Select].size = DirectX::XMFLOAT2(200.0f, 100.0f);
}

CToTutorial::~CToTutorial()
{
	for (int i = 0; i < (int)TutorialTextures::Max; i++)
	{
		SAFE_DELETE(m_pTexture[i]);
	}
}

void CToTutorial::Update()
{
	m_fTime += 1.0f / 60.0f;
	m_fTime = std::min(m_fTime, MOVE_SPEED);

	switch (m_eMove)
	{
	case ScreenMove::None:
		m_fTime = 0.0f;
		break;
	case ScreenMove::ScreenUp:
		m_tParam[(int)TutorialTextures::Back].pos = ((ce_f2UpPos - ce_f2DownPos) * EaseOutQuint(m_fTime, MOVE_SPEED)) + ce_f2DownPos;
		if (m_fTime >= MOVE_SPEED) m_eMove = ScreenMove::None;
		break;
	case ScreenMove::ScreenDown:
		m_tParam[(int)TutorialTextures::Back].pos = ((ce_f2DownPos - ce_f2UpPos) * EaseOutQuint(m_fTime, MOVE_SPEED)) + ce_f2UpPos;
		if (m_fTime >= MOVE_SPEED) m_eMove = ScreenMove::None;
		break;
	default:
		break;
	}
	m_tParam[(int)TutorialTextures::YesButton].pos = DirectX::XMFLOAT2(m_tParam[(int)TutorialTextures::Back].pos.x + ce_f2ButtonOffSet.x, m_tParam[(int)TutorialTextures::Back].pos.y - ce_f2ButtonOffSet.y);
	m_tParam[(int)TutorialTextures::NoButton].pos = DirectX::XMFLOAT2(m_tParam[(int)TutorialTextures::Back].pos.x - ce_f2ButtonOffSet.x, m_tParam[(int)TutorialTextures::Back].pos.y - ce_f2ButtonOffSet.y);
	
	if (m_nSelect == 0) m_tParam[(int)TutorialTextures::Select].pos = m_tParam[(int)TutorialTextures::NoButton].pos;
	else m_tParam[(int)TutorialTextures::Select].pos = m_tParam[(int)TutorialTextures::YesButton].pos;
}

void CToTutorial::Draw()
{
	SetRender2D();
	for (int i = 0; i < (int)TutorialTextures::Max; i++)
	{
		Sprite::ReSetSprite();
		Sprite::SetParam(m_tParam[i]);
		Sprite::SetTexture(m_pTexture[i]);
		Sprite::Draw();
	}
}

void CToTutorial::SetMove(ScreenMove inMove)
{
	m_eMove = inMove;
}

ScreenMove CToTutorial::GetMove()
{
	return m_eMove;
}

bool CToTutorial::TutorialInput()
{
	if (WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT) || IsKeyTrigger('D') ||
		WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT) || IsKeyTrigger('A'))
	{
		m_nSelect ^= 1;
	}

	if (WihtGetKeyPress(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A), VK_SPACE) || IsKeyTrigger(VK_RETURN))
	{
		return true;
	}

	return false;
}