#include "BackGround.h"
#include "Defines.h"
#include <map>

constexpr float BG_POS_Z = 600.0f;

const char* BGPass[SCENE_MAX] =
{
	TEX_PASS("BackGround/B_1.png"),
	TEX_PASS("BackGround/B_1.png"),
	TEX_PASS("BackGround/B_1.png"),
};

CBackGround::CBackGround()
	: m_pBGSprite(nullptr)
	, m_tPos{ 0.0f,0.0f,BG_POS_Z }, m_tSize{ SCREEN_WIDTH,SCREEN_WIDTH,0.0f }, m_tRotate{}
	, m_tPosTex{ 0.0f,0.0f }, m_tSizeTex(1.0f, 1.0f)
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
	switch (m_eCurrentScene)
	{
	case SCENE_TITLE: SetRender2D();	break;
	case STAGE_SELECT: SetRender2D();	break;
	case SCENE_GAME:
	{
		m_tRotate.z += DirectX::XMConvertToRadians(0.02f);
		break;
	}
	case SCENE_RESULT: SetRender2D();	break;
	case SCENE_DEBUGROOM: SetRender3D(); break;
	case SCENE_MAX: break;
	default:MessageBox(NULL, "シーン参照エラー", "BackGround.cpp", MB_OK);	break;
	}
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

	m_pBGSprite->SetPositon(m_tPos.x, m_tPos.y, m_tPos.z);
	m_pBGSprite->SetSize(m_tSize.x, m_tSize.y, m_tSize.z);
	m_pBGSprite->SetRotation(m_tRotate.x, m_tRotate.y, m_tRotate.z);
	m_pBGSprite->SetUvPos(m_tPosTex.x, m_tPosTex.y);
	m_pBGSprite->SetUvSize(m_tSizeTex.x, m_tSizeTex.y);
	m_pBGSprite->SetView(GetView());
	m_pBGSprite->SetProjection(GetProj());

	m_pBGSprite->SetTexture();
	m_pBGSprite->Disp();
}
