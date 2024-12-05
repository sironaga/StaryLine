// 3D用のSpriteを簡単にする拡張クラス

#include "SpriteEx.h"

SpriteEx::SpriteEx(const char *File)
{
	m_Texture = new Texture();
	m_Sprite = new Sprite();
	m_Texture->Create(File);
	T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));
	
	R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));
	S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

}

SpriteEx::~SpriteEx()
{
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
	if (m_Sprite)
	{
		delete m_Sprite;
		m_Sprite = nullptr;
	}


}

void SpriteEx::Disp()
{
	DirectX::XMMATRIX mat = S * R * T;
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = View;
	wvp[2] = Project;
	m_Sprite->SetView(wvp[1]);
	m_Sprite->SetProjection(wvp[2]);
	m_Sprite->SetWorld(wvp[0]);
	m_Sprite->Draw();
}

void SpriteEx::SetPositon(float X, float Y, float Z)
{
	T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		X,
		Y,
		Z,
		0.0f
	));
}

void SpriteEx::SetSize(float X, float Y, float Z)
{
	S = DirectX::XMMatrixScaling(X, Y, Z);
}

void SpriteEx::SetRotation(float X, float Y, float Z)
{
	R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		X,
		Y,
		Z,
		0.0f
	));
}

void SpriteEx::SetUvSize(float X, float Y)
{
	DirectX::XMFLOAT2 ReSize;
	ReSize.x = X;
	ReSize.y = Y;
	m_Sprite->SetUVScale(ReSize);
}

void SpriteEx::SetUvPos(float X, float Y)
{
	DirectX::XMFLOAT2 RePos;
	RePos.x = X;
	RePos.y = Y;
	m_Sprite->SetUVPos(RePos);
}

void SpriteEx::SetView(DirectX::XMFLOAT4X4 InView)
{
	View = InView;
}

void SpriteEx::SetProjection(DirectX::XMFLOAT4X4 InProjection)
{
	Project = InProjection;
}
