#include "InputEx.h"

bool WihtGetKeyPress(WORD ControllerKey, BYTE KeyboardKey)
{
	return IsKeyPress(KeyboardKey) || CGetButtons(ControllerKey);
}

bool WithGetKeyTriger(WORD ControllerKey, BYTE KeyboardKey)
{
	return IsKeyTrigger(KeyboardKey) || CGetButtonsTriger(ControllerKey);
}