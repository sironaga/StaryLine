#pragma once

#include "SpriteEX.h"


class CTransition
{
public:
	CTransition();
	virtual ~CTransition();
	void Update();
	void Draw();

	void Reset();
	void Start(bool isIn);
	void SetTransition(float time, bool isIn);
	bool IsTransition() { return m_time <= 0.0f; }
	bool IsTransitionIn() { return m_isTransitionIn; }
	bool IsTransitionOut() { return !m_isTransitionIn; }

	float GetRate();
	float GetAlpha();
protected:
	virtual void DrawTransitionIn(float alpha);
	virtual void DrawTransitionOut(float alpha);

private:
	SpriteEx* m_pTransitionA;
	SpriteEx* m_pTransitionB;

	bool m_isTransitionIn;
	float m_time;   
	float m_maxTime;

	FLOAT3 m_fPosA[11];
	FLOAT3 m_fPosB[11];

	float StartPosY_A;
	float StartPosY_B;
};
