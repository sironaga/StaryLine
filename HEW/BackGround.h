#pragma once

#include "Texture.h"
#include "Main.h"

class CBackGround
{
public:
	CBackGround();
	~CBackGround();
	void Update();
	void Draw();
private:
	Texture* m_pBGSprite;
	E_SCENE_TYPE m_eCurrentScene;
	DirectX::XMFLOAT2 m_tPos;
	DirectX::XMFLOAT2 m_tSize;
	DirectX::XMFLOAT3 m_tRotate;
	DirectX::XMFLOAT2 m_tPosTex;
	DirectX::XMFLOAT2 m_tSizeTex;
};