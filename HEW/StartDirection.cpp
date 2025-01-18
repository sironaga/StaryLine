#include "StartDirection.h"
#include "Main.h"
#include "Defines.h"
#include "Easing.h"
//460
float  g_CountTimer=0;
float  g_OldCountTimer = 0;
CStartDirection::CStartDirection()
	: m_tPosTex({ 0.0f, 0.0f })
	, m_tSizeTex({ 1.0f,1.0f })
	, m_timer(0.0f)
	, m_NowDraw(false)
	, m_startDraw(false)
{
	//m_StartNumberUI = new CNumberUI();
	m_tPos[0] = { 960.0f, 540.0f, 0.0f };
	m_tSize[0] = { 0.3f,0.07f, 0.0f };
	m_StratSprite[0] = new SpriteEx(TEX_PASS("Start/Battle_Start.png"));
	m_StratSprite[1] = new SpriteEx(TEX_PASS("Start/Battle_Start_B.png"));
	m_StratSprite[2] = new SpriteEx(TEX_PASS("Start/Battle_Start_A.png"));
	m_StratSprite[3] = new SpriteEx(TEX_PASS("Start/Battle_Start_T.png"));
	m_StratSprite[4] = new SpriteEx(TEX_PASS("Start/Battle_Start_T.png"));
	m_StratSprite[5] = new SpriteEx(TEX_PASS("Start/Battle_Start_L.png"));
	m_StratSprite[6] = new SpriteEx(TEX_PASS("Start/Battle_Start_E.png"));

	m_tPos[1] = { 720.0f, -50.0f, 0.0f };
	m_tPos[2] = { 820.0f, -50.0f, 0.0f };
	m_tPos[3] = { 920.0f, -50.0f, 0.0f };
	m_tPos[4] = { 1020.0f,-50.0f, 0.0f };
	m_tPos[5] = { 1120.0f,-50.0f, 0.0f };
	m_tPos[6] = { 1220.0f,-50.0f, 0.0f };
	m_tSize[1] = { 0.065f,0.065f, 1.0f };
	m_tSize[2] = { 0.065f,0.065f, 1.0f };
	m_tSize[3] = { 0.065f,0.065f, 1.0f };
	m_tSize[4] = { 0.065f,0.065f, 1.0f };
	m_tSize[5] = { 0.065f,0.065f, 1.0f };
	m_tSize[6] = { 0.065f,0.065f, 1.0f };
}

CStartDirection::~CStartDirection()
{
	//delete m_StartNumberUI;
	for (int i = 0; i < 7; i++)
	{
		SAFE_DELETE(m_StratSprite[i]);
	}
	
}

void CStartDirection::Update()
{
	static int i = 1;
	if (m_timer > 0.0f) {  // –ˆƒtƒŒ[ƒ€”»’è 
		m_timer -= 1.0f / 60.0f;
		if (g_CountTimer > 0)
		{
			g_CountTimer -= 1.0f / 60.0f;
			if (g_CountTimer < 0.0f)
			{
				if (i < 6)
				{
				g_CountTimer = g_OldCountTimer;
					i += 1;
				}
				else if(!m_startDraw )
				{
					i += 1;
					g_CountTimer = g_OldCountTimer;
					m_startDraw = true;
				}
			}
		}
		if (m_startDraw)
		{
			m_tSize[0].X = OutEasing(g_CountTimer, 0.0f, -0.3f, 1.0f)-(1.0f-0.3f);
			m_tSize[0].Y = OutEasing(g_CountTimer, 0.0f, -0.07f, 1.0f) - (1.0f - 0.07f);
		}
		if (m_timer < 0.0f)
		{
			m_timer = 0.0f;
			i = 1;
		}
		if (i < 7)
		{
			m_tPos[i].Y = OutEasing(g_CountTimer, 0.0f, -600.0f, 1.0f) + 470.0f;
		}
		
		//m_StartNumberUI->Update();
	}
	else
	{
		m_NowDraw = false;
	}
	
}

void CStartDirection::Draw()
{
	if (m_NowDraw)
	{
		//SetRender2D();
		/*m_StartNumberUI->SetNumber((int)m_timer);
		m_StartNumberUI->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_StartNumberUI->SetPos(m_tPos);
		m_StartNumberUI->SetScale(m_tSize);
		m_StartNumberUI->Draw();*/
		for (int i = 1; i < 7; i++)
		{
			m_StratSprite[i]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
			m_StratSprite[i]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_StratSprite[i]->SetTexture();
			m_StratSprite[i]->SetView(Get2DView());
			m_StratSprite[i]->SetProjection(Get2DProj());
			m_StratSprite[i]->SetPositon(m_tPos[i].X, m_tPos[i].Y, m_tPos[i].Z);
			m_StratSprite[i]->SetSize(m_tSize[i].X, m_tSize[i].Y, m_tSize[i].Z);
			m_StratSprite[i]->Disp();
		}
		if (m_startDraw)
		{
			m_StratSprite[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
			m_StratSprite[0]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
			m_StratSprite[0]->SetTexture();
			m_StratSprite[0]->SetView(Get2DView());
			m_StratSprite[0]->SetProjection(Get2DProj());
			m_StratSprite[0]->SetPositon(m_tPos[0].X, m_tPos[0].Y, m_tPos[0].Z);
			m_StratSprite[0]->SetSize(m_tSize[0].X, m_tSize[0].Y, m_tSize[0].Z);
			m_StratSprite[0]->Disp();
		}
	}
}

void CStartDirection::SetTimer(float InTime)
{
	m_timer = InTime;
	g_CountTimer = InTime/8.0f;
	g_OldCountTimer = InTime / 8.0f;
	m_NowDraw = true;
	m_startDraw = false;
	m_tPos[1] = { 720.0f, -50.0f, 0.0f };
	m_tPos[2] = { 820.0f, -50.0f, 0.0f };
	m_tPos[3] = { 920.0f, -50.0f, 0.0f };
	m_tPos[4] = { 1020.0f,-50.0f, 0.0f };
	m_tPos[5] = { 1120.0f,-50.0f, 0.0f };
	m_tPos[6] = { 1220.0f,-50.0f, 0.0f };
}

bool CStartDirection::GetDraw()
{
	return m_NowDraw;
}
