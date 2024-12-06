#include "StageSelect.h"
#include "Main.h"
#define _DIRECTX_
#include "DirectX.h"
#include "SpriteDrawer.h"
#include "InputEx.h"

E_SELECT_TYPE g_Select_type;

void InitStageSelect()
{
	g_Select_type = STAGE1;
}

void UninitStageSelect()
{

}

void UpdateStageSelect()
{
	switch (g_Select_type)
	{
	case(STAGE1):
		if (IsKeyTrigger(VK_RIGHT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE2; }
		if (IsKeyTrigger(VK_LEFT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type = STAGE3; }
		break;

	case(STAGE2):
		if (IsKeyTrigger(VK_RIGHT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE3; }
		if (IsKeyTrigger(VK_LEFT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type = STAGE1; }
		break;

	case(STAGE3):
		if (IsKeyTrigger(VK_RIGHT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT))	{ g_Select_type = STAGE1; }
		if (IsKeyTrigger(VK_LEFT), CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT))		{ g_Select_type = STAGE2; }
	default: break;
	}
	if (IsKeyTrigger(VK_RETURN), CGetButtonsTriger(XINPUT_GAMEPAD_B)) { ChangeScene(SCENE_GAME); }//ÉVÅ[Éìà⁄çs
}

void DrawStageSelect()
{
	switch (g_Select_type)
	{
	case(STAGE1):	

		break;

	default: break;
	}
}

int GetStageNum()
{
	return g_Select_type;
}
