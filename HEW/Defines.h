#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);
// リソースパス
#define ASSET(path)	"Assets/" path

#define EFFECT_PASS(efkser) "Assets/Effect/" efkser
#define MODEL_PASS(fbx) ("Assets/Model/" fbx)
#define TEX_PASS(tex) ("Assets/Texture/" tex)
#define SOUND_PASS(sound) ("Assets/Sound/" sound)
#define CSV_PASS(csv) ("Assets/Sound/" csv)

#define PI (3.1415f)
#define TORAD(deg) ((deg / 180.0f) * PI)
#define TODEF(rad) (((rad) / PI) * 180.0f)

#define	MILLISEC (1.0f / 60.0f)

#define AJUST_MINMAX(value,inmin,inmax) do{if(value <= inmin){value = inmin;}if(value >= inmax){value = inmax;}}while(0)

// 3D空間定義
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))
static const float GRAVITY = 0.98f;

// タイトル
static const char* APP_TITLE = "Starry Line";

// 画面サイズ
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static int NOW_SCREEN_WIDTH = 1920;
static int NOW_SCREEN_HEIGHT = 1080;
#define SET_SCREEN_RESOLUSION(wide,height) (NOWNOW_SCREEN_WIDTH = wide;NOW_SCREEN_HEIGHT = height;)
#define WIDE_RATIO(num) (((float)NOW_SCREEN_WIDTH/(float)SCREEN_WIDTH)*num)
#define HEIGHT_RATIO(num) (((float)NOW_SCREEN_HEIGHT/(float)SCREEN_HEIGHT)*num)

// グリッドサイズ(デバッグ用
static const int	DEBUG_GRID_NUM = 10;			// グリッド中心から端までの線の本数
static const float	DEBUG_GRID_MARGIN = METER(1.0f);	// グリッド配置幅





#endif // __DEFINES_H__