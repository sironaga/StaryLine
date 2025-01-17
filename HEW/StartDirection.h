#pragma once
#include "SpriteEx.h"
#include "Main.h"
#include "NumberUI.h"


class CStartDirection
{
public:
	CStartDirection();
	~CStartDirection();
	void Update();
	void Draw();

	//‰‰o‚ÌoŒ»ŠÔ
	void SetTimer(float InTime);
	//Œ»İ‚ÌDrawó‘Ô
	bool GetDraw();
private:
	FLOAT3 m_tPos;
	FLOAT3 m_tSize;
	FLOAT3 m_tRotate;
	FLOAT2 m_tPosTex;
	FLOAT2 m_tSizeTex;
	float m_timer;
	bool m_NowDraw;
	CNumberUI* m_StartNumberUI;
	SpriteEx* m_StratSprite;
};

