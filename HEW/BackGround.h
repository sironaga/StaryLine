#pragma once

#include "SpriteEx.h"
#include "Main.h"

class CBackGround
{
public:
	CBackGround();
	~CBackGround();
	void Update();
	void Draw();
private:
	SpriteEx* m_pBGSprite;
	E_SCENE_TYPE m_eCurrentScene;
};