#include "ToTutorial.h"
#include "Defines.h"
#include "Sprite.h"

CToTutorial::CToTutorial()
	: m_pTexture{}
{
	const char* chPath[] =
	{
		TEX_PASS(""),
		TEX_PASS(""),
		TEX_PASS(""),
	};

	for (int i = 0; i < (int)TutorialTextures::Max; i++)
	{
		m_pTexture[i] = new Texture();
		m_pTexture[i]->Create(chPath[i]);
		m_tParam[i] = SpriteParam();
	}

	m_tParam[(int)TutorialTextures::Back].size = DirectX::XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_tParam[(int)TutorialTextures::YesButton].size = DirectX::XMFLOAT2(200.0f, 100.0f);
	m_tParam[(int)TutorialTextures::NoButton].size = DirectX::XMFLOAT2(200.0f, 100.0f);
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

}

void CToTutorial::Draw()
{
	for (int i = 0; i < (int)TutorialTextures::Max; i++)
	{
		Sprite::SetParam(m_tParam[i]);
	}
}
