#ifndef __UI_H__
#define __UI_H__
#define _DIRECTX_

#include "Texture.h"
#include "Sprite.h"


class CUI
{
public:
	CUI();
	~CUI();
	void Update();
	void Draw();

	enum UI
	{
		CIRCLE,
		TRIANGLE,
		SQUARE,
		CROSS,
		UP,
		DOWN,
		RIGHT,
		LEFT,
		MAX_UI
	};

private:
	Texture* m_pTex;
	float m_rate;
};

#endif