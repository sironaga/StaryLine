#include "SceneGame.h"
#include "File.h"
#include "Input.h"
#include "SceneResult.h"


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
	float GameTime;				// 1/60秒
	//float GameSTime;			// 秒
	float GameSTimeSycleEnd;	// 一つのサイクルが終わった時の時間
	float GameSTimePheseAjust;	// 行き止まりになった際の時間の補正(行き止まりにならなかった時は0)
	float GameSTimeFeverAjust;  //フィーバー時の時間の補正
}g_tTime;

struct ResultCheck
{
	bool IsEnd;//勝敗が追加たらtrue
	bool IsWin;//勝ったらtrue
};

bool m_bFever;
bool TimeStart;

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

	m_pFieldVertex->SetBattleAddress(m_pBattle);
	m_pFieldVertex->SetPlayerAddress(m_pPlayer);
	m_pPlayer->SetFieldVertexAddress(m_pFieldVertex);
	SetFileAddress(m_pBattle);

	InitCharacterTexture(m_pFieldVertex, StageNum);//キャラクターテクスチャ～の初期化
	m_pBattle->m_nStageNum = StageNum;

	// タイマー初期化
 	g_tTime = {};

	m_bPhase = true;
	m_bFever = false;

	TimeStart = false;
	g_tTime.GameTime = -1.0f;

	InitSave();

	m_pBattle->RandomSelectPattern();
	//音の再生
	if (m_pSourseGameBGM)SetVolumeBGM(m_pSourseGameBGM);
	m_pSourseGameBGM->Start();
	g_FeverSound = new CSoundList(BGM_FEVER);
	g_FeverSound->SetMasterVolume();
	m_pSourseFeverBGM = g_FeverSound->GetSound(true);

	m_pEffect = new CEffectManager(TEX_PASS("Effect/Fire.efk"));

	
}

CSceneGame::~CSceneGame()
{
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
	if (m_pBattle->GetEnd() && !m_bEnd)
	{
		g_ResultData.bWin = m_pBattle->GetWin();
		g_ResultData.nHitPoint = m_pBattle->GetPlayerHpProportion();
		g_ResultData.nAverageSpwn = m_pBattle->GetSummonAllyCount();
		g_ResultData.nDrawCount = 10;
		g_ResultData.nSpawnCount = m_pBattle->GetSummonAllyCount();
		g_ResultData.nTime = g_tTime.GameTime;
		CSceneResult::InResultData(g_ResultData);
		CSceneResult::InStageLevel(m_pBattle->m_nStageNum);
		SetNext(SCENE_RESULT);
		m_bEnd = true;
	}

	if (m_bEnd)return;

	m_pBackGround->Update();

	m_pBattle->CreateLeader();

	m_pFieldVertex->LogUpdate();

	//g_tTime.GameSTime = g_tTime.GameTime / 60;	// 秒数(STime)に変換する

	m_pField->Update();		// フィールドは常に更新する
	InitInput();
	if (!TimeStart && (g_tTime.GameTime == -1.0f) && IsKeyPress(VK_SPACE))
	{
		TimeStart = true;
	}
	// ゲーム中(移動をしてから)は経過時間を記録し続ける
	if (TimeStart)
	{
		g_tTime.GameTime++;
	}

	// 移動が詰んだ時
	if (!m_pPlayer->GetCanMove() && m_bPhase)
	{
		// 召喚開始の時間(本来の値 + 前回のサイクルが終了した時間)と現在時間(経過時間)の差を求めて
		// フェーズごとの補正値(STimePheseAjust)に代入する
		if (!m_bFever)
		{
			if (0.0f <= ((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameTime) && ((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameTime) <= 600.0f)
			{
				g_tTime.GameSTimePheseAjust = (float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameTime;
			}
		}
		else
		{
			g_tTime.GameSTimeFeverAjust = g_tTime.GameTime - ((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameSTimePheseAjust);
			m_pFieldVertex->ResetFeverPoint();
		}
		m_bPhase = false;


	}

	// 召喚開始の時間になったらフィーバーかチェック
	if ((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameSTimePheseAjust == g_tTime.GameTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		float t;
		t = m_pFieldVertex->GetFeverPoint();

		if (t == 30.0f)
		{
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
			//m_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える
		}
	}

	//毎描画開始時スーパースターをセット
	if ((float)SHAPE_DRAW_START * 60.0f /* + g_tTime.GameSTimeSycleEnd */ == g_tTime.GameTime)
	{
		m_pEffect->Play({ m_pPlayer->GetPlayerPos().x, m_pPlayer->GetPlayerPos().y, m_pPlayer->GetPlayerPos().z });
		//m_pFieldVertex->SetSuperStar();
	}
	m_pPlayer->Update();
	// 図形を作る時間
	// 経過時間が作図開始の時間から召喚開始の時間になるまで
	if (((float)SHAPE_DRAW_START * 60.0f /*+ g_tTime.GameSTimeSycleEnd*/ <= g_tTime.GameTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameTime < (float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間+フィーバー時間)未満
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

	if (!m_GameDirection->GetDraw())
	{
		// 召喚開始の時間になったら
		if ((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ -g_tTime.GameSTimePheseAjust == g_tTime.GameTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
		{
			m_pBattle->CreateAlly();	// キャラクターのデータを生成する
		}

		// フィーバーの召喚開始時間になったら
		if (((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ -g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust == g_tTime.GameTime) && m_bFever)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
		{
			m_pEffect->Play({ m_pPlayer->GetPlayerPos().x, m_pPlayer->GetPlayerPos().y, m_pPlayer->GetPlayerPos().z });
			m_pBattle->CreateAlly();	// キャラクターのデータを生成する
		}

		// 描画時間終了時間または描画時間+フィーバーの終了時間
		if (g_tTime.GameTime == (float)COOLTIME_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ -g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust)			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
		{
			m_pFieldVertex->SetNowLine(); //一番最後の線を表示しないようにする
		}

		// バトルは１回目のCOOLTIMEが始まったらそれ以降常に更新する
		//if ((float)COOLTIME_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime)
		{
			m_pBattle->Update();	// 経過時間が1度目のクールタイム(本来の値 - 移動に詰んだ時の補正値)以上になった時
		}
	}
		m_pBattle->CharacterUpdate();	// 生成されたキャラクターのアニメーションを行う
	
}

void CSceneGame::Draw()
{
	m_pBackGround->Draw();
	m_pField->Draw();	// フィールドは常に描画する

	m_pFieldVertex->Draw();
	m_pPlayer->Draw();
	// 図形を作る時間
	// 経過時間が作図開始の時間から召喚開始の時間になるまで
	//if ((SHAPE_DRAW_START + g_tTime.GameSTimeSycleEnd <= g_tTime.GameSTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
	//	(g_tTime.GameSTime < (SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust)))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	//{
	//	// プレイヤーと作図処理は図形を作っている間描画する
	//	m_pFieldVertex->Draw();
	//}

	// 経過時間が召喚開始の時間からクールタイム開始の時間になるまで
	// キャラクターを召喚する時間
	//if (((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime) &&	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)以上 かつ
	//	(g_tTime.GameTime < (float)COOLTIME_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust))			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	//{
	//	m_pBattle->CharacterDraw();	// 生成されたキャラクターの描画を行う
	//}
	//キャラクターを召喚する時間
	if (((float)SHAPE_SUMMON_START * 60.0f + /*g_tTime.GameSTimeSycleEnd*/ - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime))	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)
	{
		m_pBattle->CharacterDraw();	// 生成されたキャラクターの描画を行う
	}

	//１回目のCOOLTIMEが始まったらそれ以降処理
	//if ((float)COOLTIME_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime)
	{
		m_pBattle->Draw();	// バトル全体の描画
	}

	// 一つのサイクルが終わった時
	if (g_tTime.GameTime == (float)SHAPE_DRAW_RESTART * 60.0f + /*g_tTime.GameSTimeSycleEnd*/
		- g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust)// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		// 次のサイクルに向けて各処理を行う
		// 時間の初期化処理
		g_tTime.GameSTimeFeverAjust = 0.0f;
		g_tTime.GameSTimePheseAjust = 0.0f;				// 移動に詰んだ時の補正値を初期化
		//g_tTime.GameSTimeSycleEnd = g_tTime.GameTime;	// 前回のサイクルが終了した時間を更新

		g_tTime.GameTime = -1.0f;
		TimeStart = false;

		// 各要素の初期化処理
		m_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
		m_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える

		m_bPhase = true;

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
			//m_pSourseFeverBGM = nullptr;
		}
		//SAFE_DELETE(g_FeverSound);
		m_pEffect->Stop();
		m_pSourseGameBGM->Start();
		m_bFever = false;
	}
	m_pEffect->Draw();
}

bool GetFeverMode()
{
	return m_bFever;
}

bool GetTimeStart()
{
	return TimeStart;
}

void CSceneGame::SetResult(bool InWin)
{
	g_Resltcheck.IsEnd = true;
	g_Resltcheck.IsWin = InWin;
}
