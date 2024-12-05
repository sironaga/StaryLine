/* コントローラーとキーボードの入出力を同時に返す */

#pragma once
#include"Controller.h"
#include"Input.h"
bool WithGetKeyTriger(WORD ControllerKey, BYTE KeyboardKey);
bool WihtGetKeyPress(WORD ControllerKey, BYTE KeyboardKey);

