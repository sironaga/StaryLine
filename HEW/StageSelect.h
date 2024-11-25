
#pragma once

enum E_SELECT_TYPE
{
	STAGE1 = 0,
	STAGE2,
	STAGE3
};

//プロトタイプ宣言
void InitStageSelect();
void UninitStageSelect();
void UpdateStageSelect();
void DrawStageSelect();

int GetStageNum();