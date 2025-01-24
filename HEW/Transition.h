#pragma once

#include "SpriteEX.h"


class CTransition
{
public:
	CTransition();
	virtual ~CTransition();
	void Update();
	void Draw();

protected:

private:
	SpriteEx* m_pTransitionA;
	SpriteEx* m_pTransitionB;

	bool m_isTransitionIn;
	float m_time;   
	float m_maxTime;
	float StartPosY_A;
	float StartPosY_B;
};
