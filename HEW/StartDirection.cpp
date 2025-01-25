#include "StartDirection.h"
#include "Main.h"
#include "Defines.h"
#include "Easing.h"
//460

CStartDirection::CStartDirection()
	:  m_timer(0.0f)
	, m_NowDraw(false)
	, m_startDraw(false)
{
	//テクスチャーの読み込み
	m_StratSprite[0] = new SpriteEx(TEX_PASS("Start/Battle_Start.png"));
	m_StratSprite[1] = new SpriteEx(TEX_PASS("Start/Battle_Start_B.png"));
	m_StratSprite[2] = new SpriteEx(TEX_PASS("Start/Battle_Start_A.png"));
	m_StratSprite[3] = new SpriteEx(TEX_PASS("Start/Battle_Start_T.png"));
	m_StratSprite[4] = new SpriteEx(TEX_PASS("Start/Battle_Start_T.png"));
	m_StratSprite[5] = new SpriteEx(TEX_PASS("Start/Battle_Start_L.png"));
	m_StratSprite[6] = new SpriteEx(TEX_PASS("Start/Battle_Start_E.png"));

	//サイズとポジションの初期化
	m_tPos[0] = { 960.0f, 500.0f, 0.0f };
	m_tSize[0] = { 0.3f,0.09f, 0.0f };
	m_tPos[1] = { 720.0f, -50.0f, 0.0f };
	m_tPos[2] = { 820.0f, -50.0f, 0.0f };
	m_tPos[3] = { 910.0f, -50.0f, 0.0f };
	m_tPos[4] = { 1020.0f,-50.0f, 0.0f };
	m_tPos[5] = { 1120.0f,-50.0f, 0.0f };
	m_tPos[6] = { 1210.0f,-50.0f, 0.0f };
	m_tSize[1] = { 0.07f,0.1f, 1.0f };
	m_tSize[2] = { 0.07f,0.1f, 1.0f };
	m_tSize[3] = { 0.07f,0.1f, 1.0f };
	m_tSize[4] = { 0.07f,0.1f, 1.0f };
	m_tSize[5] = { 0.07f,0.1f, 1.0f };
	m_tSize[6] = { 0.07f,0.1f, 1.0f };
	//出現時間の初期化
	for (int i = 0; i < 7; i++)
	{
		m_CountTimer[i] = 0.0f;
		
	}
	m_OldCountTimer[0]=0.0f;
	m_OldCountTimer[1] = 0.0f;
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
	// 毎フレーム判定 
	if (m_timer > 0.0f) {  
		m_timer -= 1.0f / 60.0f;
		if (m_CountTimer[6] > 0)
		{
			//B
			if (m_CountTimer[1] >= 0)
			{
				m_CountTimer[1] -= 1.0f / 60.0f;
				//A
				if (m_CountTimer[1] < (m_OldCountTimer[0]/3.0f)*2.0f)
				{
					m_CountTimer[2] -= 1.0f / 60.0f;
					//T
					if (m_CountTimer[2] < (m_OldCountTimer[0] / 3.0f) * 2.0f)
					{
						m_CountTimer[3] -= 1.0f / 60.0f;
						//T
						if (m_CountTimer[3] < (m_OldCountTimer[0] / 3.0f) * 2.0f)
						{
							m_CountTimer[4] -= 1.0f / 60.0f;
							//L
							if (m_CountTimer[4] < (m_OldCountTimer[0] / 3.0f) * 2.0f)
							{
								m_CountTimer[5] -= 1.0f / 60.0f;
								//E
								if (m_CountTimer[5] < (m_OldCountTimer[0] / 3.0f) * 2.0f)
								{
									m_CountTimer[6] -= 1.0f / 60.0f;

									if (m_CountTimer[6] < 0.0f)
									{
										m_CountTimer[6] = 0.0f;
										if(!m_startDraw) m_startDraw = true;
									}
								}
								if (m_CountTimer[5] < 0.0f)
								{
									m_CountTimer[5] = 0.0f;
								}
							}
							if (m_CountTimer[4] < 0.0f)
							{
								m_CountTimer[4] = 0.0f;
							}
						}
						if (m_CountTimer[3] < 0.0f)
						{
							m_CountTimer[3] = 0.0f;
						}
					}
					if (m_CountTimer[2] < 0.0f)
					{
						m_CountTimer[2] = 0.0f;
					}
				}
				if (m_CountTimer[1] < 0.0f)
				{
					m_CountTimer[1] = 0.0f;
				}
			}
			
		}

		//ポジションの更新
		for (int i = 1; i < 7; i++)
		{
			m_tPos[i].Y = OutEasing(m_CountTimer[i], 0.0f, -670.0f, 1.0f) + 370.0f;
		}
		if (m_startDraw)
		{
			if (m_CountTimer[0] > 0.0f)
			{
				m_CountTimer[0] -= 1.0f / 60.0f;
				if (m_CountTimer[0] < 0.0f)
				{
					m_CountTimer[0] = 0.0f;
				}
			}
			if (m_CountTimer[0] < m_OldCountTimer[1] * (3.0f / 4.0f))
			{
				m_tSize[0].X = OutEasing(m_CountTimer[0], 0.0f, -0.42f, 1.0f) - (1.0f - 0.42f);
				m_tSize[0].Y = OutEasing(m_CountTimer[0], 0.0f, -0.16f, 1.0f) - (1.0f - 0.16f);
			}
			
		}
		if (m_timer < 0.0f)
		{
			m_timer = 0.0f;
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
		//BATTLE
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
		//START
		if (m_startDraw&& m_CountTimer[0] < m_OldCountTimer[1] * (3.0f / 4.0f))
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
	m_CountTimer[0] = (InTime * 0.2f)*(3.0f / 4.0f);
	m_OldCountTimer[1] = (InTime * 0.2f) * (3.0f / 4.0f);
	for (int i = 1; i < 7; i++)
	{
		m_CountTimer[i] = (InTime * 0.6f) * 0.375f;
		
	}
	m_OldCountTimer[0] = (InTime * 0.6f) * 0.375f;
	m_NowDraw = true;
	m_startDraw = false;
	m_tPos[1] = { 720.0f, -50.0f, 0.0f };
	m_tPos[2] = { 820.0f, -50.0f, 0.0f };
	m_tPos[3] = { 910.0f, -50.0f, 0.0f };
	m_tPos[4] = { 1020.0f,-50.0f, 0.0f };
	m_tPos[5] = { 1120.0f,-50.0f, 0.0f };
	m_tPos[6] = { 1210.0f,-50.0f, 0.0f };
}

bool CStartDirection::GetDraw()
{
	return m_NowDraw;
}
