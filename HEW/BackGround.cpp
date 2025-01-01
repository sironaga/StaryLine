#include "BackGround.h"
#include "Defines.h"
#include "Sprite.h"
#include <map>

constexpr float BG_POS_Z = 600.0f;

const char* BGPass[SCENE_MAX] =
{
	TEX_PASS("BackGround/TitleBackGround.png"),
	TEX_PASS("BackGround/B_1.png"),
	TEX_PASS("BackGround/B_1.png"),
};

CBackGround::CBackGround()
	: m_pBGSprite(nullptr)
	, m_tPos{ 0.0f,0.0f}, m_tSize{ SCREEN_WIDTH,SCREEN_WIDTH }, m_tRotate{}
	, m_tPosTex{ 0.0f,0.0f }, m_tSizeTex(1.0f, 1.0f)
	, m_eCurrentScene(GetScene())
{

	m_pBGSprite = new Texture();
	switch (m_eCurrentScene)
	{
	case SCENE_TITLE:
		m_pBGSprite->Create(BGPass[SCENE_TITLE]);
		break;
	case STAGE_SELECT:
		// m_pBGSprite = new SpriteEx(BGPass[STAGE_SELECT]);
		break;
	case SCENE_GAME:
		m_pBGSprite->Create(BGPass[SCENE_GAME]);
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
	SetRender2D();

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(m_tRotate.x,m_tRotate.y,m_tRotate.z,0.0f) );
	DirectX::XMMATRIX mWorld = S * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);

	Sprite::SetSize(size);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::SetUVPos(m_tPosTex);
	Sprite::SetUVScale(m_tSizeTex);
	Sprite::SetWorld(world);
	Sprite::SetView(Get2DView());
	Sprite::SetProjection(Get2DProj());
	Sprite::SetTexture(m_pBGSprite);
	Sprite::Draw();
	Sprite::ReSetSprite();
}
