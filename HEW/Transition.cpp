#include "Transition.h"
#include "DirectXTex/TextureLoad.h"
#include "Main.h"
#include "InputEx.h"
#include "Easing.h"


CTransition::CTransition()
	: m_isTransitionIn(false)
	, m_time(0.0f), m_maxTime(0.0f)
	, StartPosY_A(SCREEN_HEIGHT * 2.0f)
	, StartPosY_B(-SCREEN_HEIGHT)
{
	m_pTransitionA = new SpriteEx("Assets/Texture/Transition/TransitionA.png");
	m_pTransitionB = new SpriteEx("Assets/Texture/Transition/TransitionB.png");

	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionB->SetProjection(Get2DProj());
	m_pTransitionB->SetView(Get2DView());

	m_fPosA[0] = { -285.0f,SCREEN_HEIGHT / 2.0f,0.0f };
	m_fPosA[1] = { -285.0f,625.0f,0.0f };
	m_fPosA[2] = { -285.0f,625.0f,0.0f };
	m_fPosA[3] = { -285.0f,435,0.0f };
	m_fPosA[4] = { -285.0f,435.0f,0.0f };
	m_fPosA[5] = { -285.0f,340.0f,0.0f };
	m_fPosA[6] = { -285.0f,340.0f,0.0f };
	m_fPosA[7] = { -285.0f,530.0f,0.0f };
	m_fPosA[8] = { -285.0f,530.0f,0.0f };
	m_fPosA[9] = { -290.0f,340.0f,0.0f };
	m_fPosA[10] = { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f };

	m_fPosB[0] = { 285.0f,-SCREEN_HEIGHT / 2.0f,0.0f };
	m_fPosB[1] = { 285.0f,-625.0f,0.0f };
	m_fPosB[2] = { 285.0f,-625.0f,0.0f };
	m_fPosB[3] = { 285.0f,-435,0.0f };
	m_fPosB[4] = { 285.0f,-435.0f,0.0f };
	m_fPosB[5] = { 285.0f,-340.0f,0.0f };
	m_fPosB[6] = { 285.0f,-340.0f,0.0f };
	m_fPosB[7] = { 285.0f,-530.0f,0.0f };
	m_fPosB[8] = { 285.0f,-530.0f,0.0f };
	m_fPosB[9] = { 290.0f,-340.0f,0.0f };
	m_fPosB[10] = { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f };
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
	if (IsKeyPress('L'))m_time = 0.0f;
	if (IsKeyPress('P'))
	{
		if (m_time >= 1.0f) m_time = 1.0f;
		else m_time += 0.02;

		StartPosY_A = InEasing(m_time, SCREEN_HEIGHT * 2.0f, -SCREEN_HEIGHT * 2.0f, 1.0f);
		StartPosY_B = InEasing(m_time, -SCREEN_HEIGHT, SCREEN_HEIGHT * 2.0f, 1.0f);
	}
	if (IsKeyPress('O'))
	{
		if (m_time >= 1.0f) m_time = 1.0f;
		else m_time += 0.02;

		StartPosY_A = OutEasing(m_time, 0.0f, SCREEN_HEIGHT * 2.0f, 1.0f);
		StartPosY_B = OutEasing(m_time, SCREEN_HEIGHT , -SCREEN_HEIGHT * 2.0f, 1.0f);
	}
}
void CTransition::Draw()
{
	if (IsTransitionIn()) DrawTransitionIn(GetAlpha());
	else DrawTransitionOut(GetAlpha());

	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionA->SetProjection(Get2DProj());
	m_pTransitionA->SetView(Get2DView());
	m_pTransitionA->SetTexture();
	m_pTransitionA->SetPositon(SCREEN_WIDTH / 2.0f, StartPosY_A, 0.0f);
	m_pTransitionA->SetSize(1920.0f, -SCREEN_HEIGHT * 2.0f, 0.0f);
	m_pTransitionA->Disp();

	SetRender2D;
	Sprite::ReSetSprite();
	m_pTransitionB->SetProjection(Get2DProj());
	m_pTransitionB->SetView(Get2DView());
	m_pTransitionB->SetTexture();
	m_pTransitionB->SetPositon(SCREEN_WIDTH / 2.0f, StartPosY_B, 0.0f);
	m_pTransitionB->SetSize(1920.0f, -SCREEN_HEIGHT * 2.0f, 0.0f);
	m_pTransitionB->Disp();

	//SetRender2D;
	//Sprite::ReSetSprite();
	//m_pTransitionA->SetProjection(Get2DProj());
	//m_pTransitionA->SetView(Get2DView());
	//m_pTransitionA->SetTexture();
	//m_pTransitionA->SetPositon(0.0f, StartPosY_B, 0.0f);
	//m_pTransitionA->SetSize(1920.0f, -60.0f, 0.0f);
	//m_pTransitionA->Disp();
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

	m_fPosA[0] = { -285.0f,SCREEN_HEIGHT / 2.0f,0.0f };
	m_fPosA[1] = { -285.0f,625.0f,0.0f };
	m_fPosA[2] = { -285.0f,625.0f,0.0f };
	m_fPosA[3] = { -285.0f,435,0.0f };
	m_fPosA[4] = { -285.0f,435.0f,0.0f };
	m_fPosA[5] = { -285.0f,340.0f,0.0f };
	m_fPosA[6] = { -285.0f,340.0f,0.0f };
	m_fPosA[7] = { -285.0f,530.0f,0.0f };
	m_fPosA[8] = { -285.0f,530.0f,0.0f };
	m_fPosA[9] = { -290.0f,340.0f,0.0f };
	m_fPosA[10] = { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f };

	m_fPosB[0] = { 285.0f,-SCREEN_HEIGHT / 2.0f,0.0f };
	m_fPosB[1] = { 285.0f,-625.0f,0.0f };
	m_fPosB[2] = { 285.0f,-625.0f,0.0f };
	m_fPosB[3] = { 285.0f,-435,0.0f };
	m_fPosB[4] = { 285.0f,-435.0f,0.0f };
	m_fPosB[5] = { 285.0f,-340.0f,0.0f };
	m_fPosB[6] = { 285.0f,-340.0f,0.0f };
	m_fPosB[7] = { 285.0f,-530.0f,0.0f };
	m_fPosB[8] = { 285.0f,-530.0f,0.0f };
	m_fPosB[9] = { 290.0f,-340.0f,0.0f };
	m_fPosB[10] = { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f };
}


void CTransition::SetTransition(float time, bool isIn)
{
	m_isTransitionIn = isIn;
	m_time = time;
	m_maxTime = time;
}

float CTransition::GetRate()
{
	if (m_maxTime == 0.0f) return 0.0f; // 0èúéZâÒî 
	return m_time / m_maxTime;
}

float CTransition::GetAlpha()
{
	float alpha = GetRate();
	if (IsTransitionOut()) alpha = 1.0f - alpha;
	return alpha;
}

void CTransition::DrawTransitionIn(float alpha)
{
}

void CTransition::DrawTransitionOut(float alpha)
{
}
