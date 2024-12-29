//SceneGame.h
//作成者：AT12B浦口龍輝

//開発履歴 
//10/31プロトタイプ宣言の追加


#pragma once
#include"Main.h"

void InitSceneGame(StageType StageNum);
void UninitSceneGame();
void UpdateSceneGame();
void DrawSceneGame();

bool GetFeverMode();

void SetResult(bool InWin);