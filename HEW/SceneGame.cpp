#include "SceneGame.h"
#include "FieldVertex.h"
#include "Player.h"
#include "Battle.h"
#include "File.h"
#include "Character.h"
#include "StageSelect.h"
#include "Field.h"
#include "SoundList.h"

E_GAME_PHASE g_ePhaseType;
CFieldVertex* g_pFieldVertex;
CPlayer* g_pPlayer;
CBattle* g_pBattle;
Field* g_pField;

// 行き止まりが発生しない時のサイクル
enum SceneGameTime
{
	SHAPE_DRAW_START = 0,		// 最初の作図開始(0～10)
	SHAPE_SUMMON_START = 10,	// 召喚開始(10～20)
	COOLTIME_START = 15,		// 再作図可能になるまでのクールタイムのスタート(20～30)、バトルもこの位置からずっと動かす
	SHAPE_DRAW_RESTART = 20,	// 再作図開始(30～40)
	GAME_END = 90				// 時間切れ
};

struct GAME_TIME
{
	float GameTime;				// 1/60秒
	float GameSTime;			// 秒
	float GameSTimeSycleEnd;	// 一つのサイクルが終わった時の時間
	float GameSTimePheseAjust;	// 行き止まりになった際の時間の補正(行き止まりにならなかった時は0)
}g_tTime;

IXAudio2SourceVoice* g_pSourseGameBGM;

bool Phase;

// 初期化処理
void InitSceneGame(int StageNum)
{
	g_ePhaseType = DRAWING;

	g_pFieldVertex = new CFieldVertex();
	g_pPlayer = new CPlayer();
	g_pBattle = new CBattle();
	g_pField = new Field();

	g_pFieldVertex->SetBattleAddress(g_pBattle);
	g_pFieldVertex->SetPlayerAddress(g_pPlayer);
	g_pPlayer->SetFieldVertexAddress(g_pFieldVertex);
	SetFileAddress(g_pBattle);

	IninCharacterTexture(g_pFieldVertex, StageNum);//キャラクターテクスチャ～の初期化
	g_pBattle->m_nStageNum = StageNum;

	// タイマー初期化
	g_tTime = {};

	Phase = true;

	InitSave();
	g_pSourseGameBGM = GetSound(BGM_BATTLE,true);
	g_pSourseGameBGM->SetVolume(0.4f);
	g_pSourseGameBGM->Start();
	g_pBattle->RandomSelectPattern();
}

//終了処理
void UninitSceneGame()
{
	if (g_pPlayer)
	{
		delete g_pPlayer;
		g_pPlayer = nullptr;
	}
	if (g_pFieldVertex)
	{
		delete g_pFieldVertex;
		g_pFieldVertex = nullptr;
	}
	if (g_pBattle)
	{
		delete g_pBattle;
		g_pBattle = nullptr;
	}
	if (g_pField)
	{
		delete g_pField;
		g_pField = nullptr;
	}
	if (g_pSourseGameBGM)
	{
		g_pSourseGameBGM = nullptr;
	}
}

//更新処理
void UpdateSceneGame()
{
	g_pBattle->CreateLeader();

	g_tTime.GameTime++;							// ゲーム中は経過時間を記録し続ける
	g_tTime.GameSTime = g_tTime.GameTime / 60;	// 秒数(STime)に変換する

	g_pField->Update();		// フィールドは常に更新する

	// 移動が詰んだ時
	if (g_pPlayer->GetPlayerPhase() && Phase)
	{
		// 召喚開始の時間(本来の値 + 前回のサイクルが終了した時間)と現在時間(経過時間)の差を求めて
		// フェーズごとの補正値(STimePheseAjust)に代入する
		g_tTime.GameSTimePheseAjust = (SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd) - g_tTime.GameSTime;
		Phase = false;
	}

	// 図形を作る時間
	// 経過時間が作図開始の時間から召喚開始の時間になるまで
	if ((SHAPE_DRAW_START + g_tTime.GameSTimeSycleEnd <= g_tTime.GameSTime) &&								// 経過時間が作図開始の時間(本来の値 + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameSTime < SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust))	// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	{
		// プレイヤーと作図処理は図形を作っている間更新する
		g_pPlayer->Update();
		g_pFieldVertex->Update();
	}

	// 召喚開始の時間になったら
	if ((SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust) == g_tTime.GameSTime)// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		g_pBattle->CreateAlly();	// キャラクターのデータを生成する
	}

	// 経過時間が召喚開始の時間からクールタイム開始の時間になるまで
	// キャラクターを召喚する時間
	if (((SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust) <= g_tTime.GameSTime) &&	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameSTime < (COOLTIME_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust)))			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	{
		g_pBattle->CharacterUpdate();	// 生成されたキャラクターのアニメーションを行う
	}

	// バトルは１回目のCOOLTIMEが始まったらそれ以降常に更新する
	if((COOLTIME_START - g_tTime.GameSTimePheseAjust) <= g_tTime.GameSTime)g_pBattle->Update();	// 経過時間が1度目のクールタイム(本来の値 - 移動に詰んだ時の補正値)以上になった時
}

//描画処理
void DrawSceneGame()
{
	g_pField->Draw();	// フィールドは常に描画する
	g_pPlayer->Draw();
	g_pFieldVertex->Draw();

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
	if (((SHAPE_SUMMON_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust ) <= g_tTime.GameSTime) &&	// 経過時間が召喚開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)以上 かつ
		(g_tTime.GameSTime < (COOLTIME_START + g_tTime.GameSTimeSycleEnd - g_tTime.GameSTimePheseAjust )))			// 経過時間がクールタイム開始の時間((本来の値  - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)未満
	{
		g_pBattle->CharacterDraw();	// 生成されたキャラクターの描画を行う
	}

	//１回目のCOOLTIMEが始まったらそれ以降処理
	if (COOLTIME_START <= g_tTime.GameSTime)g_pBattle->Draw();	// バトル全体の描画

	// 一つのサイクルが終わった時
	if (g_tTime.GameTime == (SHAPE_DRAW_RESTART + g_tTime.GameSTimeSycleEnd 
		- g_tTime.GameSTimePheseAjust) * 60)// 経過時間が召喚開始の時間((本来の値 - 移動に詰んだ時の補正値) + 前回のサイクルが終了した時間)の時
	{
		// 次のサイクルに向けて各処理を行う

		// 時間の初期化処理
 		g_tTime.GameSTimePheseAjust = 0;				// 移動に詰んだ時の補正値を初期化
		g_tTime.GameSTimeSycleEnd = g_tTime.GameSTime;	// 前回のサイクルが終了した時間を更新

		// 各要素の初期化処理
		g_pFieldVertex->InitFieldVertex();	// 次の作図に必用な初期化処理	
		g_pPlayer->SetPlayerStop();			// プレイヤーの状態をSTOPに変える

		Phase = true;
	}
}

void ChangePhase(E_GAME_PHASE next)
{
	// 現在のシーンの更新
	g_ePhaseType = next;
}

E_GAME_PHASE GetNowPhase()
{
	return g_ePhaseType;
}
