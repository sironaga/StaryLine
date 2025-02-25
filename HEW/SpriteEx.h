#pragma once

#include "Sprite.h"
#include "Texture.h"
#include "_StructList.h"
#include "Defines.h"

class SpriteEx
{
public:
	SpriteEx(const char *File);
	~SpriteEx();

	void Disp();

	void SetPositon(float X, float Y, float Z);
	void SetPositon(DirectX::XMFLOAT3 Pos);
	void SetSize(float X, float Y, float Z);
	void SetSize(DirectX::XMFLOAT3 Size);
	void SetRotation(float X, float Y, float Z);
	void SetRotate(DirectX::XMFLOAT3 rotate);
	void SetUvSize(float X, float Y);
	void SetUvSize(DirectX::XMFLOAT2 uvSize);
	void SetUvPos(float X, float Y);
	void SetUvPos(DirectX::XMFLOAT2 uvPos);
	void SetTexture();

	void Setcolor(float R, float G, float B, float A);
	void Setcolor(DirectX::XMFLOAT4 color);

	void SetCenterPosAndRotation(DirectX::XMFLOAT3 StartPos, DirectX::XMFLOAT3 NowPos);

	void SetView(DirectX::XMFLOAT4X4 InView);
	void SetProjection(DirectX::XMFLOAT4X4 InProjection);
private:
	Texture* m_Texture;
	Sprite* m_Sprite;

	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Project;

	DirectX::XMMATRIX T;
	DirectX::XMMATRIX R;
	DirectX::XMMATRIX S;
};

