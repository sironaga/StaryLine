#include "SceneGame.h"
#include "File.h"
#include "Input.h"
#include "SceneResult.h"
#include "InputEx.h"
#include "SpriteDrawer.h"
#include <future>
#include <thread>

// 行き止まりが発生しない時のサイクル
enum SceneGameTime
{
	SHAPE_DRAW_START = 0,		// 最初の作図開始(0～10)
	SHAPE_SUMMON_START = 10,	// 召喚開始(10～20)
	COOLTIME_START = 10,		// 再作図可能になるまでのクールタイムのスタート(20～30)、バトルもこの位置からずっと動かす
	SHAPE_DRAW_RESTART = 15,	// 再作図開始(30～40)
	GAME_END = 90				// 時間切れ
};

struct GAME_TIME
{
	float GameTime;             //ゲーム全体のタイム1/60秒
	float GamePhaseTime;		// 一つのサイクル経過タイム1/60秒
	float GameSTimePheseAjust;	// 行き止まりになった際の時間の補正(行き止まりにならなかった時は0)
	float GameSTimeFeverAjust;  //フィーバー時の時間の補正
	float GameSTimeFeverStellaAjust;//フィーバー中にステラを取った時の時間補正
}g_tTime;

struct ResultCheck
{
	bool IsEnd;//勝敗が追加たらtrue
	bool IsWin;//勝ったらtrue
};

bool m_bFever;
bool TimeStart;
float FadeTime;
bool FadeTimeFlag;
int DrawCount;

ResultCheck g_Resltcheck;
ResultGameInfo g_ResultData;

CSceneGame::CSceneGame(StageType StageNum)
	:m_bEnd(false)
{
	g_ResultData = {};
	g_GameSound = new CSoundList(BGM_BATTLE);
	g_GameSound->SetMasterVolume();
	m_pSourseGameBGM = g_GameSound->GetSound(true);

	m_pBackGround = new CBackGround();
	m_pFieldVertex = new CFieldVertex();
	m_pPlayer = new CPlayer();
	m_pBattle = new CBattle();
	m_pField = new Field(StageNum);
	m_pStarLine = new StarLine();

	m_pFieldVertex->SetBattleAddress(m_pBattle);
	m_pFieldVertex->SetPlayerAddress(m_pPlayer);
	m_pPlayer->SetFieldVertexAddress(m_pFieldVertex);
	m_pFieldVertex->SetStarLineAddress(m_pStarLine);
	SetFileAddress(m_pBattle);

	InitCharacterTexture(StageNum);//キャラクターテクスチャ～の初期化
	m_pBattle->m_nStageNum = StageNum;

	// タイマー初期化
 	g_tTime = {};

	m_bPhase = true;
	m_bFever = false;

	TimeStart = false;
	g_tTime.GameTime = 0.0f;
	g_tTime.GamePhaseTime = -1.0f;

	DrawCount = 0;

	InitSave();

	m_pBattle->RandomSelectPattern();
	//音の再生
	if (m_pSourseGameBGM)SetVolumeBGM(m_pSourseGameBGM);
	m_pSourseGameBGM->Start();
	g_FeverSound = new CSoundList(BGM_FEVER);
	g_FeverSound->SetMasterVolume();
	m_pSourseFeverBGM = g_FeverSound->GetSound(true);

	m_pEffect = new CEffectManager(TEX_PASS("Effect/Fire.efk"));

	FadeTimeFlag = true;
	FadeTime = 0.0f;
}

CSceneGame::~CSceneGame()
{
	m_GameDirection->SetTimer(0.0f);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pFieldVertex);
	SAFE_DELETE(m_pBattle);
	SAFE_DELETE(m_pField);
	SAFE_DELETE(m_pBackGround);
	if (m_pSourseGameBGM)
	{
		m_pSourseGameBGM->Stop();
		m_pSourseGameBGM = nullptr;
	}
	SAFE_DELETE(g_GameSound);
	if (m_pSourseFeverBGM)
	{
		m_pSourseFeverBGM->Stop();
		m_pSourseFeverBGM = nullptr;
	}
	SAFE_DELETE(g_FeverSound);

	UnInitCharacterTexture();
	SAFE_DELETE(m_pEffect);
}

void CSceneGame::Update()
{
	if (IsFadeFinish())
	{
		if (FadeTimeFlag)
		{
			FadeTime++;
			m_pBattle->SetDrawingStart(true);
			m_pBattle->SetDrawingEnd(false);
		}
		if (FadeTime > 4.0f * 60.0f)
		{
			FadeTimeFlag = false;
			g_tTime.GameTime++;//フェードタイム終わったらゲームのタイマースタート

		}
		if (m_pBattle->GetEnd() && !m_bEnd)
		{
			g_ResultData.bWin = m_pBattle->GetWin();
			g_ResultData.nHitPoint = m_pBattle->GetPlayerHpProportion();
			g_ResultData.nAverageSpwn = m_pBattle->GetSummonAllyCount();
			g_ResultData.nDrawCount = DrawCount;
			g_ResultData.nSpawnCount = m_pBattle->GetSummonAllyCount();
			g_ResultData.nTime = g_tTime.GameTime / 60.0f;
			CSceneResult::InResultData(g_ResultData);
			CSceneResult::InStageLevel(m_pBattle->m_nStageNum);
			SetNext(SCENE_RESULT);
			m_bEnd = true;
		}

		if (m_bEnd)return;

		m_pBackGround->Update();

		m_pBattle->CreateLeader();

		m_pFieldVertex->LogUpdate();

		m_pField->Update();		// フィールドは常に更新する
		InitInput();
		if (!FadeTimeFlag && !TimeStart && (g_tTime.GamePhaseTime == -1.0f) && (CGetButtons(XINPUT_GAMEPAD_A) || IsKeyPress(VK_SPACE)))
		{
			TimeStart = true;
			DrawCount++;
		}
		// ゲーム中(移動をしてから)は経過時間を記録し続ける
		if (TimeStart)
		{
			g_tTime.GamePhaseTime++;
		}
		if (!TimeStart && g_tTime.GamePhaseTime == -1.0f)
		{
			m_pPlayer->UpdateStop();
		}

		if (((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GamePhaseTime && !TimeStart))	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)
		{
			g_tTime.GamePhaseTime++;
		}

		// 移動が詰んだ時
		if (m_pPlayer->GetMoveStop() && m_bPhase && (g_tTime.GamePhaseTime != -1.0f))
		{
			// 召喚開始の時間(本来の値 + 前回のサイクルが終了した時間)と現在時間(経過時間)の差を求めて
			// フェーズごとの補正値(STimePheseAjust)に代入する
			if (!m_bFever)
			{
				if (0.0f <= ((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GamePhaseTime) && ((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GamePhaseTime) <= 600.0f)
				{
					g_tTime.GameSTimePheseAjust = (float)SHAPE_SUMMON_START * 60.0f - g_tTime.GamePhaseTime;
				}
			}
			else
			{
				g_tTime.GameSTimeFeverAjust = g_tTime.GamePhaseTime - ((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust - g_tTime.GameSTimeFeverStellaAjust);
				m_pFieldVertex->ResetFeverPoint();
				m_pPlayer->SetMoveStop();
			}
			m_bPhase = false;
		}
		if (!m_GameDirection->GetDraw())
		{
			// 召喚開始の時間になったらフィーバーかチェック
			if ((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust == g_tTime.GamePhaseTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
			{
				float t;
				t = m_pFieldVertex->GetFeverPoint();

				if (t == 30.0f)
				{
					m_pStarLine->SetLineMode(1);
					m_pSourseGameBGM->Stop();
					m_pSourseFeverBGM->FlushSourceBuffers();
					XAUDIO2_BUFFER buffer;
					buffer = g_FeverSound->GetBuffer(true);
					m_pSourseFeverBGM->SubmitSourceBuffer(&buffer);
					if (m_pSourseFeverBGM)SetVolumeBGM(m_pSourseFeverBGM);
					m_pSourseFeverBGM->Start();
					m_bPhase = true;
					m_bFever = true;
					g_tTime.GameSTimeFeverAjust = 10.0f * 60.0f;
					m_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
					m_pPlayer->SetMoveStop();
				}
			}

			//毎描画開始時スーパースターをセット
			if ((float)SHAPE_DRAW_START * 60.0f == g_tTime.GamePhaseTime)
			{
				m_pStarLine->SetLineMode(0);
				m_pEffect->Play({ m_pPlayer->GetPlayerPos().x, m_pPlayer->GetPlayerPos().y, m_pPlayer->GetPlayerPos().z });
			}
			m_pPlayer->Update();
			// 図形を作る時間
			// 経過時間が作図開始の時間から召喚開始の時間になるまで
			if (((float)SHAPE_DRAW_START * 60.0f <= g_tTime.GamePhaseTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
				(g_tTime.GamePhaseTime < (float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust + g_tTime.GameSTimeFeverStellaAjust))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間+フィーバー時間)未満
			{
				// プレイヤーと作図処理は図形を作っている間更新する

				m_pEffect->SetPos({ m_pPlayer->GetPlayerPos().x, m_pPlayer->GetPlayerPos().y, m_pPlayer->GetPlayerPos().z });
				m_pFieldVertex->Update();
			}
			else
			{
				//描画時間外だと停止
				m_pEffect->Stop();
				m_pFieldVertex->SoundStop();
				m_pPlayer->Reset();
			}

			// 召喚開始の時間になったら
			if ((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust == g_tTime.GamePhaseTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
			{
				m_pBattle->CreateAlly();	// キャラクターのデータを生成する
			}

			// フィーバーの召喚開始時間になったら
			if (((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust + g_tTime.GameSTimeFeverStellaAjust == g_tTime.GamePhaseTime) && m_bFever)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
			{
				m_pEffect->Play({ m_pPlayer->GetPlayerPos().x, m_pPlayer->GetPlayerPos().y, m_pPlayer->GetPlayerPos().z });
				m_pBattle->CreateAlly();	// キャラクターのデータを生成する
			}

			// 描画時間終了時間または描画時間+フィーバーの終了時間
			if (g_tTime.GamePhaseTime == (float)COOLTIME_START * 60.0f - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust + g_tTime.GameSTimeFeverStellaAjust)			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
			{
				m_pBattle->SetDrawingStart(false);
				m_pBattle->SetDrawingEnd(true);
				m_pBattle->CharacterUpdate(true);
				m_pFieldVertex->SetNowLine(); //一番最後の線を表示しないようにする
				/*タイマー終了のSE*/
			}
			m_pBattle->Update();
		}
		if (FadeTimeFlag)
		{
			m_pBattle->CharacterUpdate(true);	// 生成されたキャラクターのアニメーションを行う
		}
	}
}

void CSceneGame::Draw()
{
	m_pBackGround->Draw();
	m_pField->Draw();	// フィールドは常に描画する

	m_pFieldVertex->Draw();
	m_pPlayer->Draw();

	//キャラクターを召喚する時間
	if (((float)SHAPE_SUMMON_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GamePhaseTime))	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)
	{
		std::thread Th_BattleCharacterDraw(&CBattle::CharacterDraw, m_pBattle);
		Th_BattleCharacterDraw.join();
		if(!m_bFever)TimeStart = false;
	}

	std::thread Th_BattleDraw(&CBattle::Draw, m_pBattle);
	Th_BattleDraw.join();
	m_pBattle->Draw();	// バトル全体の描画
	

	// 一つのサイクルが終わった時
	if (g_tTime.GamePhaseTime == (float)SHAPE_DRAW_RESTART * 60.0f - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust + g_tTime.GameSTimeFeverStellaAjust)// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		m_pBattle->SetDrawingStart(true);
		m_pBattle->SetDrawingEnd(false);
		m_pBattle->CharacterUpdate(true);
		// 次のサイクルに向けて各処理を行う
		// 時間の初期化処理
		g_tTime.GameSTimeFeverStellaAjust = 0.0f;
		g_tTime.GameSTimeFeverAjust = 0.0f;
		g_tTime.GameSTimePheseAjust = 0.0f;				// 移動に詰んだ時の補正値を初期化

		g_tTime.GamePhaseTime = -1.0f;
		TimeStart = false;

		// 各要素の初期化処理
		m_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
		m_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える

		m_bPhase = true;

		/*再描画開始のSE*/
	}
	if (m_bFever)
	{
		m_pFieldVertex->SubtractFeverPoint();
	}
	if (m_bFever && (m_pFieldVertex->GetFeverPoint() <= 0.0f))
	{
		if (m_pSourseFeverBGM)
		{
			m_pSourseFeverBGM->Stop();
			
		}
		m_pEffect->Stop();
		m_pSourseGameBGM->Start();
		m_bFever = false;
	}
	m_pEffect->Draw();
	m_pFieldVertex->FeverDraw();
}

void SetFeverStellaTime(float time)
{
	g_tTime.GameSTimeFeverStellaAjust += time;
}

bool GetFade()
{
	return FadeTimeFlag;
}

bool GetFeverMode()
{
	return m_bFever;
}

bool GetTimeStart()
{
	return TimeStart;
}

float GetTime()
{
	return g_tTime.GamePhaseTime;
}

void CSceneGame::SetMasterVolume()
{
	g_FeverSound->SetMasterVolume();
	g_GameSound->SetMasterVolume();
	m_pFieldVertex->SetMasterVolume();
	SetVolumeBGM(m_pSourseGameBGM);
	SetVolumeBGM(m_pSourseFeverBGM);
}

void CSceneGame::SetResult(bool InWin)
{
	g_Resltcheck.IsEnd = true;
	g_Resltcheck.IsWin = InWin;
}

void CSceneGame::InitResolusion(float wide, float height, bool fullscreen)
{
	UninitDirectX();
	InitDirectX(m_phWnd, wide, height, fullscreen);

	Geometory::Uninit();
	Geometory::Init();
	Sprite::Init();
	LibEffekseer::Uninit();
	LibEffekseer::Init(GetDevice(), GetContext());
	InitInput();
	ShaderList::Uninit();
	ShaderList::Init();
	UninitSpriteDrawer();
	InitSpriteDrawer(GetDevice(), GetContext(), wide, height);

	SAFE_DELETE(m_pBackGround);
	m_pBackGround = new CBackGround();
	SAFE_DELETE(g_GameSound);
	g_GameSound = new CSoundList(BGM_BATTLE);
	g_GameSound->SetMasterVolume();
	m_pSourseGameBGM = g_GameSound->GetSound(true);
	SAFE_DELETE(g_FeverSound);
	g_FeverSound = new CSoundList(BGM_FEVER);
	g_FeverSound->SetMasterVolume();
	m_pFieldVertex->InitSound();
	m_pPlayer->InitSound();

	m_pPlayer->Reload();
	ReLoadCharacterTexture(m_pBattle->m_nStageNum);
	m_pBattle->ReLoadTexture();
	
	SetNowResolusion(wide, height);
}
