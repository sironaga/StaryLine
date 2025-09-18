#pragma once
#include "Texture.h"
#include "_StructList.h"

enum class TutorialTextures
{
	Back,
	YesButton,
	NoButton,

	Max
};

class CToTutorial
{
public:
	CToTutorial();
	~CToTutorial();
	void Update();
	void Draw();

private:
	Texture* m_pTexture[(int)TutorialTextures::Max];
	SpriteParam m_tParam[(int)TutorialTextures::Max];

};
