#include "BackGround.h"
#include "Defines.h"
#include "Sprite.h"
#include <map>

constexpr float BG_POS_Z = 600.0f;

const char* BGPass[SCENE_MAX] =
{
	TEX_PASS("BackGround/Sky.png"),
	TEX_PASS("BackGround/Sky.png"),
	TEX_PASS("BackGround/Sky.png"),
};

CBackGround::CBackGround()
	: m_pBGSprite(nullptr)
	, m_tPos{ 0.0f,0.0f}, m_tSize{ SCREEN_WIDTH * 3,SCREEN_WIDTH * 3 }, m_tRotate{}
	, m_tPosTex{ 0.0f,0.0f }, m_tSizeTex(1.0f, 1.0f)
	, m_eCurrentScene(GetScene()), m_tWorld{ 0.0f,5.0f,BG_POS_Z }
{

	m_pBGSprite = new Texture();
	switch (m_eCurrentScene)
	{
	case SCENE_TITLE:
		m_pBGSprite->Create(BGPass[SCENE_TITLE]);
		break;
	case STAGE_SELECT:
		 m_pBGSprite->Create(BGPass[STAGE_SELECT]);
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

}
void CBackGround::Draw()
{
	SetRender2D();

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT2 size;
	DirectX::XMMATRIX T;
	DirectX::XMMATRIX S;
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(m_tRotate.x,m_tRotate.y,m_tRotate.z,0.0f) );

	switch (m_eCurrentScene)
	{
	case SCENE_TITLE:
	case SCENE_RESULT:
		size = { SCREEN_WIDTH, SCREEN_HEIGHT };
		Sprite::SetView(Get2DView());
		Sprite::SetProjection(Get2DProj());
		SetRender2D();
		T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
		S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);
		break;
	case STAGE_SELECT:
	case SCENE_GAME:
		size = { 1.0f, 1.0f };
		Sprite::SetView(GetView());
		Sprite::SetProjection(GetProj());
		SetRender3D();
		T = DirectX::XMMatrixTranslation(m_tWorld.x, m_tWorld.y, m_tWorld.z);
		S = DirectX::XMMatrixScaling(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, 1.0f);
		break;
	case SCENE_DEBUGROOM: SetRender3D(); break;
	case SCENE_MAX: break;
	default:MessageBox(NULL, "シーン参照エラー", "BackGround.cpp", MB_OK);	break;
	}

	DirectX::XMMATRIX mWorld = S * R * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);
	Sprite::SetWorld(world);

	Sprite::SetSize(size);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::SetUVPos(m_tPosTex);
	Sprite::SetUVScale(m_tSizeTex);
	Sprite::SetTexture(m_pBGSprite);
	Sprite::Draw();
	Sprite::ReSetSprite();
}

void CBackGround::MultiScrollPos(DirectX::XMFLOAT3 MovePower)
{
	m_tWorld.x = MovePower.x / 100.0f;
	m_tWorld.y = MovePower.y / 100.0f + 5.0f;
	m_tWorld.z = MovePower.z / 100.0f + BG_POS_Z;
}

void CBackGround::MultiScrollRotate(DirectX::XMFLOAT3 MovePower)
{
	m_tRotate.x = MovePower.x / 50.0f;
	m_tRotate.y = MovePower.y / 50.0f;
	m_tRotate.z = MovePower.z / 50.0f;
}