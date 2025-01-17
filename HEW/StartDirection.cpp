#include "StartDirection.h"
#include "Main.h"
#include "Defines.h"

CStartDirection::CStartDirection()
	:m_tPos({ 0.0f, 0.0f, 0.0f })
	, m_tPosTex({ 0.0f, 0.0f})
	, m_tRotate({ 0.0f, 0.0f, 0.0f })
	, m_tSize({ 1.0f,1.0f, 1.0f })
	, m_tSizeTex({ 1.0f,1.0f })
	,m_timer(0.0f)
,m_NowDraw(false)
{
	//m_StartNumberUI = new CNumberUI();
	m_StratSprite = new SpriteEx(TEX_PASS("Fever_Star/Red_Fever_Star.png"));
}

CStartDirection::~CStartDirection()
{
	//delete m_StartNumberUI;
	delete m_StratSprite;
}

void CStartDirection::Update()
{
	if (m_timer > 0.0f) {  // 毎フレーム判定 
		m_timer -= 1.0f / 60.0f;
		if (m_timer < 0.0f)  // フェードが終了すると呼び出されなくなるので、少しだけ効率がよい 
			m_timer = 0.0f;
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
		m_StratSprite->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_StratSprite->SetRotation(m_tRotate.X, m_tRotate.Y, m_tRotate.Z);
		m_StratSprite->SetTexture();
		m_StratSprite->SetView(Get2DView());
		m_StratSprite->SetProjection(GetProj());
		m_StratSprite->SetPositon(m_tPos.X, m_tPos.Y, m_tPos.Z);
		m_StratSprite->SetSize(m_tSize.X, m_tSize.Y, m_tSize.Z);
		m_StratSprite->Disp();
	}
}

void CStartDirection::SetTimer(float InTime)
{
	m_timer = InTime;
	m_NowDraw = true;
}

bool CStartDirection::GetDraw()
{
	return m_NowDraw;
}
