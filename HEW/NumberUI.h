#pragma once
#include "UI.h"

#define NUMBERSIZE (10)

class CNumberUI : public CUI
{
public:
	// ↓バカきも日本語enum (仮置きです)
	enum nMode
	{
		Right_AL,
		Left_AL,
		Center_AL,
	};


	CNumberUI();
	~CNumberUI();
	void Draw() override;
	void Update() override;
	void SetNumber(int);
	void SetPos(FLOAT3);
	void SetScale(FLOAT3);
	void SetColor(float R, float G, float B, float A);
	void SetLend(int InData);
	void SetNumberColor(int InColor);
	void SetArrangment(nMode InMode);
private:
	SpriteEx* m_pNumber[2][10][10];
	int nNumber;
	int nLong;
	int nColor;
	FLOAT3 Pos;
	FLOAT3 Scale;
	FLOAT3 Color;
	float fAlpha;
	nMode NowMode;
};

