#pragma once
#include "Texture.h"
#include "_StructList.h"

enum class TutorialTextures
{
	Back,
	YesButton,
	NoButton,
	Select,

	Max
};

enum class ScreenMove
{
	None,
	ScreenUp,
	ScreenDown,
};

class CToTutorial
{
public:
	CToTutorial();
	~CToTutorial();
	void Update();
	void Draw();
	void SetMove(ScreenMove inMove);
	ScreenMove GetMove();
	bool TutorialInput();
	bool IsPlayTutorial();

private:
	Texture* m_pTexture[(int)TutorialTextures::Max];
	SpriteParam m_tParam[(int)TutorialTextures::Max];
	ScreenMove m_eMove;
	float m_fTime;
	float m_fSizeTime;
	int m_nSelect;
};
