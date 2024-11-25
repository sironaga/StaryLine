#include "SceneGame.h"
#include "FieldVertex.h"
#include "Player.h"
#include "Battle.h"
#include "File.h"
#include "Character.h"
#include "StageSelect.h"

E_GAME_PHASE g_ePhaseType;
CFieldVertex* g_pFieldVertex;
CPlayer* g_pPlayer;
CBattle* g_pBattle;

	
//初期化処理l
void InitSceneGame(int StageNum)
{
	g_ePhaseType = DRAWING;

	g_pFieldVertex = new CFieldVertex();
	g_pPlayer = new CPlayer();
	g_pBattle = new CBattle();

	g_pFieldVertex->SetBattleAddress(g_pBattle);
	g_pFieldVertex->SetPlayerAddress(g_pPlayer);
	g_pPlayer->SetFieldVertexAddress(g_pFieldVertex);
	SetFileAddress(g_pBattle);
	
	IninCharacterTexture(g_pFieldVertex);//キャラクターテクスチャ～の初期化

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
}

//更新処理
void UpdateSceneGame()
{
	switch (g_ePhaseType)
	{
	case DRAWING:
		g_pFieldVertex->Update();
		g_pPlayer->Update();
		break;
	case SHAPESCHECK:
		g_pFieldVertex->Update();
		break;
	case BATTLE:
		g_pFieldVertex->Update();
		g_pBattle->Update();
		break;
	default:
		break;
	}	
}

//描画処理
void DrawSceneGame()
{
	switch (g_ePhaseType)
	{
	case DRAWING:
		g_pFieldVertex->Draw();
		g_pPlayer->Draw();
		break;
	case SHAPESCHECK:
		g_pFieldVertex->Draw();
		
		break;
	case BATTLE:
		g_pFieldVertex->Draw();
		g_pBattle->Draw();
		break;
	default:
		break;
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
