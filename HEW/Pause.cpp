#include "Pause.h"
#include "Input.h"
#include "Main.h"
#include "Defines.h"
#include "Easing.h"
#include "InputEx.h"

SpriteEx* CPause::m_pPauseTex[11];//texture情報
COption* CPause::m_pOption;//オプション
CSoundList* CPause::m_pSoundPause[3];//音データ
IXAudio2SourceVoice* CPause::m_pSoundPauseSE[3];//スピーカー
bool CPause::m_bPause;//ポーズ画面判定
bool CPause::m_bOption;//オプション画面判定
bool CPause::m_bRetry;//リトライ判定
bool CPause::m_bSelect;//ステータス選択判定
bool CPause::m_bReturn;//ゲームに戻る判定
bool CPause::m_bClose;//画面を閉じる

FLOAT3 CPause::m_fPos[11];//ポジション
FLOAT3 CPause::m_fSize[11];//サイズ
float CPause::m_ftime;//計算用

void CPause::Init()
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
	m_pPauseTex[10] = new SpriteEx(TEX_PASS("Pause/kuro.png"));


	//ポジションの初期化
	m_fPos[0] = { -285.0f,SCREEN_HEIGHT / 2.0f,0.0f };
	m_fPos[1] = { -285.0f,625.0f,0.0f };
	m_fPos[2] = { -285.0f,625.0f,0.0f };
	m_fPos[3] = { -285.0f,435,0.0f };
	m_fPos[4] = { -285.0f,435.0f,0.0f };
	m_fPos[5] = { -285.0f,340.0f,0.0f };
	m_fPos[6] = { -285.0f,340.0f,0.0f };
	m_fPos[7] = { -285.0f,530.0f,0.0f };
	m_fPos[8] = { -285.0f,530.0f,0.0f };
	m_fPos[9] = { -290.0f,340.0f,0.0f };
	m_fPos[10] = { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f };

	//サイズの初期化
	m_fSize[0] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT, 0.0f };
	m_fSize[1] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[2] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[3] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[4] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[5] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[6] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[7] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[8] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.07f,0.0f };
	m_fSize[9] = { SCREEN_WIDTH * 0.304f,SCREEN_HEIGHT * 0.087f,0.0f };
	m_fSize[10] = { SCREEN_WIDTH ,SCREEN_HEIGHT ,0.0f };


	m_pSoundPause[0] = new CSoundList(SE_CANCEL);
	m_pSoundPause[1] = new CSoundList(SE_DECISION);
	m_pSoundPause[2] = new CSoundList(SE_SELECT);
	for (int i = 0; i < 3; i++)
	{
		m_pSoundPause[i]->SetMasterVolume();
		m_pSoundPauseSE[i] = m_pSoundPause[i]->GetSound(false);
	}

	m_bPause = false;
	m_bOption = false;
	m_bRetry = false;
	m_bSelect = false;
	m_bReturn = false;
	m_pOption = nullptr;
	m_ftime = 0.0f;
	m_bClose = false;
}

void CPause::Uninit()
{
	for (int i = 0; i < 11; i++)
	{
		if (m_pPauseTex[i]) SAFE_DELETE(m_pPauseTex[i]);
	}
	if (m_pOption)
	{
		m_pOption = nullptr;
	}
}

void CPause::Update()
{
	static float t = 0.0f;
	static int section = SEC_RETURN;//選択中のバーの場所
	XAUDIO2_BUFFER buffer;
	//ポーズボタンを押したか
	if (!m_bPause && WithGetKeyTriger(XINPUT_GAMEPAD_START, VK_ESCAPE))
	{
		m_bPause = true;
		m_ftime = 0.1f;
		t = 0.0f;
	}
	if (!m_bPause) return;

	//ポーズ中かつcloseが呼び出されていない場合
	if (m_bPause && !m_bClose)
	{
		
		// 毎フレーム判定 
		if (m_ftime > 0.0f) {
			m_ftime -= 1.0f / 60.0f;
			t += 1.0f / 60.0f;
			if (m_ftime < 0.0f)
			{
				m_ftime = 0.0f;
				t = 0.1f;
			}
		}
		//ポーズ画面の開く処理
			for (int i = 0; i < 9; i++)
			{
				m_fPos[i].X = OutEasing(t, 0.0f, 285.0f+ 2800.0f, 1.0f)-300.0f;
			}
			m_fPos[9].X =	 OutEasing(t, 0.0f, 290.0f+ 2800.0f, 1.0f) - 300.0f;

	}
	//バー選択
	SetAllMasterVolume(m_pOption->GetMasterVoluem());
	SetAllVolumeBGM(m_pOption->GetBGMVoluem());
	SetAllVolumeSE(m_pOption->GetSEVoluem());
	if (!m_bOption && !m_bRetry && !m_bReturn && !m_bSelect)
	{
		if (GetControllerLStickTriggerForeDirection()== XINPUT_GAMEPAD_DPAD_UP|| WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_UP, VK_UP) || IsKeyTrigger('W'))
		{
			
			
			switch (section)
			{
			case SEC_OPTION:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
				section = SEC_STAGESELECT;
				break;
			case SEC_RETRY:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
				section = SEC_RETURN;
				break;
			case SEC_RETURN:
				section = SEC_RETURN;
				break;
			case SEC_STAGESELECT:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
				section = SEC_RETRY;
				break;
			default:
				break;
			}
		}
		if (GetControllerLStickTriggerForeDirection()==XINPUT_GAMEPAD_DPAD_DOWN||WithGetKeyTriger(XINPUT_GAMEPAD_DPAD_DOWN,VK_DOWN) || IsKeyTrigger('S'))
		{
			
			switch (section)
			{
			case SEC_OPTION:
				section = SEC_OPTION;
				break;
			case SEC_RETRY:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
				section = SEC_STAGESELECT;
				break;
			case SEC_RETURN:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
				section = SEC_RETRY;
				break;
			case SEC_STAGESELECT:
				//se
				m_pSoundPauseSE[2]->Stop();
				m_pSoundPauseSE[2]->FlushSourceBuffers();
				buffer = m_pSoundPause[2]->GetBuffer(false);
				m_pSoundPauseSE[2]->SubmitSourceBuffer(&buffer);
				SetVolumeSE(m_pSoundPauseSE[2]);
				m_pSoundPauseSE[2]->Start();
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
		if (WithGetKeyTriger(COption::GetTypeAB(COption::GetControllerSetting(), XINPUT_GAMEPAD_A), VK_RETURN))
		{
			//se
			m_pSoundPauseSE[1]->Stop();
			m_pSoundPauseSE[1]->FlushSourceBuffers();
			buffer = m_pSoundPause[1]->GetBuffer(false);
			m_pSoundPauseSE[1]->SubmitSourceBuffer(&buffer);
			SetVolumeSE(m_pSoundPauseSE[1]);
			m_pSoundPauseSE[1]->Start();
			switch (section)
			{
			case SEC_OPTION:
				m_pOption->SetOption();
				m_pOption->ResetPos();
				m_pOption->ResetSize();
				//m_pOption->SetMulSize(0.8f);
				//m_pOption->SetAddPos(280.0f, 0.0f);
				m_bOption = true;
				break;
			case SEC_RETRY:
				m_bPause = false;
				m_bRetry = true;
				break;
			case SEC_RETURN:
				m_bClose = true;
				m_ftime = 0.1f;
				break;
			case SEC_STAGESELECT:
				m_bPause = false;
				m_bSelect = true;
				break;
			default:
				break;
			}
		}
	}
	//バー選択中
	else
	{

		switch (section)
		{
		case SEC_OPTION:
			m_pOption->Update();
			for (int i = 0; i < 3; i++)
			{
				m_pSoundPause[i]->SetMasterVolume();
				m_pSoundPauseSE[i]->SetVolume(m_pOption->GetSEVoluem());
			}
			if (!m_pOption->GetOption())
			{
				if (WithGetKeyTriger(XINPUT_GAMEPAD_START, VK_ESCAPE))
				{
					//se
					m_pSoundPauseSE[0]->Stop();
					m_pSoundPauseSE[0]->FlushSourceBuffers();
					buffer = m_pSoundPause[0]->GetBuffer(false);
					m_pSoundPauseSE[0]->SubmitSourceBuffer(&buffer);
					SetVolumeSE(m_pSoundPauseSE[0]);
					m_pSoundPauseSE[0]->Start();
					m_bOption = false;
				}
			}
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
	static int b = m_pOption->GetIsFullScreen();
	if (m_pOption->GetIsFullScreen() != b)
	{

		SetFullscreenSwap();
		if (b == 1)b = 0;
		else if (b == 0)b = 1;
	}
	static int Resolusion = m_pOption->GetResolusion();
	static int OldResolusion = 1;
	OldResolusion = Resolusion;
	Resolusion = m_pOption->GetResolusion();
	if (Resolusion != OldResolusion)
	{

		switch (Resolusion)
		{
		case SCREEN_1920:
			SetNowResolusion(1920, 1080);
			InitResolusionMain();
			break;
		case SCREEN_1600:
			SetNowResolusion(1600, 900);
			InitResolusionMain();
			break;
		case SCREEN_1280:
			SetNowResolusion(1280, 720);
			InitResolusionMain();
			break;
		case SCREEN_800:
			SetNowResolusion(800, 600);
			InitResolusionMain();
			break;
		}
	}
	//ゲームに戻る戻るとき
	if (m_bPause && m_bClose)
	{
		// 毎フレーム判定 
		if (m_ftime > 0.0f) {
			m_ftime -= 1.0f / 60.0f;
			if (m_ftime < 0.0f)
			{
				m_ftime = 0.0f;
			}
		}
		else
		{
			m_bPause = false;
			m_bClose = false;
		}
		for (int i = 0; i < 9; i++)
		{
			m_fPos[i].X = OutEasing(m_ftime, 0.0f, 285.0f + 2800.0f, 1.0f) - 300.0f;
		}
		m_fPos[9].X = OutEasing(m_ftime, 0.0f, 290.0f + 2800.0f, 1.0f) - 300.0f;
	}
}

void CPause::Draw()
{
	//m_pBackGround->Draw();
	Sprite::ReSetSprite();
	SetRender2D();
	//背景
	m_pPauseTex[10]->Setcolor(1.0f, 1.0f, 1.0f, 0.5f);
	m_pPauseTex[10]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
	m_pPauseTex[10]->SetTexture();
	m_pPauseTex[10]->SetView(Get2DView());
	m_pPauseTex[10]->SetProjection(Get2DProj());
	m_pPauseTex[10]->SetPositon(m_fPos[10].X, m_fPos[10].Y, m_fPos[10].Z);
	m_pPauseTex[10]->SetSize(m_fSize[10].X, m_fSize[10].Y, m_fSize[10].Z);
	m_pPauseTex[10]->Disp();
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
	if (!m_pOption->GetOption())
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
	//オプション画面
	if (m_pOption->GetOption())
	{
		m_pOption->Draw();
	}
}

void CPause::SetOption(COption* InOption)
{
	m_pOption = InOption;
	//m_pOption->SetAddPosX(200.0f);
}

void CPause::SetRetry()
{
	m_bRetry = false;
}

void CPause::SetPause()
{
	m_bPause = true;
}

void CPause::SetSelect()
{
	m_bSelect = false;
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

void CPause::InitReload()
{
	for (int i = 0; i < 11; i++)
	{
		if (m_pPauseTex[i]) SAFE_DELETE(m_pPauseTex[i]);
	}

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
	m_pPauseTex[10] = new SpriteEx(TEX_PASS("Pause/kuro.png"));

	m_pSoundPause[0] = new CSoundList(SE_CANCEL);
	m_pSoundPause[1] = new CSoundList(SE_DECISION);
	m_pSoundPause[2] = new CSoundList(SE_SELECT);
	for (int i = 0; i < 3; i++)
	{
		m_pSoundPauseSE[i] = m_pSoundPause[i]->GetSound(false);
	}
}
