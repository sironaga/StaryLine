#include "InputEx.h"

bool WihtGetKeyPress(WORD ControllerKey, BYTE KeyboardKey)
{
	return IsKeyPress(KeyboardKey) || CGetButtons(ControllerKey);
}

bool WithGetKeyTriger(WORD ControllerKey, BYTE KeyboardKey)
{
	return IsKeyTrigger(KeyboardKey) || CGetButtonsTriger(ControllerKey);
}

DIRECTION WASDKeyBorad()
{
	bool CheckKey[4];
	for (int nLoop = 0; nLoop < 4; nLoop++)
	{
		CheckKey[nLoop] = false;
	}

	DIRECTION Info;
	Info = D_no;
	CheckKey[0] = IsKeyPress('W');
	CheckKey[1] = IsKeyPress('A');
	CheckKey[2] = IsKeyPress('D');
	CheckKey[3] = IsKeyPress('S');


	// --- １つのキーが押された時
	if (CheckKey[0])
	{
		Info = D_above;
	}
	if (CheckKey[1])
	{
		Info = D_left;
	}
	if (CheckKey[2])
	{
		Info = D_right;
	}
	if (CheckKey[3])
	{
		Info = D_under;
	}

	// ---- 2つのキーが押された時
	if (CheckKey[0] && CheckKey[1])
	{
		Info = D_upper_left;
	}
	if (CheckKey[0] && CheckKey[2])
	{
		Info = D_upper_right;
	}
	if (CheckKey[3] && CheckKey[1])
	{
		Info = D_under_left;
	}
	if (CheckKey[3] && CheckKey[2])
	{
		Info = D_under_right;
	}
	if (CheckKey[0] && CheckKey[3])
	{
		Info = D_no;
	}
	// --- ３つのキーが押された時

	// A W D
	if (CheckKey[0] && CheckKey[1] && CheckKey[2])
	{
		Info = D_above;
	}
	// W D S
	if (CheckKey[0] && CheckKey[2] && CheckKey[3])
	{
		Info = D_right;
	}
	// W A S
	if (CheckKey[0] && CheckKey[1] && CheckKey[3])
	{
		Info = D_left;
	}
	// A S D
	if (CheckKey[1] && CheckKey[2] && CheckKey[3])
	{
		Info = D_under;
	}
	// --- すべてのKeyboardが押された時
	if (CheckKey[0] && CheckKey[1] && CheckKey[2] && CheckKey[3])
	{
		Info = D_no;
	}
	return Info;
}