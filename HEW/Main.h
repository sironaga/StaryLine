#pragma once
/*
* File Main.h
* 2024/10/07 音量のセッターゲッターの追加
* 2024/10/08 モード管理用関数追加
*/

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

/* モード管理用 */
enum Screen
{
	TITLE, /* TITLE */
};

typedef struct
{
	float X;
	float Y;
}FLOAT2;

typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT3;

typedef struct
{
	float X;
	float Y;
	float Z;
	float W;
}FLOAT4;

typedef struct
{
	int time;
	int oldTime;
	int fpsCount;
	int FPS;
	int fpsTime;
}FPSTIMER;

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);
// リソースパス
#define ASSET(path)	"Assets/"path

// 3D空間定義
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))
static const float GRAVITY = 0.98f;

// グリッドサイズ(デバッグ用
static const int	DEBUG_GRID_NUM = 10;			// グリッド中心から端までの線の本数
static const float	DEBUG_GRID_MARGIN = METER(1.0f);	// グリッド配置幅

void SetVolume(int InVolume);	/* g_nSoundVolumeの変数を変更します */
int GetVolume();				/* g_nSoundVolumeの変数の値を取得します*/
void SetScreen(Screen InScreen);/* g_Screenの変数の値を変更します */