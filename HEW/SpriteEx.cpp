// 3D用のSpriteを簡単にする拡張クラス

#include "SpriteEx.h"
#include "Main.h"
#include <cmath>

SpriteEx::SpriteEx(const char *File)
{
	m_Texture = new Texture();
	m_Sprite = new Sprite();
	if (FAILED(m_Texture->Create(File))) MessageBox(NULL, "画像エラー", File, MB_OK);
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
	View = GetView();
	Project = GetProj();
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
	DirectX::XMFLOAT4 C;
	C.x = C.w = C.y = C.z = 1.0f;

	m_Sprite->SetColor(C);
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

void SpriteEx::SetTexture()
{
	m_Sprite->SetTexture(m_Texture);
}

void SpriteEx::SetCenterPosAndRotation(DirectX::XMFLOAT3 StartPos, DirectX::XMFLOAT3 NowPos)
{
	DirectX::XMFLOAT3 Calculated;
	Calculated.x = ((StartPos.x + NowPos.x) / 2);
	Calculated.y = ((StartPos.y + NowPos.y) / 2);
	Calculated.z - ((StartPos.z + NowPos.z) / 2);

	T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		Calculated.x,
		Calculated.y,
		Calculated.z,
		0.0f
	));

	DirectX::XMFLOAT3 delta;
	delta.x = NowPos.x - StartPos.x;
	delta.y = NowPos.y - StartPos.y;
	delta.z = NowPos.z - StartPos.z;

	DirectX::XMFLOAT3 Theta;
	Theta.x = std::atan2(delta.z, delta.y);
	Theta.y = std::atan2(delta.x, delta.z);
	Theta.z = std::atan2(delta.y, delta.x);

	R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		Theta.x,
		Theta.y,
		Theta.z,
		0.0f
	));

	S = DirectX::XMMatrixScaling(delta.x, delta.y, delta.z);
}

void SpriteEx::SetView(DirectX::XMFLOAT4X4 InView)
{
	View = InView;
}

void SpriteEx::SetProjection(DirectX::XMFLOAT4X4 InProjection)
{
	Project = InProjection;
}

