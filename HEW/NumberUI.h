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
	void SetLen(int nInData);
private:
	SpriteEx* m_pNumber[2][10];
	int nNumber;
	int nLong;
	FLOAT3 Pos;
	FLOAT3 Scale;

};

