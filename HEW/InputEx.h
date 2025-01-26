/* コントローラーとキーボードの入出力を同時に返す */

#pragma once
#include"Controller.h"
#include"Input.h"
enum  DIRECTION
{
	D_above,
	D_upper_right,
	D_upper_left,
	D_right,
	D_left,
	D_under,
	D_under_right,
	D_under_left,
	D_no,
};

bool WithGetKeyTriger(WORD ControllerKey, BYTE KeyboardKey);
bool WihtGetKeyPress(WORD ControllerKey, BYTE KeyboardKey);
DIRECTION WASDKeyBorad();
WORD GetControllerLStickTriggerForeDirection();
WORD GetControllerLStickPressForeDirection();