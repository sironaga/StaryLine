#include "Transition.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#include "InputEx.h"
#include "Easing.h"


CTransition::CTransition()
	: m_isTransitionIn(false)
	, m_time(0.0f), m_maxTime(0.0f)
	, StartPosY_A(1000.0f)
	, StartPosY_B(-1000.0f)
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

void CTransition::Update()
{
		StartPosY_A = InEasing(0.0f, 730.0f, 5, 1.0f);
		StartPosY_A = InEasing(0.0f, -730.0f, 5, 1.0f);
}
void CTransition::Draw()
{
	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionA->SetTexture();
	m_pTransitionA->SetPositon(0.0f, StartPosY_A, 0.0f);
	m_pTransitionA->SetSize(1920.0f, -60.0f, 0.0f);
	m_pTransitionA->Disp();

	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionA->SetTexture();
	m_pTransitionA->SetPositon(0.0f, StartPosY_B, 0.0f);
	m_pTransitionA->SetSize(1920.0f, -60.0f, 0.0f);
	m_pTransitionA->Disp();
}

