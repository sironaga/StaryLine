#pragma once
/* Log */
/*
	2024/10/29 秋野翔太　オブジェクトの移動関数の簡略化 Geometoryのみ対応
*/

/* 使用例 */
/*
	SetTranslation(10.0f, 10.0f, 10.0f); Positionデータの書き換え
	Geometory::DrawBox();
	ResetData();						　データのリセット

	2DのときのSetSpriteと使用方法は同じです

*/


/* Include */
#include"Main.h"
#include"DirectX.h"
#include"Geometory.h"
#include"Sprite.h"
#include"Input.h"
#include"ShaderList.h"
#include"Controller.h"

void SetTranslation(float X, float Y, float Z);
void SetTranslation(FLOAT3 SetTranslation);
void SetScaling(float X, float Y, float Z);
void SetScaling(FLOAT3 SetScaling);
void SetRotation(float X, float Y, float Z);
void SetRotation(FLOAT3 SetRotation);
void ResetData();