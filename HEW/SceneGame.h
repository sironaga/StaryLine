//SceneGame.h
//作成者：AT12B浦口龍輝

//開発履歴 
//10/31プロトタイプ宣言の追加


#pragma once

void InitSceneGame(int StageNum);
void UninitSceneGame();
void UpdateSceneGame();
void DrawSceneGame();

enum E_GAME_PHASE
{
	DRAWING = 0,
	SHAPESCHECK,
	BATTLE
};

void ChangePhase(E_GAME_PHASE next);
E_GAME_PHASE GetNowPhase();