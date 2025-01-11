#pragma once
#include "UI.h"

#define SIZE (10)

class CNumberUI : public CUI
{
public:
	CNumberUI();
	~CNumberUI();
	void Draw() override;
	void Update() override;
	void SetNumber(int);
	void SetPos(FLOAT3);
	void SetScale(FLOAT3);
	void SetColor(float R, float G, float B, float A);
private:
	SpriteEx* m_pNumber[2][10];
	int nNumber;
	int nLong;
	FLOAT3 Pos;
	FLOAT3 Scale;
	FLOAT3 color;
	float fAlpha;

};

