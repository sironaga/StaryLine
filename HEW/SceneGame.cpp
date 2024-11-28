#include "SceneGame.h"
#include "FieldVertex.h"
#include "Player.h"
#include "Battle.h"
#include "File.h"
#include "Character.h"
#include "StageSelect.h"
#include "Field.h"

E_GAME_PHASE g_ePhaseType;
CFieldVertex* g_pFieldVertex;
CPlayer* g_pPlayer;
CBattle* g_pBattle;
Field* g_pField;

enum SceneGameTime
{
	SHAPE_DRAW = 0,
	SHAPE_END = 10,
	COOLTIME = 20,
	GAME_END = 90
};

int GameTime;//秒×60
int GameSTime;//秒
int GameSTimeError;//時間のズレ
	
//初期化処理l
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
	
	IninCharacterTexture(g_pFieldVertex);//キャラクターテクスチャ～の初期化

	GameTime = 0;//タイマー初期化
	GameSTime = 0;
	GameSTimeError = 0;

	switch (StageNum)
	{
	default:
	case(STAGE1):
		break;
	case(STAGE2):
		break;
	case(STAGE3):
		break;
	}

	InitSave();
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
}

//更新処理
void UpdateSceneGame()
{
	GameTime++;
	GameSTime = GameTime / 60;

	g_pField->Update();
	if(COOLTIME <= GameSTime)g_pBattle->Update();
	if(SHAPE_DRAW + GameSTimeError <= GameSTime && GameSTime < COOLTIME + GameSTimeError)g_pFieldVertex->Update();
	if(SHAPE_DRAW + GameSTimeError <= GameSTime && GameSTime < SHAPE_END + GameSTimeError)g_pPlayer->Update();
	
}

//描画処理
void DrawSceneGame()
{
	g_pField->Draw();
	if (COOLTIME <= GameSTime)g_pBattle->Draw();
	if (SHAPE_DRAW + GameSTimeError <= GameSTime && GameSTime < COOLTIME + GameSTimeError)g_pFieldVertex->Draw();
	if (SHAPE_DRAW + GameSTimeError <= GameSTime && GameSTime < SHAPE_END + GameSTimeError)g_pPlayer->Draw();
	if (GameTime == COOLTIME * 60)
	{
		GameSTimeError = GameSTime;
		//フィールドの初期化処理
		g_pFieldVertex->InitFieldVertex();
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
