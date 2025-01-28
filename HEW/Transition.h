#pragma once

#include "SpriteEX.h"
#include "Scene.h"

class CTransition : public CFade
{
public:
	CTransition();
	virtual ~CTransition();
	// フェードイン・フェードアウト共に同じ処理で実装 
	void DrawFadeIn(float alpha) final;
	void DrawFadeOut(float alpha) final;
	void DrawFade(float alpha);

	virtual void Reset() override;
private:
	SpriteEx* m_pTransitionA;
	SpriteEx* m_pTransitionB;


	float StartPosY_A;
	float StartPosY_B;
};
