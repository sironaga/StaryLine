/*
	XInputを使用したControllerの入出力
	2024/10/13 制作開始 動作未確認　　秋野
*/

/* Include */
#include"Controller.h"


/* define */
#define PAD_USER_NUMBER (0)

/* 構造体 */
XINPUT_STATE def;
XINPUT_STATE state;

/* ゲームパッドの更新処理 */
void Controller_Update()
{

	XInputGetState(PAD_USER_NUMBER, &state);
}

/* 左アナログスティックの座標獲得 */
/* -32768 ～ 32767 */
FLOAT2 CGetLStick()
{
	FLOAT2 Pos;

	Pos.X = state.Gamepad.sThumbLX;
	Pos.Y = state.Gamepad.sThumbLY;

	return Pos;
}

/* 右アナログスティックの座標獲得 */
/* -32768 ～ 32767 */
FLOAT2 CGetRStick()
{
	FLOAT2 Pos;

	Pos.X = state.Gamepad.sThumbRX;
	Pos.Y = state.Gamepad.sThumbRY;

	return Pos;
}

/* トリガーの値を返します */
/* x 左トリガー */
/* y 右トリガー */
/* 0 ～　255 */
FLOAT2 CGetTrigger()
{
	FLOAT2 Pos;

	Pos.X = state.Gamepad.bLeftTrigger;
	Pos.Y = state.Gamepad.bRightTrigger;

	return Pos;
}

/* Buttonの入出力 */
/*
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/
bool CGetButtons(WORD InButton)
{
	return (state.Gamepad.wButtons & InButton);
}
