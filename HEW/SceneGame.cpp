#include "SceneGame.h"
#include "FieldVertex.h"
#include "Player.h"
#include "Battle.h"
#include "File.h"
#include "Character.h"
#include "StageSelect.h"
#include "Field.h"
#include "SoundList.h"
#include "BackGround.h"

CFieldVertex* g_pFieldVertex;
CPlayer* g_pPlayer;
CBattle* g_pBattle;
Field* g_pField;
CBackGround* g_pBackGround;
CEffectManager* m_pEffect;

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

IXAudio2SourceVoice* g_pSourseGameBGM;
CSoundList* g_GameSound;
IXAudio2SourceVoice* g_pSourseFeverBGM;
CSoundList* g_FeverSound;

bool Phase;
bool Fever;

// 初期化処理
void InitSceneGame(int StageNum)
{
	g_GameSound = new CSoundList(BGM_BATTLE);
	g_pSourseGameBGM = g_GameSound->GetSound(true);
	
	g_pBackGround = new CBackGround();
	g_pFieldVertex = new CFieldVertex();
	g_pPlayer = new CPlayer();
	g_pBattle = new CBattle();
	g_pField = new Field();

	g_pFieldVertex->SetBattleAddress(g_pBattle);
	g_pFieldVertex->SetPlayerAddress(g_pPlayer);
	g_pPlayer->SetFieldVertexAddress(g_pFieldVertex);
	SetFileAddress(g_pBattle);

	InitCharacterTexture(g_pFieldVertex, StageNum);//キャラクターテクスチャ～の初期化
	g_pBattle->m_nStageNum = StageNum;

	// タイマー初期化
	g_tTime = {};

	Phase = true;
	Fever = false;

	InitSave();
	
	g_pBattle->RandomSelectPattern();
	//音の再生
	if (g_pSourseGameBGM)SetVolumeBGM(g_pSourseGameBGM);
	g_pSourseGameBGM->Start();
	g_FeverSound = new CSoundList(BGM_FEVER);
	g_pSourseFeverBGM = g_FeverSound->GetSound(true);

	m_pEffect = new CEffectManager(TEX_PASS("Effect/Fire.efk"));
}

//終了処理
void UninitSceneGame()
{
	SAFE_DELETE(g_pPlayer);
	SAFE_DELETE(g_pFieldVertex);
	SAFE_DELETE(g_pBattle);
	SAFE_DELETE(g_pField);
	SAFE_DELETE(g_pBackGround);
	if (g_pSourseGameBGM)
	{
		g_pSourseGameBGM->Stop();
		g_pSourseGameBGM = nullptr;
	}
	SAFE_DELETE(g_GameSound);
	if (g_pSourseFeverBGM)
	{
		g_pSourseFeverBGM->Stop();
		g_pSourseFeverBGM = nullptr;
	}
	SAFE_DELETE(g_FeverSound);
	
	UnInitCharacterTexture();

	SAFE_DELETE(m_pEffect);;
}

//更新処理
void UpdateSceneGame()
{


	g_pBackGround->Update();

	g_pBattle->CreateLeader();

	
	//g_tTime.GameSTime = g_tTime.GameTime / 60;	// 秒数(STime)に変換する

	g_pField->Update();		// フィールドは常に更新する

	

	// 移動が詰んだ時
	if (!g_pPlayer->GetCanMove() && Phase)
	{
		// 召喚開始の時間(本来の値 + 前回のサイクルが終了した時間)と現在時間(経過時間)の差を求めて
		// フェーズごとの補正値(STimePheseAjust)に代入する
		if (!Fever)
		{
			if (0.0f <= ((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameTime) &&((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameTime) <= 600.0f)
			{
				g_tTime.GameSTimePheseAjust = (float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameTime;
			}
		}
		else
		{
			g_tTime.GameSTimeFeverAjust = g_tTime.GameTime - ((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust);
			g_pFieldVertex->ResetFeverPoint();
		}
		Phase = false;
		
	
	}
	
	// 召喚開始の時間になったらフィーバーかチェック
	if ((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust == g_tTime.GameTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		float t;
		t = g_pFieldVertex->GetFeverPoint();

		if (t == 30.0f)
		{
			g_pSourseGameBGM->Stop();
			g_pSourseFeverBGM->FlushSourceBuffers();
			XAUDIO2_BUFFER buffer;
			buffer = g_FeverSound->GetBuffer(true);
			g_pSourseFeverBGM->SubmitSourceBuffer(&buffer);
			if (g_pSourseFeverBGM)SetVolumeBGM(g_pSourseFeverBGM);
			g_pSourseFeverBGM->Start();
			Phase = true;
			Fever = true;
			g_tTime.GameSTimeFeverAjust = 10.0f * 60.0f;
			g_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
			//g_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える
		}
	}

	//毎描画開始時スーパースターをセット
	if ((float)SHAPE_DRAW_START *60.0f + g_tTime.GameSTimeSycleEnd == g_tTime.GameTime)
	{
		m_pEffect->Play({ g_pPlayer->GetPlayerPos().x, g_pPlayer->GetPlayerPos().y, g_pPlayer->GetPlayerPos().z });
		//g_pFieldVertex->SetSuperStar();
	}

 	g_pPlayer->Update();
	// 図形を作る時間
	// 経過時間が作図開始の時間から召喚開始の時間になるまで
	if (((float)SHAPE_DRAW_START * 60.0f + g_tTime.GameSTimeSycleEnd <= g_tTime.GameTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameTime < (float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間+フィーバー時間)未満
	{
		// プレイヤーと作図処理は図形を作っている間更新する
		m_pEffect->SetPos({ g_pPlayer->GetPlayerPos().x, g_pPlayer->GetPlayerPos().y, g_pPlayer->GetPlayerPos().z });
		g_pFieldVertex->Update();
	}
	else
	{
		//描画時間外だと停止
		m_pEffect->Stop();
		g_pFieldVertex->SoundStop();
	}

	// 召喚開始の時間になったら
	if ((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust == g_tTime.GameTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{	
		g_pBattle->CreateAlly();	// キャラクターのデータを生成する
	}

	// フィーバーの召喚開始時間になったら
	if (((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust == g_tTime.GameTime) && Fever)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		m_pEffect->Play({ g_pPlayer->GetPlayerPos().x, g_pPlayer->GetPlayerPos().y, g_pPlayer->GetPlayerPos().z });
		g_pBattle->CreateAlly();	// キャラクターのデータを生成する
	}

	// 経過時間が召喚開始の時間からクールタイム開始の時間になるまで
	// キャラクターを召喚する時間
	if (((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime) &&	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameTime < (float)COOLTIME_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust))			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	{
		g_pBattle->CharacterUpdate();	// 生成されたキャラクターのアニメーションを行う
	}

	// バトルは１回目のCOOLTIMEが始まったらそれ以降常に更新する
	//if ((float)COOLTIME_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime)
	{
		g_pBattle->Update();	// 経過時間が1度目のクールタイム(本来の値 - 移動に詰んだ時の補正値)以上になった時
	}
							// ゲーム中は経過時間を記録し続ける
	g_tTime.GameTime++;
	
}


//描画処理
void DrawSceneGame()
{
	g_pBackGround->Draw();
	g_pField->Draw();	// フィールドは常に描画する

	g_pFieldVertex->Draw();
	g_pPlayer->Draw();
	// 図形を作る時間
	// 経過時間が作図開始の時間から召喚開始の時間になるまで
	//if ((SHAPE_DRAW_START + g_tTime.GameSTimeSycleEnd <= g_tTime.GameSTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
	//	(g_tTime.GameSTime < (SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust)))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	//{
	//	// プレイヤーと作図処理は図形を作っている間描画する
	//	g_pFieldVertex->Draw();
	//}

	// 経過時間が召喚開始の時間からクールタイム開始の時間になるまで
	// キャラクターを召喚する時間
	//if (((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime) &&	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)以上 かつ
	//	(g_tTime.GameTime < (float)COOLTIME_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust))			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	//{
	//	g_pBattle->CharacterDraw();	// 生成されたキャラクターの描画を行う
	//}
	//キャラクターを召喚する時間
	if (((float)SHAPE_SUMMON_START * 60.0f + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime))	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)
	{
		g_pBattle->CharacterDraw();	// 生成されたキャラクターの描画を行う
	}

	//１回目のCOOLTIMEが始まったらそれ以降処理
	//if ((float)COOLTIME_START * 60.0f - g_tTime.GameSTimePheseAjust <= g_tTime.GameTime)
	{
		g_pBattle->Draw();	// バトル全体の描画
	}

	// 一つのサイクルが終わった時
	if (g_tTime.GameTime == (float)SHAPE_DRAW_RESTART * 60.0f + g_tTime.GameSTimeSycleEnd
		- g_tTime.GameSTimePheseAjust + g_tTime.GameSTimeFeverAjust)// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		// 次のサイクルに向けて各処理を行う
		// 時間の初期化処理
		g_tTime.GameSTimeFeverAjust = 0.0f;
		g_tTime.GameSTimePheseAjust = 0.0f;				// 移動に詰んだ時の補正値を初期化
		g_tTime.GameSTimeSycleEnd = g_tTime.GameTime;	// 前回のサイクルが終了した時間を更新

		// 各要素の初期化処理
		g_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
		g_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える

		Phase = true;
		
	}
	if (Fever)
	{
		g_pFieldVertex->SubtractFeverPoint();
	}
	if (Fever && (g_pFieldVertex->GetFeverPoint() <= 0.0f))
	{
		if (g_pSourseFeverBGM)
		{
			g_pSourseFeverBGM->Stop();
			//g_pSourseFeverBGM = nullptr;
		}
		//SAFE_DELETE(g_FeverSound);
		m_pEffect->Stop();
		g_pSourseGameBGM->Start();
		Fever = false;
	}
	m_pEffect->Draw();
}

bool GetFeverMode()
{
	return Fever;
}
