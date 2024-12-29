#include "FadeBlack.h"
#include "Main.h"
#include "Sprite.h"
#include "Defines.h"

CFadeBlack::CFadeBlack()
	: m_pTexture(nullptr) ,m_pPS(nullptr)
{
	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create("Asset/Fade/Fade.png")))MessageBox(NULL, "Load failed FadeBlack.", "Error", MB_OK);
 }

CFadeBlack::~CFadeBlack()
{
	SAFE_DELETE(m_pTexture);
 }

void CFadeBlack::DrawFadeIn(float alpha)
{
	DrawFade(alpha);
}

void CFadeBlack::DrawFadeOut(float alpha)
{
	DrawFade(alpha);
}

void CFadeBlack::DrawFade(float alpha)
{
	SetRender2D();

	// フェードの表示設定 
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);// フレームは倍率を変更せずに表示 
	DirectX::XMMATRIX mWorld = S * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);

	Sprite::SetWorld(world);
	Sprite::SetView(Get2DView());
	Sprite::SetProjection(Get2DProj());
	Sprite::SetTexture(m_pTexture);
	Sprite::SetSize(size);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, alpha });

	//Sprite::SetPixelShader(m_pPS);
	//m_pPS->SetTexture(0,m_pTexture);

	Sprite::Draw();
}