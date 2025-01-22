#include "Pause.h"
#include "Input.h"
#include "Main.h"
#include "Defines.h"



CPause::CPause()
	:m_bPause(false)
,m_bOption(false)
,m_bRetry(false)
,m_bSelect(false)
,m_bReturn(false)
{
	//テクスチャの読み込み
	m_pPauseTex[0] = new SpriteEx(TEX_PASS("Pause/Pause_.png"));
	m_pPauseTex[1] = new SpriteEx(TEX_PASS("Pause/Pause_Option.png"));
	m_pPauseTex[2] = new SpriteEx(TEX_PASS("Pause/Pause_Option_Push.png"));
	m_pPauseTex[3] = new SpriteEx(TEX_PASS("Pause/Pause_Retry.png"));
	m_pPauseTex[4] = new SpriteEx(TEX_PASS("Pause/Pause_Retry_Push.png"));
	m_pPauseTex[5] = new SpriteEx(TEX_PASS("Pause/Pause_Return.png"));
	m_pPauseTex[6] = new SpriteEx(TEX_PASS("Pause/Pause_Return_Push.png"));
	m_pPauseTex[7] = new SpriteEx(TEX_PASS("Pause/Pause_Stageselect.png"));
	m_pPauseTex[8] = new SpriteEx(TEX_PASS("Pause/Pause_Stageselect_Push.png"));
	m_pPauseTex[9] = new SpriteEx(TEX_PASS("Pause/Pause_Selected.png"));
	
	//ポジションの初期化
	m_fPos[0] ={285.0f,SCREEN_HEIGHT/2.0f,0.0f};
	m_fPos[1] ={ 285.0f,625.0f,0.0f };
	m_fPos[2] = { 285.0f,625.0f,0.0f };
	m_fPos[3] = {285.0f,435,0.0f };
	m_fPos[4] = { 285.0f,435.0f,0.0f };
	m_fPos[5] = {285.0f,340.0f,0.0f };
	m_fPos[6] = { 285.0f,340.0f,0.0f };
	m_fPos[7] = {285.0f,530.0f,0.0f};
	m_fPos[8] ={ 285.0f,530.0f,0.0f};
	m_fPos[9] = { 290.0f,340.0f,0.0f };

	//サイズの初期化
	m_fSize[0] = { 0.3f,1.0f,0.0f };
	m_fSize[1] = { 0.3f,0.07f,0.0f };
	m_fSize[2] = { 0.3f,0.07f,0.0f };
	m_fSize[3] = { 0.3f,0.07f,0.0f };
	m_fSize[4] = { 0.3f,0.07f,0.0f };
	m_fSize[5] = { 0.3f,0.07f,0.0f };
	m_fSize[6] = { 0.3f,0.07f,0.0f };
	m_fSize[7] = { 0.3f,0.07f,0.0f };
	m_fSize[8] = { 0.3f,0.07f,0.0f };
	m_fSize[9] = { 0.304f,0.087f,0.0f };

}

CPause::~CPause()
{
	for (int i = 0; i < 10; i++)
	{
		if (m_pPauseTex[i]) SAFE_DELETE(m_pPauseTex[i]);
	}
	if (m_pOption) m_pOption = nullptr;
}

void CPause::Update()
{
	static int section = SEC_RETURN;//選択中のバーの場所
	//ポーズボタンを押したか
	if (IsKeyTrigger(VK_ESCAPE))
	{
		m_bPause = true;
	}
	if (!m_bPause) return;

	//バー選択中
	if (!m_bOption && !m_bRetry && !m_bReturn && !m_bSelect)
	{
		if (IsKeyTrigger(VK_UP) || IsKeyTrigger('W'))
		{
			switch (section)
			{
			case SEC_OPTION:
				section = SEC_STAGESELECT;
				break;
			case SEC_RETRY:
				section = SEC_RETURN;
				break;
			case SEC_RETURN:
				section = SEC_RETURN;
				break;
			case SEC_STAGESELECT:
				section = SEC_RETRY;
				break;
			default:
				break;
			}
		}
		if (IsKeyTrigger(VK_DOWN) || IsKeyTrigger('S'))
		{
			switch (section)
			{
			case SEC_OPTION:
				section = SEC_OPTION;
				break;
			case SEC_RETRY:
				section = SEC_STAGESELECT;
				break;
			case SEC_RETURN:
				section = SEC_RETRY;
				break;
			case SEC_STAGESELECT:
				section = SEC_OPTION;
				break;
			default:
				break;
			}
		}
		switch (section)
		{
		case SEC_OPTION:
			m_fPos[9] = m_fPos[1];
			break;
		case SEC_RETRY:
			m_fPos[9] = m_fPos[3];
			break;
		case SEC_RETURN:
			m_fPos[9] = m_fPos[5];
			break;
		case SEC_STAGESELECT:
			m_fPos[9] = m_fPos[7];
			break;
		default:
			break;
		}
		//決定ボタン
		if (IsKeyTrigger(VK_RETURN))
		{
			switch (section)
			{
			case SEC_OPTION:
				m_bOption = true;
				break;
			case SEC_RETRY:
				m_bRetry = true;
				break;
			case SEC_RETURN:
				m_bReturn = true;
				break;
			case SEC_STAGESELECT:
				m_bSelect = true;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		switch (section)
		{
		case SEC_OPTION:
			m_pOption->Update();
			break;
		case SEC_RETRY:
			break;
		case SEC_RETURN:
			break;
		case SEC_STAGESELECT:
			break;
		default:
			break;
		}
	}
}

void CPause::Draw()
{
	//ポーズ
	m_pPauseTex[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pPauseTex[0]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pPauseTex[0]->SetTexture();
	m_pPauseTex[0]->SetView(Get2DView());
	m_pPauseTex[0]->SetProjection(Get2DProj());
	m_pPauseTex[0]->SetPositon(m_fPos[0].X, m_fPos[0].Y, m_fPos[0].Z);
	m_pPauseTex[0]->SetSize(m_fSize[0].X, m_fSize[0].Y, m_fSize[0].Z);
	m_pPauseTex[0]->Disp();
	//オプションバー
	if (!m_bOption)
	{
		m_pPauseTex[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[1]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[1]->SetTexture();
		m_pPauseTex[1]->SetView(Get2DView());
		m_pPauseTex[1]->SetProjection(Get2DProj());
		m_pPauseTex[1]->SetPositon(m_fPos[1].X, m_fPos[1].Y, m_fPos[1].Z);
		m_pPauseTex[1]->SetSize(m_fSize[1].X, m_fSize[1].Y, m_fSize[1].Z);
		m_pPauseTex[1]->Disp();
	}
	else
	{
		m_pPauseTex[2]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[2]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[2]->SetTexture();
		m_pPauseTex[2]->SetView(Get2DView());
		m_pPauseTex[2]->SetProjection(Get2DProj());
		m_pPauseTex[2]->SetPositon(m_fPos[2].X, m_fPos[2].Y, m_fPos[2].Z);
		m_pPauseTex[2]->SetSize(m_fSize[2].X, m_fSize[2].Y, m_fSize[2].Z);
		m_pPauseTex[2]->Disp();
		m_pOption->Draw();
	}
	//リトライバー
	if (!m_bRetry)
	{
		m_pPauseTex[3]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[3]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[3]->SetTexture();
		m_pPauseTex[3]->SetView(Get2DView());
		m_pPauseTex[3]->SetProjection(Get2DProj());
		m_pPauseTex[3]->SetPositon(m_fPos[3].X, m_fPos[3].Y, m_fPos[3].Z);
		m_pPauseTex[3]->SetSize(m_fSize[3].X, m_fSize[3].Y, m_fSize[3].Z);
		m_pPauseTex[3]->Disp();
	}
	else
	{
		m_pPauseTex[4]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[4]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[4]->SetTexture();
		m_pPauseTex[4]->SetView(Get2DView());
		m_pPauseTex[4]->SetProjection(Get2DProj());
		m_pPauseTex[4]->SetPositon(m_fPos[4].X, m_fPos[4].Y, m_fPos[4].Z);
		m_pPauseTex[4]->SetSize(m_fSize[4].X, m_fSize[4].Y, m_fSize[4].Z);
		m_pPauseTex[4]->Disp();
	}
	//ゲームに戻るバー
	if (!m_bReturn)
	{
		m_pPauseTex[5]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[5]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[5]->SetTexture();
		m_pPauseTex[5]->SetView(Get2DView());
		m_pPauseTex[5]->SetProjection(Get2DProj());
		m_pPauseTex[5]->SetPositon(m_fPos[5].X, m_fPos[5].Y, m_fPos[5].Z);
		m_pPauseTex[5]->SetSize(m_fSize[5].X, m_fSize[5].Y, m_fSize[5].Z);
		m_pPauseTex[5]->Disp();
	}
	else
	{
		m_pPauseTex[6]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[6]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[6]->SetTexture();
		m_pPauseTex[6]->SetView(Get2DView());
		m_pPauseTex[6]->SetProjection(Get2DProj());
		m_pPauseTex[6]->SetPositon(m_fPos[6].X, m_fPos[6].Y, m_fPos[6].Z);
		m_pPauseTex[6]->SetSize(m_fSize[6].X, m_fSize[6].Y, m_fSize[6].Z);
		m_pPauseTex[6]->Disp();
	}
	//ステータス選択バー
	if (!m_bSelect)
	{
		m_pPauseTex[7]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[7]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[7]->SetTexture();
		m_pPauseTex[7]->SetView(Get2DView());
		m_pPauseTex[7]->SetProjection(Get2DProj());
		m_pPauseTex[7]->SetPositon(m_fPos[7].X, m_fPos[7].Y, m_fPos[7].Z);
		m_pPauseTex[7]->SetSize(m_fSize[7].X, m_fSize[7].Y, m_fSize[7].Z);
		m_pPauseTex[7]->Disp();
	}
	else
	{
		m_pPauseTex[8]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPauseTex[8]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pPauseTex[8]->SetTexture();
		m_pPauseTex[8]->SetView(Get2DView());
		m_pPauseTex[8]->SetProjection(Get2DProj());
		m_pPauseTex[8]->SetPositon(m_fPos[8].X, m_fPos[8].Y, m_fPos[8].Z);
		m_pPauseTex[8]->SetSize(m_fSize[8].X, m_fSize[8].Y, m_fSize[8].Z);
		m_pPauseTex[8]->Disp();
	}
	//選択枠
	m_pPauseTex[9]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pPauseTex[9]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pPauseTex[9]->SetTexture();
	m_pPauseTex[9]->SetView(Get2DView());
	m_pPauseTex[9]->SetProjection(Get2DProj());
	m_pPauseTex[9]->SetPositon(m_fPos[9].X, m_fPos[9].Y, m_fPos[9].Z);
	m_pPauseTex[9]->SetSize(m_fSize[9].X, m_fSize[9].Y, m_fSize[9].Z);
	m_pPauseTex[9]->Disp();
}

void CPause::SetOption(COption* InOption)
{
	m_pOption = InOption;
}

bool CPause::GetPause()
{
	return m_bPause;
}

bool CPause::GetOption()
{
	return m_bOption;
}

bool CPause::GetRetry()
{
	return m_bRetry;
}

bool CPause::GetSelect()
{
	return m_bSelect;
}

bool CPause::GetReturn()
{
	return m_bReturn;
}
