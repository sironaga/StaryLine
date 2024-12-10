#include "BackGround.h"
#include "Defines.h"
#include <map>

constexpr float BG_POS_Z = 800.0f;

const char* BGPass[SCENE_MAX] =
{
	TEX_PASS("BackGround/B_1.png"),
	TEX_PASS("BackGround/B_1.png"),
	TEX_PASS("BackGround/B_1.png"),
};

CBackGround::CBackGround()
	: m_pBGSprite(nullptr)
	, m_eCurrentScene(GetScene())
{

	switch (m_eCurrentScene)
	{
	case SCENE_TITLE:
		// m_pBGSprite = new SpriteEx(BGPass[SCENE_TITLE]);
		break;
	case STAGE_SELECT:
		// m_pBGSprite = new SpriteEx(BGPass[STAGE_SELECT]);
		break;
	case SCENE_GAME:
		m_pBGSprite = new SpriteEx(BGPass[SCENE_GAME]);
		break;
	case SCENE_RESULT:
		// m_pBGSprite = new SpriteEx(BGPass[SCENE_RESULT]);
		break;
	case SCENE_DEBUGROOM:
		// m_pBGSprite = new SpriteEx(BGPass[SCENE_DEBUGROOM]);
		break;
	case SCENE_MAX:
		break;
	default:
		MessageBox(NULL, "シーン参照エラー", "BackGround.cpp", MB_OK);
		break;
	}
}

CBackGround::~CBackGround()
{
	SAFE_DELETE(m_pBGSprite);
}

void CBackGround::Update()
{

}

void CBackGround::Draw()
{
	switch (m_eCurrentScene)
	{
	case SCENE_TITLE: SetRender2D();	break;
	case STAGE_SELECT: SetRender2D();	break;
	case SCENE_GAME: SetRender3D();		break;
	case SCENE_RESULT: SetRender2D();	break;
	case SCENE_DEBUGROOM: SetRender3D();break;
	case SCENE_MAX: break;
	default:MessageBox(NULL, "シーン参照エラー", "BackGround.cpp", MB_OK);	break;
	}
	m_pBGSprite->SetPositon(0.0f, 0.0f, BG_POS_Z);
	m_pBGSprite->SetSize(SCREEN_WIDTH , SCREEN_WIDTH, 0.0f);
	m_pBGSprite->SetRotation(0.0f, 0.0f, 0.0f);
	m_pBGSprite->SetUvPos(0.0f, 0.0f);
	m_pBGSprite->SetUvSize(1.0f, 1.0f);
	m_pBGSprite->SetView(GetView());
	m_pBGSprite->SetProjection(GetProj());

	m_pBGSprite->SetTexture();
	m_pBGSprite->Disp();
}
