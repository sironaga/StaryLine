#include "Transition.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#include "InputEx.h"
#include "Easing.h"


CTransition::CTransition()
	: StartPosY_A(SCREEN_HEIGHT * 2.0f)
	, StartPosY_B(-SCREEN_HEIGHT)
{
	m_pTransitionA = new SpriteEx("Assets/Texture/Transition/TransitionA.png");
	m_pTransitionB = new SpriteEx("Assets/Texture/Transition/TransitionB.png");

	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionB->SetProjection(Get2DProj());
	m_pTransitionB->SetView(Get2DView());
}

CTransition::~CTransition()
{
	if (m_pTransitionA)
	{
		delete m_pTransitionA;
		m_pTransitionA = nullptr;
	}
	if (m_pTransitionB)
	{
		delete m_pTransitionB;
		m_pTransitionB = nullptr;
	}
}

void CTransition::DrawFadeIn(float alpha)
{
	DrawFade(alpha);
}

void CTransition::DrawFadeOut(float alpha)
{
	DrawFade(alpha);
}

void CTransition::DrawFade(float alpha)
{
	float time = 1 - alpha;
	if (IsFadeIn())
	{
		StartPosY_A = OutEasing(time, 0.0f, SCREEN_HEIGHT * 2.0f, 1.0f);
		StartPosY_B = OutEasing(time, SCREEN_HEIGHT, -SCREEN_HEIGHT * 2.0f, 1.0f);
	}
	else
	{
		StartPosY_A = InEasing(time, SCREEN_HEIGHT * 2.0f, -SCREEN_HEIGHT * 2.0f, 1.0f);
		StartPosY_B = InEasing(time, -SCREEN_HEIGHT, SCREEN_HEIGHT * 2.0f, 1.0f);
	}

	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionA->SetTexture();
	m_pTransitionA->SetPositon(SCREEN_WIDTH / 2.0f, StartPosY_A, 0.0f);
	m_pTransitionA->SetSize(1920.0f, -SCREEN_HEIGHT * 2.0f, 0.0f);
	m_pTransitionA->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pTransitionA->SetUvPos(0.0f, 0.0f);
	m_pTransitionA->SetUvSize(1.0f, 1.0f);
	m_pTransitionA->Disp();

	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionB->SetProjection(Get2DProj());
	m_pTransitionB->SetView(Get2DView());
	m_pTransitionB->SetTexture();
	m_pTransitionB->SetPositon(SCREEN_WIDTH / 2.0f, StartPosY_B, 0.0f);
	m_pTransitionB->SetSize(1920.0f, -SCREEN_HEIGHT * 2.0f, 0.0f);
	m_pTransitionB->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pTransitionB->SetUvPos(0.0f, 0.0f);
	m_pTransitionB->SetUvSize(1.0f, 1.0f);;
	m_pTransitionB->Disp();

	Sprite::ReSetSprite();
}

void CTransition::Reset()
{
	SAFE_DELETE(m_pTransitionA);
	SAFE_DELETE(m_pTransitionB);
	m_pTransitionA = new SpriteEx("Assets/Texture/Transition/TransitionA.png");
	m_pTransitionB = new SpriteEx("Assets/Texture/Transition/TransitionB.png");

	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionB->SetProjection(Get2DProj());
	m_pTransitionB->SetView(Get2DView());
}